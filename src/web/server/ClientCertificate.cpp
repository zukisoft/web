//---------------------------------------------------------------------------
// ClientCertificate Implementation
//
// Zucchini Embedded ASP.NET Web Server
//
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.TXT at the root of this distribution.
// By using this software in any fashion, you are agreeing to be bound by
// the terms of this license. You must not remove this notice, or any other,
// from this software.
//
// Contributor(s):
//	Michael G. Brehm (original author)
//---------------------------------------------------------------------------

#include "stdafx.h"						// Include project pre-compiled headers
#include "ClientCertificate.h"			// Include ClientCertificate decls

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// ClientCertificate Constructor
//
// Arguments:
//
//	request			- HttpListenerRequest object instance

ClientCertificate::ClientCertificate(HttpListenerRequest^ request) : m_pContext(NULL)
{
	Debug::Assert(request != nullptr);

	// Attempt to get the client certificate from the request.  This is surprisingly
	// difficult to set up properly when using HTTP.SYS and you don't want to pay
	// for a true SSL certificate from a trusted authority ...

	try { if(request->IsSecureConnection) m_cert = request->GetClientCertificate(); }
	catch(Exception^) { /* DO NOTHING */ }

	if(m_cert != nullptr) {

		// Use CertDuplicateCertificateContext to add a reference to the existing
		// unmanaged context information.  As of this writing, this does not copy it

		m_pContext = CertDuplicateCertificateContext(reinterpret_cast<PCCERT_CONTEXT>(m_cert->Handle.ToPointer()));
		if(!m_pContext) throw gcnew HttpException("Cannot duplicate client certificate context");
	}
}

//---------------------------------------------------------------------------
// ClientCertificate Destructor

ClientCertificate::~ClientCertificate()
{
	if(m_disposed) return;				// Already disposed of this object

	m_cert = nullptr;					// NULL out the object reference
	this->!ClientCertificate();			// Invoke the finalizer
	m_disposed = true;					// Object has been disposed of
}

//---------------------------------------------------------------------------
// ClientCertificate Finalizer

ClientCertificate::!ClientCertificate()
{
	// Release the reference on the unmanaged certificate context data

	if(m_pContext) CertFreeCertificateContext(m_pContext);
	m_pContext = NULL;
}

//---------------------------------------------------------------------------
// ClientCertificate::BinaryIssuer::get
//
// Gets the encoded issuer information as an array of bytes

array<Byte>^ ClientCertificate::BinaryIssuer::get(void)
{
	array<Byte>^			issuer;			// The array to be returned
	PCERT_INFO				pCertInfo;		// Pointer to the CERT_INFO

	CHECK_DISPOSED(m_disposed);
	if(m_cert == nullptr) return gcnew array<Byte>(0);

	pCertInfo = m_pContext->pCertInfo;		// Cast out for clarity

	// Allocate and initialize a new byte array with the encoded data

	issuer = gcnew array<Byte>(pCertInfo->Issuer.cbData);
	Marshal::Copy(IntPtr(pCertInfo->Issuer.pbData), issuer, 0, pCertInfo->Issuer.cbData);

	return issuer;							// Return the generated array
}

//---------------------------------------------------------------------------
// ClientCertificate::Certificate::get
//
// Gets the encoded client certificate as an array of bytes

array<Byte>^ ClientCertificate::Certificate::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return (m_cert != nullptr) ? m_cert->RawData : gcnew array<Byte>(0);
}

//---------------------------------------------------------------------------
// ClientCertificate::Cookie::get
//
// Gets the unique identifier for the client certificate

String^ ClientCertificate::Cookie::get(void)
{
	const int			MD5_HASH_LENGTH = 16;		// Length of an MD5 hash
	BYTE				hash[MD5_HASH_LENGTH];		// 16 byte MD5 certificate hash
	DWORD				cbHash = MD5_HASH_LENGTH;	// Length of the MD5 hash buffer
	StringBuilder^		cookie;						// Cookie builder
	
	// NOTE: This is not the same way IIS generates the hash value, but it should be
	// sufficient.  I can't for the life of me figure out what it does to get CERT_COOKIE,
	// but it's apparently based on the binary issuer and serial number not the whole cert.
	// (I've tried various permutations of hashes based on those, and they never match)

	CHECK_DISPOSED(m_disposed);
	if(m_cert == nullptr) return String::Empty;

	// Use the certificate API to get the MD5 hash.  If it fails (which it never should),
	// just return an empty string.  Using the API should be faster than doing it managed

	if(!CertGetCertificateContextProperty(m_pContext, CERT_MD5_HASH_PROP_ID, hash, &cbHash)) 
		return String::Empty;

	cookie = gcnew StringBuilder(MD5_HASH_LENGTH * 2);	// <-- 2 characters per byte

	// The cookie appears to just be a big giant hex string, at least it is
	// when you look at what IIS does, so do the same thing here

	for(int index = 0; index < MD5_HASH_LENGTH; index++) cookie->Append(hash[index].ToString("x2"));
	return cookie->ToString();
}

//---------------------------------------------------------------------------
// ClientCertificate::Encoding::get
//
// Gets the encoding used on the client certificate

int ClientCertificate::Encoding::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return (m_cert != nullptr) ? m_pContext->dwCertEncodingType : 0;
}

//---------------------------------------------------------------------------
// ClientCertificate::Flags::get
//
// Gets the certificate validity flags

int ClientCertificate::Flags::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return (m_cert != nullptr) ? 1 : 0;
}

//---------------------------------------------------------------------------
// ClientCertificate::Issuer::get
//
// Gets the issuer of the certificate

String^ ClientCertificate::Issuer::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return (m_cert != nullptr) ? m_cert->Issuer : String::Empty;
}

