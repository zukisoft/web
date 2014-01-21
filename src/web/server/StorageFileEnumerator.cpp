//---------------------------------------------------------------------------
// StorageFileEnumerator Implementation
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
#include "StorageFileEnumerator.h"		// Include class declarations
#include "StorageFile.h"				// Include StorageFile declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// StorageFileEnumerator Constructor
//
// Arguments:
//
//	path			- Virtual directory of the referenced container
//	container		- Referenced structured storage container object

StorageFileEnumerator::StorageFileEnumerator(StoragePath^ path, 
	StorageContainer^ container) : m_path(path)
{
	m_enum = container->Objects->GetEnumerator();	// Grab enumerator
}

//---------------------------------------------------------------------------
// StorageFileEnumerator::Current::get
//
// Retrieves the object instance at the current enumerated position

VirtualFile^ StorageFileEnumerator::Current::get(void)
{
	StorageObject^			object;			// Object to be returned
	StoragePath^			path;			// New virtual file path

	CHECK_DISPOSED(m_disposed);
	
	object = m_enum->Current;
	path = gcnew StoragePath(m_path, m_path->Full + "/" + object->Name);
	return gcnew StorageFile(path, object);
}

//---------------------------------------------------------------------------
// StorageFileEnumerator::MoveNext
//
// Advances the enumerator to the next element
//
// Arguments:
//
//	NONE

bool StorageFileEnumerator::MoveNext(void)
{
	bool				result;			// Operational result

	CHECK_DISPOSED(m_disposed);
	
	result = m_enum->MoveNext();		// Move the real enumerator
	if(!result) m_spent = true;			// Set spent enumerator flag
	return result;						// Return result from MoveNext()
}

//---------------------------------------------------------------------------
// StorageFileEnumerator::Reset
//
// Resets the enumerator back to it's default state
//
// Arguments:
//
//	NONE

void StorageFileEnumerator::Reset(void)
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


