//---------------------------------------------------------------------------
// WebApplication Implementation
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
#include "WebApplication.h"				// Include WebApplication declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// WebApplication::Configuration::get
//
// Accesses a copy of the web application configuration data

WebApplicationConfiguration^ WebApplication::Configuration::get(void)
{
	Host^					host;			// Registered application host

	// Use the ProjectUtil helper to get the host with a wait period in 
	// case it's in the middle of restarting or something here

	if(!ProjectUtil::TryGetHostWithWait(m_manager, m_appid, host))
		throw gcnew ApplicationNotFoundException(m_appid);

	return host->Configuration;		// <-- This will make a COPY via serialization
}

//---------------------------------------------------------------------------
// WebApplication::CreateObject
//
// Creates an object in the hosted application domain
//
// Arguments:
//
//	type			- Type of the object to be created (IRegisteredObject)
//	failIfExists	- Flag to fail if the object exists or return existing

IRegisteredObject^ WebApplication::CreateObject(Type^ type, bool failIfExists)
{
	WebApplicationConfiguration^	config;		// Application configuration data

	if(type == nullptr) throw gcnew ArgumentNullException();

	// The application configuration data gets serialized across the app domains,
	// so access it only once to prevent that from happening multiple times

	config = Configuration::get();

	// Attempt to create the object in the hosted application domain, specifying
	// the same configuration information we did to set up the host itself.

	return m_manager->CreateObject(m_appid, type, config->VirtualRoot,
		config->PhysicalRootFull, failIfExists, true);
}

//---------------------------------------------------------------------------
// WebApplication::DestroyObject
//
// Stops an existing object in the hosted application domain
//
// Arguments:
//
//	type			- Type of the object to be destroyed (IRegisteredObject)

void WebApplication::DestroyObject(Type^ type)
{
	if(type == nullptr) throw gcnew ArgumentNullException();

	m_manager->StopObject(m_appid, type);			// Invoke manager
}

//---------------------------------------------------------------------------
// WebApplication::GetLoadedAssemblies
//
// Gets a List<> of all the assemblies loaded into the application
//
// Arguments:
//
//	NONE

Generic::IList<AssemblyName^>^ WebApplication::GetLoadedAssemblies(void)
{
	Host^					host;			// Registered application host
	array<AssemblyName^>^	asmNames;		// Array of loaded assembly names

	// Use the ProjectUtil helper to get the host with a wait period in 
	// case it's in the middle of restarting or something here

	if(!ProjectUtil::TryGetHostWithWait(m_manager, m_appid, host))
		throw gcnew ApplicationNotFoundException(m_appid);

	// Grab the list of aseemblies, convert it to read only and return it
	// as a generic IList<Assembly^>^ back to the caller

	asmNames = host->GetLoadedAssemblies();
	return static_cast<Generic::IList<AssemblyName^>^>(Array::AsReadOnly(asmNames)); 
}

//---------------------------------------------------------------------------
// WebApplication::GetObject
//
// Gets an object from the hosted application domain
//
// Arguments:
//
//	type			- Type of the object to be created (IRegisteredObject)

IRegisteredObject^ WebApplication::GetObject(Type^ type)
{
	if(type == nullptr) throw gcnew ArgumentNullException();

	return m_manager->GetObject(m_appid, type);		// Invoke manager
}

//---------------------------------------------------------------------------
// WebApplication::Restart
//
// Restarts the web application
//
// Arguments:
//
//	NONE

void WebApplication::Restart(void)
{
	Host^						host;			// Registered application host

	// Use the ProjectUtil helper to get the host with a wait period in 
	// case it's in the middle of restarting or something here

	if(!ProjectUtil::TryGetHostWithWait(m_manager, m_appid, host))
		throw gcnew ApplicationNotFoundException(m_appid);

	// Note that when we manually shut down a host like this, it will
	// not be automatically restarted, but it's still not hard to accomplish

	ShutdownApplication(m_manager, m_appid);	// Shut it down first
	ApplicationRestarter::Restart(host);		// Restart the host instance

	// Wait for the host to restart (or fail trying) before returning to caller

	ProjectUtil::TryGetHostWithWait(m_manager, m_appid, host);
}

//---------------------------------------------------------------------------
// WebApplication::Shutdown
//
// Shuts down the web application
//
// Arguments:
//
//	NONE

void WebApplication::Shutdown(void)
{
	ShutdownApplication(m_manager, m_appid);	// Shut it down ...
}

//---------------------------------------------------------------------------
// WebApplication::ShutdownApplication (private, static)
//
// Shuts down an application and actually waits for it to stop running
//
// Arguments:
//
//	manager		- ApplicationManager instance
//	appid		- Application identifier string

void WebApplication::ShutdownApplication(ApplicationManager^ manager, String^ appid)
{
	Host^					host;			// Registered application host
	WaitHandle^				stopped;		// Handle to wait for app to stop

	// Use the ProjectUtil helper to get the host with a wait period in 
	// case it's in the middle of restarting or something here

	if(!ProjectUtil::TryGetHostWithWait(manager, appid, host)) return;

	stopped = host->StopWaitHandle;			// Get the wait handle out first

	manager->ShutdownApplication(appid);	// Initiate the shutdown
	stopped->WaitOne();						// Wait for it to really stop
}

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
