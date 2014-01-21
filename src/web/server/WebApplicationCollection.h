//---------------------------------------------------------------------------
// WebApplicationCollection Declarations
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

#ifndef __WEBAPPLICATIONCOLLECTION_H_
#define __WEBAPPLICATIONCOLLECTION_H_
#pragma once

#include "Exceptions.h"						// Include Exceptions declarations
#include "Host.h"							// Include Host declarations
#include "WebApplication.h"					// Include WebApplication decls
#include "WebApplicationConfiguration.h"	// Include WebApplicationConfig
#include "WebApplicationEnumerator.h"		// Include WebApplicationEnumerator

#pragma warning(push, 4)					// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Web;
using namespace System::Web::Hosting;
using namespace zuki::web::server::exceptions;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class WebApplicationCollection
//
// WebApplicationCollection implements a collection of WebApplication objects.
// This collection is used to manage the running applications on the server.
// WebApplications are just lightweight wrappers around the underlying Host
// object instances.
//---------------------------------------------------------------------------

public ref class WebApplicationCollection sealed : public Generic::IEnumerable<WebApplication^>
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// Contains
	//
	// Determines if a web application with the specified app id exists
	bool Contains(String^ appid);

	// Create
	//
	// Creates a new web application based on the provided configuration
	WebApplication^ Create(String^ appid, WebApplicationConfiguration^ config);

	// GetEnumerator (IEnumerable<T>)
	//
	// Creates and returns a new collection enumerator instance
	virtual Generic::IEnumerator<WebApplication^>^ GetEnumerator(void);

	//-----------------------------------------------------------------------
	// Properties

	// Count
	//
	// Gets the number of running web application instances
	property int Count
	{
		int get(void);
	}

	// Item
	//
	// Retrieves a WebApplication instance via it's application id string
	property WebApplication^ default[String^]
	{
		WebApplication^ get(String^ appid);
	}

internal:

	// INTERNAL CONSTRUCTOR
	WebApplicationCollection(ApplicationManager^ manager) : m_appManager(manager) {}

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// ShutdownAll
	//
	// Shuts down all running web applications on this server
	void ShutdownAll(void);

private:

	//-----------------------------------------------------------------------
	// Private Member Functions

	// _GetEnumerator (IEnumerator::GetEnumerator)
	//
	// Privatized interface implementation
	virtual Collections::IEnumerator^ _GetEnumerator(void) sealed = 
		Collections::IEnumerable::GetEnumerator { return GetEnumerator(); }

	//-----------------------------------------------------------------------
	// Member Variables

	ApplicationManager^		m_appManager;	// Application manager instance
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __WEBAPPLICATIONCOLLECTION_H_