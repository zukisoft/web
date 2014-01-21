//---------------------------------------------------------------------------
// ClientCertificate Declarations
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

#ifndef __CLIENTCERTIFICATE_H_
#define __CLIENTCERTIFICATE_H_
#pragma once

#pragma warning(push, 4)					// Enable maximum compiler warnings

using namespace System;
using namespace System::Diagnostics;
using namespace System::Net;
using namespace System::Runtime::InteropServices;
using namespace System::Security::Cryptography::X509Certificates;
using namespace System::Text;
using namespace System::Web;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class ClientCertificate (internal)
//
// ClientCertificate implements a wrapper around the X.509 client certificate
// details for the Request object.  Most of the information is gathered from
// the unmanaged CERT_CONTEXT, since the X509Certificate2 object exposed by
// HttpListenerRequest doesn't have much of what was needed here.
//---------------------------------------------------------------------------

ref class ClientCertificate
{
public:

	ClientCertificate(HttpListenerRequest^ request);

	//-----------------------------------------------------------------------
	// Member Functions

	// VerifyChain
	//
	// Checks the certificate SSL chain.  This is an optional thing to do
	// for the web application based on configuration, so it's not automatic
	bool VerifyChain(void);

	// VerifyDates
	//
	// Checks the certificate expiration.  This is an optional thing to do
	// for the web application based on configuration, so it's not automatic
	bool VerifyDates(void);

	//-----------------------------------------------------------------------
	// Properties

	// BinaryIssuer
	//
	// Gets the binary issuer information from the client certificate
	property array<Byte>^ BinaryIssuer
	{
		array<Byte>^ get(void);
	}

	// Certificate
	//
	// Gets a reference to the certificate raw binary data
	property array<Byte>^ Certificate
	{
		array<Byte>^ get(void);
	}

	// Cookie
	//
	// Gets the unique ID for the client certificate
	property String^ Cookie
	{
		String^ get(void);
	}

	// Encoding
	//
	// Gets the certificate encoding bitmask
	property int Encoding
	{
		int get(void);
	}

	// Flags
	//
	// Gets the certificate flag bitmask
	property int Flags
	{
		int get(void);
	}

	// IsPresent
	//
	// Gets a value that indicates whether the client certificate is present
	property bool IsPresent
	{
		bool get(void) { return ((Flags & 1) == 1); }
	}

	// Issuer
	//
	// Gets the certificate issuer as a string
	property String^ Issuer
	{
		String^ get(void);
	}

	// IsValid
	//
	// Gets a value that indicates whether the client certificate is valid
	property bool IsValid
	{
		bool get(void) { return ((Flags & 2) == 0); }
	}

	// PublicKey
	//
	// Gets a reference to the raw certificate public key data
	property array<Byte>^ PublicKey
	{
		array<Byte>^ get(void);
	}

	// SerialNumber
	//
	// Provides the certificate serial number as a string
	property String^ SerialNumber
	{
		String^ get(void);
	}

	// Subject
	//
	// Gets the subject field of the client certificate
	property String^ Subject
	{
		String^ get(void);
	}

	// ValidFrom
	//
	// Gets the date when the certificate becomes valid
	property DateTime ValidFrom
	{
		DateTime get(void);
	}

	// ValidUntil
	//
	// Gets the certificate expiration date
	property DateTime ValidUntil
	{
		DateTime get(void);
	}

private:

	// DESTRUCTOR / FINALIZER
	~ClientCertificate();
	!ClientCertificate();

	//-----------------------------------------------------------------------
	// Member Variables

	bool						m_disposed;			// Object disposal flag
	X509Certificate2^			m_cert;				// Wrapped certificate
	PCCERT_CONTEXT				m_pContext;			// CERT_CONTEXT information
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __CLIENTCERTIFICATE_H_