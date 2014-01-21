//---------------------------------------------------------------------------
// WebApplicationEvents Declarations
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

#ifndef __WEBAPPLICATIONEVENTS_H_
#define __WEBAPPLICATIONEVENTS_H_
#pragma once

#include "WebApplicationConfiguration.h"	// Include WebAppConfig decls

#pragma warning(push, 4)					// Enable maximum compiler warnings

using namespace System;
using namespace System::Diagnostics;
using namespace System::Reflection;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// DEFINED IN THIS MODULE
//
// WebApplicationEventArgs
// WebApplicationEventHandler
// WebApplicationActivityHandler
// WebApplicationAssemblyResolveHandler
// WebApplicationExceptionHandler
// WebApplicationEvent
// WebApplicationActivityEvent
// WebApplicationRestartFailureEvent
// WebApplicationStartedEvent
// WebApplicationStoppedEvent
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Forward Class Declarations
//---------------------------------------------------------------------------

ref class Host;							// Host.h
ref class WebServer;					// WebServer.h

//---------------------------------------------------------------------------
// Class WebApplicationEventArgs
//
// Provides the standard event arguments for all WebApplicationEventHandlers
//---------------------------------------------------------------------------

[Serializable()]
public ref class WebApplicationEventArgs : public EventArgs
{
public:

	//-----------------------------------------------------------------------
	// Properties

	// ApplicationID
	//
	// Gets the web application's identifier string
	property String^ ApplicationID
	{
		String^ get(void) { return m_appid; }
	}

	// Configuration
	//
	// Gets a reference to a copy of the web server configuration
	property WebApplicationConfiguration^ Configuration
	{
		WebApplicationConfiguration^ get(void) { return m_config; }
	}

	//-----------------------------------------------------------------------
	// Properties

	// Empty
	//
	// Provides a useless empty instance of the class
	static property WebApplicationEventArgs^ Empty
	{
		WebApplicationEventArgs^ get(void) { return gcnew WebApplicationEventArgs(nullptr); }
	}

	// RequestsHandled
	//
	// Exposes how many requests have been handled by this application
	property __int64 RequestsHandled
	{
		__int64 get(void) { return m_requests; }
	}

	// RestartCount
	//
	// Indicates the number of times this app has been restarted
	property int RestartCount
	{
		int get(void) { return m_restarts; }
	}

internal:

	// INTERNAL CONSTRUCTOR
	WebApplicationEventArgs(Host^ host);

private:

	//-----------------------------------------------------------------------
	// Member Variables

	String^							m_appid;	// Web application ID string
	WebApplicationConfiguration^	m_config;	// Web application configuration
	__int64							m_requests;	// Number of requests handled
	int								m_restarts;	// Number of host restarts
};

//---------------------------------------------------------------------------
// Delegate WebApplicationEventHandler
//
// Delegate type used to communicate unhandled web application events
//---------------------------------------------------------------------------

public delegate void WebApplicationEventHandler(String^ appid, WebApplicationEventArgs^ args);

//---------------------------------------------------------------------------
// Delegate WebApplicationActivityHandler
//
// Delegate type used to communicate activity log events
//---------------------------------------------------------------------------

public delegate void WebApplicationActivityHandler(String^ appid, String^ activity);

//---------------------------------------------------------------------------
// Delegate WebApplicationAssemblyResolveHandler
//
// Delegate type used to communicate a translated assembly resolve event
//---------------------------------------------------------------------------

public delegate Assembly^ WebApplicationAssemblyResolveHandler(String^ appid, ResolveEventArgs^ args);

//---------------------------------------------------------------------------
// Delegate WebApplicationExceptionHandler
//
// Delegate type used to communicate activity log events
//---------------------------------------------------------------------------

public delegate void WebApplicationExceptionHandler(String^ appid, Exception^ exception);

//---------------------------------------------------------------------------
// Class WebApplicationEvent (internal)
//
// WebApplicationEvent serves as the base class for the specific web app
// event classes.  Must be derived from.
//---------------------------------------------------------------------------

[Serializable()]
ref class WebApplicationEvent abstract
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// Invoke
	//
	// Invokes the derived class' callback in the proper app domain
	Object^ Invoke(void);

protected:

	// PROTECTED CONSTRUCTOR
	WebApplicationEvent(AppDomain^ domain, String^ appid, WebApplicationEventArgs^ args) :
	  m_domain(domain), m_appid(appid), m_args(args) {}

	//-----------------------------------------------------------------------
	// Protected Constants

	// RESULT_OBJECT
	//
	// Name assigned to the result object passed across domains
	static initonly String^ RESULT_OBJECT = "__result";

	//-----------------------------------------------------------------------
	// Protected Member Functions

	// DoCallback (abstract)
	//
	// Implemented in the derived class to actually invoke the callback
	virtual void DoCallback(void) abstract;

	//-----------------------------------------------------------------------
	// Protected Member Variables

	initonly AppDomain^					m_domain;	// Target AppDomain object
	initonly String^					m_appid;	// Application ID string
	initonly WebApplicationEventArgs^	m_args;		// Event argument data
};

