//---------------------------------------------------------------------------
// ApplicationRestarter Implementation
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
#include "ApplicationRestarter.h"		// Include ApplicationRestarter decls
#include "Host.h"						// Include Host class declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// ApplicationRestarter::Restart (static)
//
// Attempts to restart an application that has been shut down automatically
//
// Arguments:
//
//	host			- The host instance to be restarted

void ApplicationRestarter::Restart(Host^ host)
{
	AppDomain^				restartDomain;		// AppDomain for the restart

	if(host == nullptr) throw gcnew ArgumentNullException();

	// Create an isolated application domain specifically for the restart
	// operation (so it doesn't disappear when the calling domain does)

	restartDomain = AppDomain::CreateDomain("restart-" + AppDomain::CurrentDomain->FriendlyName);

	// Create an instance of the private Restarter class in the new application domain.
	// From here on out it will handle the rest ...

	// 02.20.2012 - Removed final evidence parameter (nullptr) due to obsolete warning
	restartDomain->CreateInstance(ApplicationRestarter::Restarter::typeid->Assembly->FullName,
		ApplicationRestarter::Restarter::typeid->FullName, false,
		BindingFlags::CreateInstance | BindingFlags::Public | BindingFlags::Instance, 
		nullptr, gcnew array<Object^>{host->ApplicationManager, host->ParentDomain, 
		host->ApplicationID, host->Configuration, host->RestartCount}, nullptr, nullptr);

	restartDomain = nullptr;					// Release strong reference
}

//---------------------------------------------------------------------------
// ApplicationRestarter::Restarter Constructor
//
// Arguments:
//
//	appManager		- OUTER application maanger instance to use
//	parentDomain	- Parent application domain (for firing events)
//	appid			- Application ID to be restarted anew
//	config			- Original application configuration info
//	restarts		- Number of times the host has been restarted

ApplicationRestarter::Restarter::Restarter(ApplicationManager^ appManager, 
	AppDomain^ parentDomain, String^ appid, WebApplicationConfiguration^ config, int restarts)
{
	Thread^				worker;			// Restart worker thread instance

	m_appManager = appManager;			// Save off the application manager
	m_parentDomain = parentDomain;		// Save off the parent AppDomain
	m_appid = appid;					// Save off the application ID string
	m_config = config;					// Save off the configuration
	m_restarts = restarts + 1;			// Increment and save off the restart counter

	// Use a worker thread to actually sit around and try to restart the application

	worker = gcnew Thread(gcnew ThreadStart(this, &ApplicationRestarter::Restarter::WorkerThread));
	worker->IsBackground = true;
	worker->Start();
}

//---------------------------------------------------------------------------
// ApplicationRestarter::WorkerThread (private)
//
// Worker thread responsible for restarting the application as well as getting
// rid of the application domain constructed by the parent class for us
//
// Arguments:
//
//	NONE

void ApplicationRestarter::Restarter::WorkerThread(void)
{
	Host^								host;		// Located Host instance
	WebApplicationRestartFailureEvent^	failure;	// Failure event instance

	// In testing, it appeared that the host is ALWAYS removed from the
	// application manager before we ever get here, but just in case it's better
	// to just wait a little bit to make sure it's good and dead

	host = dynamic_cast<Host^>(m_appManager->GetObject(m_appid, Host::typeid));
	if(host != nullptr) host->StopWaitHandle->WaitOne();
	
	// Attempt to create a brand new Host that matches the old one.  In the
	// event the application cannot be restarted, notify the WebServer instance

	try { Host::Create(m_appManager, m_parentDomain, m_appid, m_config, m_restarts); }
	catch(Exception^ ex) {

		failure = gcnew WebApplicationRestartFailureEvent(m_parentDomain, m_appid, ex);
		failure->Invoke();
	}

	// When we're all done, get rid of the isolated application domain. Parts
	// of the .NET runtime execute the Sleep(250) before unloading, so we'll
	// do the same thing here, although in testing this always worked anyway

	Thread::Sleep(250);
	try { AppDomain::Unload(Thread::GetDomain()); }
	catch(Exception^) { /* DO NOTHING */ }
}

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
