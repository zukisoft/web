//---------------------------------------------------------------------------
// UriPrefix Declarations
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

#ifndef __URIPREFIX_H_
#define __URIPREFIX_H_
#pragma once

#include "exceptions.h"					// Include project exception decls
#include "UriPrefixHost.h"				// Include UriPrefixHost declarations
#include "UriPrefixPort.h"				// Include UriPrefixPort declarations
#include "UriPrefixScheme.h"			// Include UriPrefixScheme declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace zuki::web::server::exceptions;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class UriPrefix
//
// UriPrefix represents a single URI prefix for a web application's config
// information.  The URI prefix is combined with the application's virtual
// root to provide the listener with the complete listening prefix
//---------------------------------------------------------------------------

[Serializable()]
public ref class UriPrefix sealed
{
public:

	//-----------------------------------------------------------------------
	// Constructors
	//
	// Arguments:
	//
	//	scheme		- UriPrefixScheme value (http/https)
	//	host		- Host name or IP address to listen on
	//	port		- TCP port to listen on

	UriPrefix(UriPrefixScheme scheme, UriPrefixHost host) { Construct(scheme, host); }
	UriPrefix(UriPrefixScheme scheme, String^ host) { Construct(scheme, UriPrefixHost(host)); }
	UriPrefix(UriPrefixScheme scheme, UriPrefixHost host, UriPrefixPort port) { Construct(scheme, host, port); }
	UriPrefix(UriPrefixScheme scheme, String^ host, UriPrefixPort port) { Construct(scheme, UriPrefixHost(host), port); }
	UriPrefix(UriPrefixScheme scheme, UriPrefixHost host, int port) { Construct(scheme, host, UriPrefixPort(port)); }
	UriPrefix(UriPrefixScheme scheme, String^ host, int port) { Construct(scheme, UriPrefixHost(host), UriPrefixPort(port)); }

	//-----------------------------------------------------------------------
	// Member Functions

	// Equals (Object)
	//
	// Compares this object with another object for equality
	virtual bool Equals(Object^ rhs) override 
	{
		UriPrefix^			uriOther;		// The other UriPrefix instance

		if(rhs->GetType() != UriPrefix::typeid) throw gcnew ArgumentException();
		uriOther = dynamic_cast<UriPrefix^>(rhs);

		if(m_scheme != uriOther->m_scheme) return false;
		if(m_host != uriOther->m_host) return false;
		if(m_port != uriOther->m_port) return false;

		return true;				// Close enough for our purposes, anyway
	}

	// ToString (Object)
	//
	// Converts this instance into it's string representation
	virtual String^ ToString(void) override
	{
		String^ prefix = (gcnew UriBuilder(m_scheme, m_host.ToString(true), m_port))->ToString();
		return (prefix->EndsWith("/")) ? prefix : prefix + "/";
	}

	//-----------------------------------------------------------------------
	// Properties

	// Host
	//
	// Exposes the host name string
	property UriPrefixHost Host
	{
		UriPrefixHost get(void) { return m_host; }
	}

	// Port
	//
	// Exposes the port value
	property UriPrefixPort Port
	{
		UriPrefixPort get(void) { return m_port; }
		internal: void set(UriPrefixPort value) { m_port = value; }
	}

	// Scheme
	//
	// Exposes the scheme (http/https)
	property UriPrefixScheme Scheme
	{
		UriPrefixScheme get(void) { return m_scheme; }
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// ToString
	//
	// Converts this instance into it's string representation
	String^ ToString(String^ path) 
	{
		if(String::IsNullOrEmpty(path)) return ToString();		// Use standard on NULL

		String^ prefix = (gcnew UriBuilder(m_scheme, m_host.ToString(true), m_port, path))->ToString();
		return (prefix->EndsWith("/")) ? prefix : prefix + "/";
	}

private:

	//-----------------------------------------------------------------------
	// Private Member Functions

	// Construct
	//
	// Acts as the class pseudo-constructor
	void Construct(UriPrefixScheme scheme, UriPrefixHost host);
	void Construct(UriPrefixScheme scheme, UriPrefixHost host, UriPrefixPort port);

	//-----------------------------------------------------------------------
	// Member Variables

	UriPrefixScheme				m_scheme;		// URI scheme (http/https)
	UriPrefixHost				m_host;			// URI host name / IP address
	UriPrefixPort				m_port;			// URI TCP port number

	static initonly array<__wchar_t>^ s_trimSlash = {'/'};
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __URIPREFIX_H_