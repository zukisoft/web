//---------------------------------------------------------------------------
// StorageDirectoryEnumerator Implementation
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
#include "StorageDirectoryEnumerator.h"	// Include class declarations
#include "StorageDirectory.h"			// Include StorageDirectory declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// StorageDirectoryEnumerator Constructor
//
// Arguments:
//
//	path			- Virtual directory of the referenced container
//	container		- Referenced structured storage container object

StorageDirectoryEnumerator::StorageDirectoryEnumerator(StoragePath^ path, 
	StorageContainer^ container) : m_path(path)
{
	m_enum = container->Containers->GetEnumerator();	// Grab enumerator
}

//---------------------------------------------------------------------------
// StorageDirectoryEnumerator::Current::get
//
// Retrieves the object instance at the current enumerated position

VirtualDirectory^ StorageDirectoryEnumerator::Current::get(void)
{
	StorageContainer^		container;			// Object to be returned
	StoragePath^			path;				// New StoragePath instance

	CHECK_DISPOSED(m_disposed);
	
	container = m_enum->Current;
	path = gcnew StoragePath(m_path, m_path->Full + "/" + container->Name);
	return gcnew StorageDirectory(path, container);
}

//---------------------------------------------------------------------------
// StorageDirectoryEnumerator::MoveNext
//
// Advances the enumerator to the next element
//
// Arguments:
//
//	NONE

bool StorageDirectoryEnumerator::MoveNext(void)
{
	bool				result;			// Operational result

	CHECK_DISPOSED(m_disposed);
	
	result = m_enum->MoveNext();		// Move the real enumerator
	if(!result) m_spent = true;			// Set spent enumerator flag
	return result;						// Return result from MoveNext()
}

//---------------------------------------------------------------------------
// StorageDirectoryEnumerator::Reset
//
// Resets the enumerator back to it's default state
//
// Arguments:
//
//	NONE

void StorageDirectoryEnumerator::Reset(void)
{
	CHECK_DISPOSED(m_disposed);
	
	m_enum->Reset();					// Reset the real enumerator
	m_spent = false;					// Enumerator is no longer spent
}

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
