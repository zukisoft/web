//---------------------------------------------------------------------------
// StorageFileEnumerator Declarations
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

#ifndef __STORAGEFILEENUMERATOR_H_
#define __STORAGEFILEENUMERATOR_H_
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

ref class StorageFile;					// StorageFile.h

//---------------------------------------------------------------------------
// Class StorageFileEnumerator (internal)
//
// StorageFileEnumerator generates an IEnumerator<> implementation that is
// built from the StorageObjects contained within a StorageContainer so that
// the web hosting environment can find out what's in there
//---------------------------------------------------------------------------

ref class StorageFileEnumerator sealed : public Generic::IEnumerator<VirtualFile^>
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
	virtual property VirtualFile^ Current 
	{ 
		VirtualFile^ get(void); 
	}
	
internal:

	// INTERNAL CONSTRUCTOR
	StorageFileEnumerator(StoragePath^ path, StorageContainer^ container);

	//-----------------------------------------------------------------------
	// Internal Properties

	// Spent
	//
	// Determines if this enumerator has been spent or not
	property bool Spent
	{
		bool get(void) { return m_spent; }
	}

private:

	// DESTRUCTOR
	~StorageFileEnumerator() { if(!m_disposed) delete m_enum; m_disposed = true; }

	//-----------------------------------------------------------------------
	// Private Properties

	// _Current (IEnumerator::Current)
	virtual property Object^ _Current 
	{
		Object^ get(void) sealed = Collections::IEnumerator::Current::get { return Current; }
	}

	//-----------------------------------------------------------------------
	// Member Variables

	bool						m_disposed;			// Object disposal flag
	StoragePath^				m_path;				// Base virtual directory
	bool						m_spent;			// Enumerator spent flag

	Generic::IEnumerator<StorageObject^>^ m_enum;	// Real enumerator
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __STORAGEFILEENUMERATOR_H_
