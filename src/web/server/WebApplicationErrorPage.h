//---------------------------------------------------------------------------
// WebApplicationErrorPage Declarations
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

#ifndef __WEBAPPLICATIONERRORPAGE_H_
#define __WEBAPPLICATIONERRORPAGE_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Enum WebApplicationErrorPage (internal)
//
// Provides an enumeration describing a specific host-provided error page.
// This was placed into an enumeration since it will be exposed publically
// at some point to allow the allow the application to provide it's own
// version of the error page.
//---------------------------------------------------------------------------

enum class WebApplicationErrorPage 
{
	ClientCertificateRequired		= 0,			// HTTP 403.7
	SecureChannelRequired			= 1,			// HTTP 403.4
	Unauthorized					= 2,			// HTTP 401
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __WEBAPPLICATIONERRORPAGE_H_