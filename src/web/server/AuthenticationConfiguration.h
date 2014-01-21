//---------------------------------------------------------------------------
// AuthenticationConfiguration Declarations
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

#ifndef __AUTHENTICATIONCONFIGURATION_H_
#define __AUTHENTICATIONCONFIGURATION_H_
#pragma once

#include "exceptions.h"					// Include custom exception decls
#include "XmlConstants.h"				// Include XmlConstants declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Net;
using namespace System::Xml;
using namespace zuki::web::server::exceptions;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class AuthenticationConfiguration
//
// VirtualWebConfiguration is a child object underneath the main config class
// WebApplicationConfiguration, and contains the properties specific to the
// application authentication information
//---------------------------------------------------------------------------

[Serializable()]
public ref class AuthenticationConfiguration sealed
{
public:

	//-----------------------------------------------------------------------
	// Properties

	// CacheNtlmCredentials
	//
	// Defines the flag that instructs the listener to only authenticate
	// NTLM clients once for any given TCP connection
	property bool CacheNtlmCredentials
	{
		bool get(void) { return m_cacheNtlm; }
		void set(bool value);
	}

	// DefaultDomain
	//
	// Defines the default domain to use for basic authentication
	property String^ DefaultDomain
	{
		String^ get(void) { return m_defaultDomain; }
		void set(String^ value); 
	}

	// Realm
	//
	// Defines the realm used with Basic and Digest authentication
	property String^ Realm
	{
		String^ get(void) { return m_realm; }
		void set(String^ value);
	}

	// RequireClientCertificate
	//
	// Configures the application to require client certificates
	property bool RequireClientCertificate
	{
		bool get(void) { return m_requireCert; }
		void set(bool value);
	}

	// Schemes
	//
	// Defines the HTTP authentication schemes that the application will use
	property AuthenticationSchemes Schemes
	{
		AuthenticationSchemes get(void) { return m_schemes; }
		void set(AuthenticationSchemes value); 
	}

	// VerifyClientCertificateChain
	//
	// Indicates that any client certificates should have their chain checked
	// before allowing access to the web application
	property bool VerifyClientCertificateChain
	{
		bool get(void) { return m_verifyCertChain; }
		void set(bool value);
	}

	// VerifyClientCertificateExpiration
	//
	// Indicates that any client certificates should have their dates checked
	// before allowing access to the web application
	property bool VerifyClientCertificateExpiration
	{
		bool get(void) { return m_verifyCertDates; }
		void set(bool value); 
	}

internal:

	// INTERNAL CONSTRUCTOR
	AuthenticationConfiguration(bool writable);

	// ReadXml
	//
	// Reads the state of this instance from an XmlNode
	void ReadXml(XmlNode^ parent);

	// ToXml
	//
	// Writes the state of this instance into an XmlWriter
	void ToXml(XmlWriter^ writer);

private:

	//-----------------------------------------------------------------------
	// Member Variables

	[NonSerialized()] bool		m_writable;			// Writable config flag
	Net::AuthenticationSchemes	m_schemes;			// Authentication schemes
	String^						m_realm;			// Basic authentiation realm
	String^						m_defaultDomain;	// Default logon domain
	bool						m_requireCert;		// Flag to require certs
	bool						m_verifyCertDates;	// Flag to check cert dates
	bool						m_verifyCertChain;	// Flag to check cert chain
	bool						m_cacheNtlm;		// Flag to cache NTLM
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __AUTHENTICATIONCONFIGURATION_H_