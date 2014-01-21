//---------------------------------------------------------------------------
// WebApplicationEnumerator Implementation
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
#include "WebApplicationEnumerator.h"	// Include class declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// WebApplicationEnumerator Constructor (internal)
//
// Arguments:
//
//	manager		- Application Manager instance to use when managing

WebApplicationEnumerator::WebApplicationEnumerator(ApplicationManager^ manager) :
	m_current(-1), m_manager(manager), m_hosts(gcnew List<String^>())
{
	Debug::Assert(m_manager != nullptr);

	// Iterate over all of the running applications in this process, and 
	// each one that can be cast into our Host type gets added into the
	// collection.  I'm not too concerned about hosts getting shutdown after
	// they have been added into an enumerator at the moment.  Maybe later,
	// if it's actually still a concern when this is all said and done

	for each(ApplicationInfo^ info in manager->GetRunningApplications()) {

		Host^ host = dynamic_cast<Host^>(manager->GetObject(info->ID, Host::typeid));
		if(host != nullptr) m_hosts->Add(host->ApplicationID);
	}
}

//---------------------------------------------------------------------------
// WebApplicationEnumerator::Current::get
//
// Retrieves the object at the current enumerator position

WebApplication^ WebApplicationEnumerator::Current::get(void)
{
	CHECK_DISPOSED(m_disposed);

	// The enumerator must be positioned somewhere between the first and last elements
	// If it is, just wrap the Host instance into a new WebApplication instance.  This
	// is somewhat wasteful, but WebApplication is pretty lightweight other than having
	// an event sink, so it should be able to remain this simple of an operation

	if((m_current < 0) || (m_current >= m_hosts->Count)) throw gcnew InvalidOperationException();
	return gcnew WebApplication(m_manager, m_hosts[m_current]);
}

//---------------------------------------------------------------------------
// WebApplicationEnumerator::MoveNext
//
// Advances the enumerator to the next position in the list
//
// Arguments:
//
//	NONE

bool WebApplicationEnumerator::MoveNext(void)
{
	CHECK_DISPOSED(m_disposed);

	m_current++;								// Move to the next element
	return (m_current < m_hosts->Count);		// True/False based on position
}

//---------------------------------------------------------------------------
// WebApplicationEnumerator::Reset
//
// Resets the enumerator back to it's original state
//
// Arguments:
//
//	NONE

void WebApplicationEnumerator::Reset(void)
{
	CHECK_DISPOSED(m_disposed);
	m_current = -1;
}

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
