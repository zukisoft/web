//---------------------------------------------------------------------------
// WebServer Implementation
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
#include "WebServer.h"					// Include WebServer declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// WebServer::GetAvailablePort
//
// Locates an available TCP port that can be used for dynamic configurations
//
// Arguments:
//
//	NONE

int WebServer::GetAvailablePort(void)
{
	// Implementation moved to ProjectUtil so it can be used elsewhere
	return ProjectUtil::FindOpenTcpPort();
}

//---------------------------------------------------------------------------
// WebServer::OnApplicationActivity
//
// Invoked when a web application has generated an activity log
//
// Arguments:
//
//	appid		- Application ID string of the reference application
//	activity	- Activity log string

void WebServer::OnApplicationActivity(String^ appid, String^ activity)
{
	try { ApplicationActivity(appid, activity); }
	catch(Exception^) { /* DO NOTHING */ }
}

//---------------------------------------------------------------------------
// WebServer::OnApplicationRestartFailure
//
// Invoked when a web application could not be restarted
//
// Arguments:
//
//	appid		- Application ID string of the reference application
//	exception	- Exception that occurred during the restart

void WebServer::OnApplicationRestartFailure(String^ appid, Exception^ exception)
{
	try { ApplicationRestartFailure(appid, exception); }
	catch(Exception^) { /* DO NOTHING */ }
}

//---------------------------------------------------------------------------
// WebServer::OnApplicationStarted
//
// Invoked when a web application has been started, and in turn fires off
// the public event
//
// Arguments:
//
//	appid		- Application ID string of the reference application
//	args		- WebApplicationEventArgs information for the event

void WebServer::OnApplicationStarted(String^ appid, WebApplicationEventArgs^ args)
{
	try { ApplicationStarted(appid, args); }
	catch(Exception^) { /* DO NOTHING */ }
}

//---------------------------------------------------------------------------
// WebServer::OnApplicationStarting
//
// Invoked when a web application is about to start, and in turn fires off
// the public event
//
// Arguments:
//
//	appid		- Application ID string of the reference application
//	args		- WebApplicationEventArgs information for the event

void WebServer::OnApplicationStarting(String^ appid, WebApplicationEventArgs^ args)
{
	try { ApplicationStarting(appid, args); }
	catch(Exception^) { /* DO NOTHING */ }
}

//---------------------------------------------------------------------------
// WebServer::OnApplicationStopped
//
// Invoked when a web application has been stopped, and in turn fires off
// the public event
//
// Arguments:
//
//	appid		- Application ID string of the reference application
//	args		- WebApplicationEventArgs information for the event

void WebServer::OnApplicationStopped(String^ appid, WebApplicationEventArgs^ args)
{
	try { ApplicationStopped(appid, args); }
	catch(Exception^) { /* DO NOTHING */ }
}

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
