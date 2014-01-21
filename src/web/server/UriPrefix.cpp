//---------------------------------------------------------------------------
// UriPrefix Implementation
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
#include "UriPrefix.h"					// Include UriPrefix declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// UriPrefix::Construct (private)
//
// Acts as the class pseudo-constructor since C++/CLI doesn't allow ctors
// to invoke each other with the fancy C# this() format in the initializer
//
// Arguments:
//
//	scheme		- URI scheme (http/https)
//	host		- Host or IP address to listen on

void UriPrefix::Construct(UriPrefixScheme scheme, UriPrefixHost host)
{
	m_scheme = scheme;			// Copy the scheme as-is
	m_host = host;				// Copy the host name as-is

	// Automatically select a default port for the provided scheme

	m_port = (scheme == UriPrefixScheme::Http) ? UriPrefixPort::Http : UriPrefixPort::Https;

	if((m_port < 0) || (m_port > 65535)) throw gcnew InvalidUriPortException(m_port);
}

//---------------------------------------------------------------------------
// UriPrefix::Construct (private)
//
// Acts as the class pseudo-constructor since C++/CLI doesn't allow ctors
// to invoke each other with the fancy C# this() format in the initializer
//
// Arguments:
//
//	scheme		- URI scheme (http/https)
//	host		- Host or IP address to listen on
//	port		- TCP port to listen on

void UriPrefix::Construct(UriPrefixScheme scheme, UriPrefixHost host, UriPrefixPort port)
{
	m_scheme = scheme;			// Copy the scheme as-is
	m_host = host;				// Copy the host name as-is
	m_port = port;				// Copy the port as-is

	if((m_port < 0) || (m_port > 65535)) throw gcnew InvalidUriPortException(m_port);
}

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
