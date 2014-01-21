//---------------------------------------------------------------------------
// WebServer Declarations
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

#ifndef __WEBSERVER_H_
#define __WEBSERVER_H_
#pragma once

#include "WebApplicationCollection.h"		// Include WebAppCollection decls
#include "WebApplicationEvents.h"			// Include WebApplicationEvents decls

#pragma warning(push, 4)					// Enable maximum compiler warnings

using namespace System;
using namespace System::Security::Permissions;
using namespace System::Security::Principal;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class WebServer (static)
//
// WebServer implements the top level access into the web server library.
// It maintains the global web application management collection and performs
// other process-wide tasks (when I know what those will be).
//---------------------------------------------------------------------------

[PermissionSet(SecurityAction::LinkDemand, Name="FullTrust")]
[PermissionSet(SecurityAction::InheritanceDemand, Name="FullTrust")]
public ref class WebServer abstract sealed
{
public:

	//-----------------------------------------------------------------------
	// Events

	// ApplicationActivity
	//
	// Fired when the application has generated activity
	static event WebApplicationActivityHandler^ ApplicationActivity;

	// ApplicationRestartFailure
	//
	// Fired when a web application fails to restart
	static event WebApplicationExceptionHandler^ ApplicationRestartFailure;
	
	// ApplicationStarted
	//
	// Fired when an application has been started
	static event WebApplicationEventHandler^ ApplicationStarted;

	// ApplicationStarting
	//
	// Fired when an application is about to be started
	static event WebApplicationEventHandler^ ApplicationStarting;

	// ApplicationStopped
	//
	// Fired when an application has been stopped
	static event WebApplicationEventHandler^ ApplicationStopped;

	//-----------------------------------------------------------------------
	// Member Functions

	// GetAvailablePort
	//
	// Gets an available TCP port number for automatic port assignments
	static int GetAvailablePort(void);

	// Shutdown
	//
	// Shuts down all web applications running on this server
	static void Shutdown(void)
	{
		s_apps->ShutdownAll();
	}

	//-----------------------------------------------------------------------
	// Properties

	// Applications
	//
	// Accesses the application management collection
	static property WebApplicationCollection^ Applications
	{
		WebApplicationCollection^ get(void) { return s_apps; }
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// OnApplicationActivity
	//
	// Invoked when an application has generated activity
	static void OnApplicationActivity(String^ appid, String^ activity);

	// OnApplicationRestartFailure
	//
	// Invoked when an application could not be restarted
	static void OnApplicationRestartFailure(String^ appid, Exception^ exception);

	// OnApplicationStarted
	//
	// Invoked when an application has been started
	static void OnApplicationStarted(String^ appid, WebApplicationEventArgs^ args);

	// OnApplicationStarting
	//
	// Invoked when an application is about to start
	static void OnApplicationStarting(String^ appid, WebApplicationEventArgs^ args);

	// OnApplicationStopped
	//
	// Invoked when an application has been stopped
	static void OnApplicationStopped(String^ appid, WebApplicationEventArgs^ args);

private:

	// STATIC CONSTRUCTOR
	static WebServer() 
	{
		// Grab a reference to the static application manager, and artificially
		// addref the thing so that if any other code in this process is using
		// the Open()/Close() methodology, it won't accidentially shut down all
		// of the web applications when it makes it's final Close().  In case
		// we want to emply a Close() of our own at some point, pass this instance
		// throughout the object model rather than letting each class use their own.

		s_appManager = ApplicationManager::GetApplicationManager();
		s_appManager->Open();

		s_apps = gcnew WebApplicationCollection(s_appManager);
	}

	//-----------------------------------------------------------------------
	// Member Variables

	static ApplicationManager^			s_appManager;	// AppManager instance
	static WebApplicationCollection^	s_apps;			// Contained collection
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __WEBSERVER_H_