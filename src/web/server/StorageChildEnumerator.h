//---------------------------------------------------------------------------
// StorageChildEnumerator Declarations
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

#ifndef __STORAGECHILDENUMERATOR_H_
#define __STORAGECHILDENUMERATOR_H_
#pragma once

#include "StoragePath.h"				// Include StoragePath declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Web::Hosting;
using namespace zuki::data::structured;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Forward Class Declarations
//---------------------------------------------------------------------------

ref class StorageDirectoryEnumerator;		// StorageDirectoryEnumerator.h
ref class StorageFileEnumerator;			// StorageFileEnumerator.h

//---------------------------------------------------------------------------
// Class StorageChildEnumerator (internal)
//
// StorageChildEnumerator generates an IEnumerator<> implementation that is
// built from the StorageObjects contained within a StorageContainer so that
// the web hosting environment can find out what's in there
//---------------------------------------------------------------------------

ref class StorageChildEnumerator sealed : public Generic::IEnumerator<VirtualFileBase^>
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
	// Returns the object instance at the current enumerated position
	virtual property VirtualFileBase^ Current 
	{ 
		VirtualFileBase^ get(void); 
	}
	
internal:

	// INTERNAL CONSTRUCTOR
	StorageChildEnumerator(StoragePath^ path, StorageContainer^ container);

private:

	// DESTRUCTOR
	~StorageChildEnumerator();

	//-----------------------------------------------------------------------
	// Private Properties

	// _Current (IEnumerator::Current)
	virtual property Object^ _Current 
	{
		Object^ get(void) sealed = Collections::IEnumerator::Current::get { return Current; }
	}

	//-----------------------------------------------------------------------
	// Member Variables

	bool							m_disposed;		// Object disposal flag
	StorageDirectoryEnumerator^		m_enumDirs;		// Directory enumerator
	StorageFileEnumerator^			m_enumFiles;	// File enumerator
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __STORAGECHILDENUMERATOR_H_