//---------------------------------------------------------------------------
// WebApplicationActivityEvent (internal)
//
// Specialization of WebApplicationEvent to handle an application activity log
//---------------------------------------------------------------------------

[Serializable()]
ref class WebApplicationActivityEvent : WebApplicationEvent
{
public:

	//-----------------------------------------------------------------------
	// Constructor
	//
	// Arguments:
	//
	//	domain		- AppDomain to invoke the callback in
	//	appid		- Web application ID string
	//	activity	- The web application activity log message

	WebApplicationActivityEvent(AppDomain^ domain, String^ appid, String^ activity) :
	  WebApplicationEvent(domain, appid, WebApplicationEventArgs::Empty),
	  m_activity(activity) {}

protected:

	//-----------------------------------------------------------------------
	// Protected Member Functions

	// DoCallback
	//
	// Executes the callback in the proper application domain
	virtual void DoCallback(void) override;

private:

	//-----------------------------------------------------------------------
	// Member Variables

	String^				m_activity;			// The activity message to send
};

//---------------------------------------------------------------------------
// WebApplicationRestartFailureEvent (internal)
//
// Specialization of WebApplicationEvent to handle an exception
//---------------------------------------------------------------------------

[Serializable()]
ref class WebApplicationRestartFailureEvent : WebApplicationEvent
{
public:

	//-----------------------------------------------------------------------
	// Constructor
	//
	// Arguments:
	//
	//	domain		- AppDomain to invoke the callback in
	//	appid		- Web application ID string
	//	exception	- Exception object that was thrown

	WebApplicationRestartFailureEvent(AppDomain^ domain, String^ appid, Exception^ exception) :
	  WebApplicationEvent(domain, appid, WebApplicationEventArgs::Empty),
	  m_exception(exception) {}

protected:

	//-----------------------------------------------------------------------
	// Protected Member Functions

	// DoCallback
	//
	// Executes the callback in the proper application domain
	virtual void DoCallback(void) override;

private:

	//-----------------------------------------------------------------------
	// Member Variables

	Exception^				m_exception;		// The exception to report
};

//---------------------------------------------------------------------------
// WebApplicationStartedEvent (internal)
//
// Specialization of WebApplicationEvent to handle an application start
//---------------------------------------------------------------------------

[Serializable()]
ref class WebApplicationStartedEvent : WebApplicationEvent
{
public:

	//-----------------------------------------------------------------------
	// Constructor
	//
	// Arguments:
	//
	//	domain		- AppDomain to invoke the callback in
	//	appid		- Web application ID string
	//	args		- Web application event argument data

	WebApplicationStartedEvent(AppDomain^ domain, String^ appid, WebApplicationEventArgs^ args) :
	  WebApplicationEvent(domain, appid, args) {}

protected:

	//-----------------------------------------------------------------------
	// Protected Member Functions

	// DoCallback
	//
	// Executes the callback in the proper application domain
	virtual void DoCallback(void) override;
};

//---------------------------------------------------------------------------
// WebApplicationStartingEvent (internal)
//
// Specialization of WebApplicationEvent to handle an application starting
//---------------------------------------------------------------------------

[Serializable()]
ref class WebApplicationStartingEvent : WebApplicationEvent
{
public:

	//-----------------------------------------------------------------------
	// Constructor
	//
	// Arguments:
	//
	//	domain		- AppDomain to invoke the callback in
	//	appid		- Web application ID string
	//	args		- Web application event argument data

	WebApplicationStartingEvent(AppDomain^ domain, String^ appid, WebApplicationEventArgs^ args) :
	  WebApplicationEvent(domain, appid, args) {}

protected:

	//-----------------------------------------------------------------------
	// Protected Member Functions

	// DoCallback
	//
	// Executes the callback in the proper application domain
	virtual void DoCallback(void) override;
};

//---------------------------------------------------------------------------
// WebApplicationStoppedEvent (internal)
//
// Specialization of WebApplicationEvent to handle an application stop
//---------------------------------------------------------------------------

[Serializable()]
ref class WebApplicationStoppedEvent : WebApplicationEvent
{
public:

	//-----------------------------------------------------------------------
	// Constructor
	//
	// Arguments:
	//
	//	domain		- AppDomain to invoke the callback in
	//	appid		- Web application ID string
	//	args		- Web application event argument data

	WebApplicationStoppedEvent(AppDomain^ domain, String^ appid, WebApplicationEventArgs^ args) :
	  WebApplicationEvent(domain, appid, args) {}

protected:

	//-----------------------------------------------------------------------
	// Protected Member Functions

	// DoCallback
	//
	// Executes the callback in the proper application domain
	virtual void DoCallback(void) override;
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif		// __WEBAPPLICATIONEVENTS_H_