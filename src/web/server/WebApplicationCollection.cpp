//---------------------------------------------------------------------------
// WebApplicationCollection Implementation
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
#include "WebApplicationCollection.h"	// Include WebApplicationCollection decls

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// WebApplicationCollection::Create
//
// Attempts to create a new web application instance inside of this process
//
// Arguments:
//
//	appid		- Web application identifer string
//	config		- Web application configuration information

WebApplication^ WebApplicationCollection::Create(String^ appid, WebApplicationConfiguration^ config)
{
	if(config == nullptr) throw gcnew ArgumentNullException();
	WebApplicationConfiguration::Validate(config);

	Host::Create(m_appManager, AppDomain::CurrentDomain, appid, config, 0);
	return gcnew WebApplication(m_appManager, appid);
}

//---------------------------------------------------------------------------
// WebApplicationCollection::Contains
//
// Determines if an application with the specified ID exists or not
//
// Arguments:
//
//	appid		- Application ID to look for in the collection

bool WebApplicationCollection::Contains(String^ appid)
{
	Host^					host;			// Located host instance

	return ProjectUtil::TryGetHostWithWait(m_appManager, appid, host);
}

//---------------------------------------------------------------------------
// WebApplicationCollection::Count::get
//
// Gets the number of running web applications

int WebApplicationCollection::Count::get(void)
{
	array<ApplicationInfo^>^		apps;		// Running applications

	// This entire collection is dynamic .. we just ask for the array
	// of running applications and report on how many we happen to get back

	apps = m_appManager->GetRunningApplications();
	return apps->Length;
}

//---------------------------------------------------------------------------
// WebApplicationCollection::default::get
//
// Retrieves a WebApplication instance via it's application id
//
// Arguments:
//
//	appid		- Application id string used when creating the application

WebApplication^ WebApplicationCollection::default::get(String^ appid)
{
	Host^					host;			// Registered application host

	if((appid == nullptr) || (appid->Length == 0)) throw gcnew ArgumentNullException();

	// Even though the WebApplication has been detached from the host, we
	// should still make sure it exists before wrapping it.

	if(!ProjectUtil::TryGetHostWithWait(m_appManager, appid, host))
		throw gcnew ApplicationNotFoundException(appid);

	return gcnew WebApplication(m_appManager, appid);	// Wrap and return
}

//---------------------------------------------------------------------------
// WebApplicationCollection::GetEnumerator
//
// Creates and returns a new collection enumerator
//
// Arguments:
//
//	NONE

Generic::IEnumerator<WebApplication^>^ WebApplicationCollection::GetEnumerator(void)
{
	// Just create and return an instance of the custom web app enumerator

	return gcnew WebApplicationEnumerator(m_appManager);
}

//---------------------------------------------------------------------------
// WebApplicationCollection::ShutdownAll (internal)
//
// Shuts down all of the web application instances running on this server
//
// Arguments:
//
//	NONE

void WebApplicationCollection::ShutdownAll(void)
{
	// We don't want to just shut down ALL of the managed applications
	// running in this process by calling ShutdownAll(), that would be
	// a little mean.  Use our own enumerator to grab only applications
	// that were registered by this object model and shut them down

	for each(WebApplication^ app in this) app->Shutdown();
}

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
