//---------------------------------------------------------------------------
// ApplicationRestarter Declarations
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

#ifndef __APPLICATIONRESTARTER_H_
#define __APPLICATIONRESTARTER_H_
#pragma once

#include "WebApplicationConfiguration.h"	// Include WebAppConfig decls
#include "WebApplicationEvents.h"			// Include WebApplicationEvents

#pragma warning(push, 4)					// Enable maximum compiler warnings

using namespace System;
using namespace System::Reflection;
using namespace System::Threading;
using namespace System::Web::Hosting;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Forward Class Declarations
//---------------------------------------------------------------------------

ref class Host;								// Host.h

//---------------------------------------------------------------------------
// Class ApplicationRestarter (internal)
//
// Helper class to the Host object that is used to restart an application
// after it's been shut down by the runtime.  This class creates it's own
// isolated Application Domain to do the work, and will destroy said domain
// when it's done trying to restart the application
//---------------------------------------------------------------------------

ref class ApplicationRestarter abstract sealed
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// Restart
	//
	// Restarts an application that was shut down automatically by ASP.NET
	static void Restart(Host^ host);

private:

	//-----------------------------------------------------------------------
	// Private Type Declarations

	// Restarter
	//
	// Class that is instantiated in the isolated application domain to 
	// make the attempt to restart the hosted application
	ref class Restarter
	{
	public:

		// CONSTRUCTOR
		Restarter(ApplicationManager^ appManager, AppDomain^ parentDomain, 
			String^ appid, WebApplicationConfiguration^ config, int restarts);

	private:

		//-------------------------------------------------------------------
		// Private Member Functions

		// WorkerThread
		//
		// Thread used to wait for the old app to die and start the new one
		void WorkerThread(void);

		//-------------------------------------------------------------------
		// Member Variables

		ApplicationManager^				m_appManager;	// App manager
		AppDomain^						m_parentDomain;	// Parent app domain
		String^							m_appid;		// Application ID
		WebApplicationConfiguration^	m_config;		// Application config
		int								m_restarts;		// Number of restarts
	};
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __APPLICATIONRESTARTER_H_