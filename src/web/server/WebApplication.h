//---------------------------------------------------------------------------
// WebApplication Declarations
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

#ifndef __WEBAPPLICATION_H_
#define __WEBAPPLICATION_H_
#pragma once

#include "ApplicationRestarter.h"			// Include ApplicationRestarter
#include "Host.h"							// Include Host declarations
#include "WebApplicationConfiguration.h"	// Include WebAppConfig decls

#pragma warning(push, 4)					// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Threading;
using namespace System::Web;
using namespace System::Web::Hosting;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class WebApplication
//
// WebApplication is a lightweight wrapper around a Host object instance that
// allows the calling application to manipulate hosts without being able to
// work with them directly.  At one point this was hooked directly to the
// Host object via a convoluted series of events and marshalers, but it has
// been changed to a more detached implementation.  This was mainly done 
// because Hosts can be restarted and the management of keeping this wrapper
// connected was almost pointless.
//---------------------------------------------------------------------------

public ref class WebApplication sealed
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// CreateObject
	//
	// Creates an object in the hosted application domain
	IRegisteredObject^ CreateObject(Type^ type, bool failIfExists);

	// DestroyObject
	//
	// Destroys an object in the hosted application domain
	void DestroyObject(Type^ type);

	// GetLoadedAssemblies
	//
	// Gets a List<> of all the assemblies loaded into the application
	Generic::IList<AssemblyName^>^ GetLoadedAssemblies(void);

	// GetObject
	//
	// Retrieves an object from the hosted application domain
	IRegisteredObject^ GetObject(Type^ type);

	// Restart
	//
	// Restarts the web application
	void Restart(void);

	// Shutdown
	//
	// Shuts down and disposes of the web application
	void Shutdown(void);

	//-----------------------------------------------------------------------
	// Properties

	// Configuration
	//
	// Accesses the web application configuration data
	property WebApplicationConfiguration^ Configuration
	{
		WebApplicationConfiguration^ get(void);
	}

	// ID
	//
	// Exposes the application identifier string
	property String^ ID
	{
		String^ get(void) { return m_appid; }
	}

internal:

	// INTERNAL CONSTRUCTOR
	WebApplication(ApplicationManager^ manager, String^ appid) : m_manager(manager),
		m_appid(appid) {}

private:

	//-----------------------------------------------------------------------
	// Private Member Functions

	// ShutdownApplication
	//
	// Helper function to shut down an application and actually wait for it
	static void ShutdownApplication(ApplicationManager^ manager, String^ appid);

	//-----------------------------------------------------------------------
	// Member Variables

	ApplicationManager^			m_manager;		// Application Manager instance
	String^						m_appid;		// Application ID string
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __WEBAPPLICATION_H_