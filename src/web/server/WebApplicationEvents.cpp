//---------------------------------------------------------------------------
// WebApplicationEvents Implementation
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
#include "WebApplicationEvents.h"		// Include WebApplicationEvents decls
#include "Host.h"						// Include Host declarations
#include "WebServer.h"					// Include WebServer declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// WebApplicationEventArgs Implementation
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// WebApplicationEventArgs Constructor
//
// Arguments:
//
//	host			- Reference to the Host for this application

WebApplicationEventArgs::WebApplicationEventArgs(Host^ host)
{
	if(host == nullptr) return;			// <-- SPECIAL CASE FOR "EMPTY"

	m_appid = host->ApplicationID;			// Copy the application ID
	m_config = host->Configuration;			// Reference the configuration
	m_requests = host->RequestsHandled;		// Get the number of reqs handled
	m_restarts = host->RestartCount;		// Get the restart indicator
}

//---------------------------------------------------------------------------
// WebApplicationEvent Implementation
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// WebApplicationEvent::Invoke 
//
// Invokes the callback of the derived class in the proper app domain
//
// Arguments:
//
//	NONE

Object^ WebApplicationEvent::Invoke(void)
{
	Debug::Assert(m_domain != nullptr);
	m_domain->DoCallBack(gcnew CrossAppDomainDelegate(this, &WebApplicationEvent::DoCallback));
	return m_domain->GetData(RESULT_OBJECT);
}

//---------------------------------------------------------------------------
// WebApplicationActivityEvent Implementation
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// WebApplicationActivityEvent::DoCallback (protected)
//
// Actually invokes the method on the static WebServer instance from the
// application domain that the Host was created from
//
// Arguments:
//
//	NONE

void WebApplicationActivityEvent::DoCallback(void)
{
	WebServer::OnApplicationActivity(m_appid, m_activity);
	AppDomain::CurrentDomain->SetData(RESULT_OBJECT, nullptr);
}

//---------------------------------------------------------------------------
// WebApplicationRestartFailureEvent Implementation
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// WebApplicationRestartFailureEvent::DoCallback (protected)
//
// Actually invokes the method on the static WebServer instance from the
// application domain that the Host was created from
//
// Arguments:
//
//	NONE

void WebApplicationRestartFailureEvent::DoCallback(void)
{
	WebServer::OnApplicationRestartFailure(m_appid, m_exception);
	AppDomain::CurrentDomain->SetData(RESULT_OBJECT, nullptr);
}

//---------------------------------------------------------------------------
// WebApplicationStartedEvent Implementation
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// WebApplicationStartedEvent::DoCallback (protected)
//
// Actually invokes the method on the static WebServer instance from the
// application domain that the Host was created from
//
// Arguments:
//
//	NONE

void WebApplicationStartedEvent::DoCallback(void)
{
	WebServer::OnApplicationStarted(m_appid, m_args);
	AppDomain::CurrentDomain->SetData(RESULT_OBJECT, nullptr);
}

//---------------------------------------------------------------------------
// WebApplicationStartingEvent Implementation
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// WebApplicationStartingEvent::DoCallback (protected)
//
// Actually invokes the method on the static WebServer instance from the
// application domain that the Host was created from
//
// Arguments:
//
//	NONE

void WebApplicationStartingEvent::DoCallback(void)
{
	WebServer::OnApplicationStarting(m_appid, m_args);
	AppDomain::CurrentDomain->SetData(RESULT_OBJECT, nullptr);
}

//---------------------------------------------------------------------------
// WebApplicationStoppedEvent Implementation
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// WebApplicationStoppedEvent::DoCallback (protected)
//
// Actually invokes the method on the static WebServer instance from the
// application domain that the Host was created from
//
// Arguments:
//
//	NONE

void WebApplicationStoppedEvent::DoCallback(void)
{
	WebServer::OnApplicationStopped(m_appid, m_args);
	AppDomain::CurrentDomain->SetData(RESULT_OBJECT, nullptr);
}

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