//---------------------------------------------------------------------------
// ClientCertificate::PublicKey::get
//
// Gets the encoded client certificate public key as an array of bytes

array<Byte>^ ClientCertificate::PublicKey::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return (m_cert != nullptr) ? m_cert->GetPublicKey() : gcnew array<Byte>(0);
}

//---------------------------------------------------------------------------
// ClientCertificate::SerialNumber::get
//
// Provides the certificate serial number as a string

String^ ClientCertificate::SerialNumber::get(void)
{
	array<Byte>^			serialNumber;	// Generated serial number
	PCRYPT_INTEGER_BLOB		pRawSerial;		// Pointer to raw serial number
	PBYTE					pCurrent;		// Current position in buffer

	CHECK_DISPOSED(m_disposed);
	if(m_cert == nullptr) return String::Empty;

	pRawSerial = &m_pContext->pCertInfo->SerialNumber;	// Yank out a pointer
	if(pRawSerial->cbData == 0) return String::Empty;	// <-- No serial number

	// The serial number is stored in Least Significant Byte to Most Significant
	// Byte order in the buffer, AND we're supposed to skip over any leading
	// 0x00 or 0xFF bytes (according to the documentation).  Note that the string
	// exposed by X509Certificate2 is incorrect; it does not reverse the bytes

	pCurrent = pRawSerial->pbData + (pRawSerial->cbData - 1);
	while((pCurrent > pRawSerial->pbData) && ((*pCurrent == 0x00) || (*pCurrent == 0xFF))) pCurrent--;

	// Generate the resultant array and copy the serial number data in backwards.
	// If we managed to back all the way to the beginning this will be zero length,
	// which while I'm certain is incorrect, I'm not certain what to do about it

	serialNumber = gcnew array<Byte>(static_cast<int>(pCurrent - pRawSerial->pbData) + 1);
	for(int index = 0; index < serialNumber->Length; index++) {

		serialNumber[index] = *pCurrent;
		pCurrent--;
	}

	return BitConverter::ToString(serialNumber);	// Convert into hex string
}

//---------------------------------------------------------------------------
// ClientCertificate::Subject::get
//
// Gets the subject field of the client certificate

String^ ClientCertificate::Subject::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return (m_cert != nullptr) ? m_cert->SubjectName->Name : String::Empty;
}

//---------------------------------------------------------------------------
// ClientCertificate::ValidFrom::get
//
// Gets the date when the certificate becomes valid

DateTime ClientCertificate::ValidFrom::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return(m_cert != nullptr) ? m_cert->NotBefore : DateTime::Now;
}

//---------------------------------------------------------------------------
// ClientCertificate::ValidUntil::get
//
// Gets the date when the certificate expires

DateTime ClientCertificate::ValidUntil::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return(m_cert != nullptr) ? m_cert->NotAfter : DateTime::Now;
}

//---------------------------------------------------------------------------
// ClientCertificate::VerifyChain
//
// Verifies the client certificate's SSL chain.  I'll be honest here, I have
// no clue if this was the correct way to implement this or not.  It does seem
// to function the way I envisioned it would, but real world usage will tell
//
// Arguments:
//
//	NONE

bool ClientCertificate::VerifyChain(void)
{
	CERT_CHAIN_PARA					chainPara;		// Chain building parameters
	CERT_CHAIN_POLICY_PARA			policyPara;		// Policy criteria
	CERT_CHAIN_POLICY_STATUS		policyStatus;	// Policy status
	PCCERT_CHAIN_CONTEXT			pChain = NULL;	// Chain context pointer
	BOOL							bResult;		// Result from verification
	
	CHECK_DISPOSED(m_disposed);
	if(m_cert == nullptr) return false;

	// Initialize the CHAIN_PARA, CHAIN_POLICY_PARA and CHAIN_POLICY_STATUS structures,
	// which apparently don't need a whole lot of additional information to just perform
	// a basic verification like I'm trying to accomplish here in this first release

	memset(&chainPara, 0, sizeof(CERT_CHAIN_PARA));
	chainPara.cbSize = sizeof(CERT_CHAIN_PARA);

	memset(&policyPara, 0, sizeof(CERT_CHAIN_POLICY_PARA));
	policyPara.cbSize = sizeof(CERT_CHAIN_POLICY_PARA);

	memset(&policyStatus, 0, sizeof(CERT_CHAIN_POLICY_STATUS));
	policyStatus.cbSize = sizeof(CERT_CHAIN_POLICY_STATUS);

	// Attempt to build the certificate chain we need to perform the verification, and
	// if successful, go ahead and attempt to verify the certificate chain

	if(!CertGetCertificateChain(HCCE_LOCAL_MACHINE, m_pContext, NULL, NULL, &chainPara, 
		0, NULL, &pChain)) return false;

	bResult = CertVerifyCertificateChainPolicy(CERT_CHAIN_POLICY_SSL, pChain, &policyPara, 
		&policyStatus);
	
	CertFreeCertificateChain(pChain);			// Release the certificate chain
	return (bResult) ? true : false;			// Convert into bool and we're done
}

//---------------------------------------------------------------------------
// ClientCertificate::VerifyDates
//
// Determines if the client certificate has expired or is not yet valid
//
// Arguments:
//
//	NONE

bool ClientCertificate::VerifyDates(void)
{
	// Sure, we *could* just look at the dates and do this ourselves, but
	// there's an API, so we better use it ... this is called justification
	// for using C++/CLI instead of C# for this project, I suppose.

	CHECK_DISPOSED(m_disposed);
	return (m_cert != nullptr) ? (CertVerifyTimeValidity(NULL, m_pContext->pCertInfo) == 0) : false;
}

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
