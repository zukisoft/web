//---------------------------------------------------------------------------
// WebApplicationEnumerator Declarations
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

#ifndef __WEBAPPLICATIONENUMERATOR_H_
#define __WEBAPPLICATIONENUMERATOR_H_
#pragma once

#include "Host.h"						// Include Host declarations
#include "WebApplication.h"				// Include WebApplication decls

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Web::Hosting;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class WebApplicationEnumerator
//
// WebApplicationEnumerator implements a custom enumerator that only shows
// web applications registered by this library to the calling application.
//---------------------------------------------------------------------------

public ref class WebApplicationEnumerator sealed : public Generic::IEnumerator<WebApplication^>
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// MoveNext (IEnumerator<T>)
	//
	// Advances the enumerator to the next element position
	virtual bool MoveNext(void);

	// Reset (IEnumerator<T>)
	//
	// Resets the enumerator back to it's default state
	virtual void Reset(void);

	//-----------------------------------------------------------------------
	// Properties

	// Current (IEnumerator<T>)
	//
	// Retrieves the object at the current position in the enumerator
	virtual property WebApplication^ Current 
	{ 
		WebApplication^ get(void); 
	}
	
internal:

	// INTERNAL CONSTRUCTOR
	WebApplicationEnumerator(ApplicationManager^ manager);

private:

	// DESTRUCTOR
	~WebApplicationEnumerator() { m_disposed = true; }

	//-----------------------------------------------------------------------
	// Private Properties

	// _Current (IEnumerator::Current)
	virtual property Object^ _Current 
	{
		Object^ get(void) sealed = Collections::IEnumerator::Current::get { return Current; }
	}

	//-----------------------------------------------------------------------
	// Member Variables

	bool					m_disposed;			// Object disposal flag
	ApplicationManager^		m_manager;			// Referenced application manager
	List<String^>^			m_hosts;			// Enumerated hosts
	int						m_current;			// Current enumerator position
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __WEBAPPLICATIONENUMERATOR_H_