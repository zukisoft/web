//---------------------------------------------------------------------------
// ServerCertificate Implementation
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
#include "ServerCertificate.h"			// Include ServerCertificate decls

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// ServerCertificate Constructor
//
// Arguments:
//
//	request			- HttpListenerRequest object instance

ServerCertificate::ServerCertificate(HttpListenerRequest^ request)
{
	Object^					reqBlob;		// HttpListenerRequest.m_MemoryBlob
	Object^					reqContextBlob;	// RequestContextBase.m_MemoryBlob
	Pointer^				httpRequest;	// Managed HTTP_REQUEST (V1) pointer
	PHTTP_REQUEST			pHttpRequest;	// Unmanaged HTTP_REQUEST (V1) pointer
	PHTTP_SSL_INFO			pSslInfo;		// Unmanaged HTTP_SSL_INFO pointer

	Debug::Assert(request != nullptr);		// <-- One would hope not

	if(!request->IsSecureConnection) return;	// Not HTTPS, nothing to do
	if(!s_valid) return;						// Check static mvar status

	try {

		s_perm->Demand();					// Demand necessary reflection access
		
		// Attempt to access the private m_OriginalBlobAddress variable of the
		// RequestContextBase class contained by the HttpListenerRequest instance

		reqBlob = s_reqBlob->GetValue(request);
		reqContextBlob = s_reqContextBlob->GetValue(reqBlob);
		if((reqBlob == nullptr) || (reqContextBlob == nullptr)) return;

		// Cast the object value back into a Reflection.Pointer, then into a
		// normal old pointer, and make sure that it's somewhat valid

		httpRequest = safe_cast<Pointer^>(reqContextBlob);
		pHttpRequest = reinterpret_cast<PHTTP_REQUEST>(Pointer::Unbox(reqContextBlob));
		if(IsBadReadPtr(pHttpRequest, sizeof(HTTP_REQUEST))) return;

		// That all managed to function, as amazing as that seems, so go ahead and
		// pull out all the things we need into mvars in case the pointer goes bad
		// on us (this is a fairly unsafe operation after all)

		pSslInfo = pHttpRequest->pSslInfo;			// Cast pointer for brevity

		m_keySize = pSslInfo->ConnectionKeySize;
		m_secureKeySize = pSslInfo->ServerCertKeySize;
		m_issuer = ProjectUtil::FastPtrToStringAnsi(pSslInfo->pServerCertIssuer);
		m_subject = ProjectUtil::FastPtrToStringAnsi(pSslInfo->pServerCertSubject);
	}

	catch(Exception^) { return; }			// Just give up on ANY exception
	m_hasInfo = true;						// Successful reflection operation
}

//---------------------------------------------------------------------------
// ServerCertificate::InitializeStatics (private, static)
//
// Acts as the static constructor for the class so it doesn't need to be
// implemented in the header file
//
// Arguments:
//
//	NONE

void ServerCertificate::InitializeStatics(void)
{
	Assembly^			assembly;							// Referenced assembly

	// If the static permission hasn't been created, go ahead and create it
	// now.  There shouldn't be any reason for this particular part to fail

	if(s_perm == nullptr) s_perm = gcnew ReflectionPermission(ReflectionPermissionFlag::MemberAccess);

	try {
	
		// 1. Demand the necessary Reflection access permissions are present

		s_perm->Demand();

		// 2. System.dll Assembly

		assembly = HttpListenerRequest::typeid->Assembly;

		// 3. HttpListenerRequest.m_MemoryBlob

		s_reqBlob = HttpListenerRequest::typeid->GetField("m_MemoryBlob", BindingFlags::Instance | BindingFlags::NonPublic);
		if(s_reqBlob == nullptr) return;

		// 4. RequestContextBase

		s_reqContextType = assembly->GetType("System.Net.RequestContextBase", true, true);

		// 5. RequestContextBase.m_OriginalBlobAddress

		s_reqContextBlob = s_reqContextType->GetField("m_OriginalBlobAddress", BindingFlags::Instance | BindingFlags::NonPublic);
		if(s_reqContextBlob == nullptr) return;
	} 
	
	catch(Exception^) { return; }

	s_valid = true;				// Did we manage to get through all of that?
}

//---------------------------------------------------------------------------
// ServerCertificate::KeySize::get
//
// Gets the number of bits in the SSL connection key

int ServerCertificate::KeySize::get(void)
{
	return (m_hasInfo) ? m_keySize : 0;
}

//---------------------------------------------------------------------------
// ServerCertificate::SecretKeySize::get
//
// Gets the number of bits in the server certificate private key

int ServerCertificate::SecretKeySize::get(void)
{
	return (m_hasInfo) ? m_secureKeySize : 0;
}

//---------------------------------------------------------------------------
// ServerCertificate::Issuer::get
//
// Gets the issuer of the certificate

String^ ServerCertificate::Issuer::get(void)
{
	return (m_hasInfo) ? m_issuer : String::Empty;
}

//---------------------------------------------------------------------------
// ServerCertificate::Subject::get
//
// Gets the subject field of the client certificate

String^ ServerCertificate::Subject::get(void)
{
	return (m_hasInfo) ? m_subject : String::Empty;
}

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
