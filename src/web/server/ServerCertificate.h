//---------------------------------------------------------------------------
// ServerCertificate Declarations
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

#ifndef __SERVERCERTIFICATE_H_
#define __SERVERCERTIFICATE_H_
#pragma once

#include "ProjectUtil.h"				// Include ProjectUtil declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Diagnostics;
using namespace System::Net;
using namespace System::Reflection;
using namespace System::Runtime::InteropServices;
using namespace System::Security;
using namespace System::Security::Permissions;
using namespace System::Web;
using namespace msclr;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class ServerCertificate (internal)
//
// ServerCertificate implements a wrapper around the HTTPS server certificate
// access.  Unfortunately for us, the designers behind HttpListener didn't
// feel it necessary to expose that information, and we have to violate a
// few rules in order to get it.
//---------------------------------------------------------------------------

ref class ServerCertificate
{
public:

	ServerCertificate(HttpListenerRequest^ request);

	//-----------------------------------------------------------------------
	// Properties

	// KeySize
	//
	// Gets the number of bits in the SSL connection key
	property int KeySize
	{
		int get(void);
	}

	// SecretKeySize
	//
	// Gets the number of bits in the server certificate private key
	property int SecretKeySize
	{
		int get(void);
	}

	// Issuer
	//
	// Gets the certificate issuer as a string
	property String^ Issuer
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

private:

	// STATIC CONSTRUCTOR
	static ServerCertificate() { InitializeStatics(); }

	//-----------------------------------------------------------------------
	// Private Member Functions

	// InitializeStatics
	//
	// Initializes the static member variables
	static void InitializeStatics(void);

	//-----------------------------------------------------------------------
	// Member Variables

	bool					m_hasInfo;			// Flag if information is valid
	int						m_keySize;			// Connection key size
	int						m_secureKeySize;	// Private key size
	String^					m_issuer;			// Certificate issuer
	String^					m_subject;			// Certificate subject

	// REFLECTION SCHEMA AND INFORMATION ----------------------------

	static bool						s_valid;			// Flag if reflection data is valid
	static ReflectionPermission^	s_perm;				// Reflection member access permission
	static FieldInfo^				s_reqBlob;			// System.Net.HttpListenerRequest.m_MemoryBlob
	static Type^					s_reqContextType;	// System.Net.RequestContextBase
	static FieldInfo^				s_reqContextBlob;	// System.Net.RequestContextBase.m_OriginalBlobAddress
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __SERVERCERTIFICATE_H_