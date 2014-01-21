//---------------------------------------------------------------------------
// StorageChildEnumerator Implementation
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
#include "StorageChildEnumerator.h"		// Include class declarations
#include "StorageDirectoryEnumerator.h"	// Include StorageDirectory declarations
#include "StorageFileEnumerator.h"		// Include StorageFile declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// StorageChildEnumerator Constructor
//
// Arguments:
//
//	path			- Virtual directory information for the container
//	container		- Referenced structured storage container object

StorageChildEnumerator::StorageChildEnumerator(StoragePath^ path, 
	StorageContainer^ container)
{
	m_enumDirs = gcnew StorageDirectoryEnumerator(path, container);
	m_enumFiles = gcnew StorageFileEnumerator(path, container);
}

//---------------------------------------------------------------------------
// StorageChildEnumerator Destructor

StorageChildEnumerator::~StorageChildEnumerator()
{
	if(m_disposed) return;			// Object has already been disposed of

	delete m_enumFiles;				// Dispose of the enumerator
	delete m_enumDirs;				// Dispose of the enumerator
	m_disposed = true;				// Object has now been disposed of
}

//---------------------------------------------------------------------------
// StorageChildEnumerator::Current::get
//
// Retrieves the object instance at the current enumerated position

VirtualFileBase^ StorageChildEnumerator::Current::get(void)
{
	CHECK_DISPOSED(m_disposed);

	// If the directories haven't been used up, return those first.  After
	// that come the files.  After that ... well .. nullptr looks good

	if(!m_enumDirs->Spent) return m_enumDirs->Current;
	else if(!m_enumFiles->Spent) return m_enumFiles->Current;

	return nullptr;				// Nothing left to enumerate
}

//---------------------------------------------------------------------------
// StorageChildEnumerator::MoveNext
//
// Advances the enumerator to the next element
//
// Arguments:
//
//	NONE

bool StorageChildEnumerator::MoveNext(void)
{
	CHECK_DISPOSED(m_disposed);

	// If the directory enumerator is still valid, attempt a MoveNext on it
	// first.  If that returns false, automatically switch over to the file
	// enumerator.  If both are spent or become spent, just return false

	if((!m_enumDirs->Spent) && (m_enumDirs->MoveNext())) return true;
	else if((!m_enumFiles->Spent) && (m_enumFiles->MoveNext())) return true;
	else return false;
}

//---------------------------------------------------------------------------
// StorageChildEnumerator::Reset
//
// Resets the enumerator back to it's default state
//
// Arguments:
//
//	NONE

void StorageChildEnumerator::Reset(void)
{
	CHECK_DISPOSED(m_disposed);

	m_enumDirs->Reset();			// Reset the directory enumerator
	m_enumFiles->Reset();			// Reset the file enumerator
}

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
