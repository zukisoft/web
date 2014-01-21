//---------------------------------------------------------------------------
// StoragePropertySetEnumerator Declarations
//
// ZukiSoft Structured Storage
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

#ifndef __STORAGEPROPERTYSETENUMERATOR_H_
#define __STORAGEPROPERTYSETENUMERATOR_H_
#pragma once

#include "ComStorage.h"					// Include ComStorage declarations
#include "StorageException.h"			// Include StorageException declarations
#include "StorageExceptions.h"			// Include exception declarations
#include "StorageUtil.h"				// Include StorageUtil declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace msclr;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(data)
BEGIN_NAMESPACE(structured)

//---------------------------------------------------------------------------
// Forward Class Declarations
//
// Include the specified header files in the .CPP file for this class
//---------------------------------------------------------------------------

ref class StructuredStorage;				// <-- StructuredStorage.h
ref class StoragePropertySet;				// <-- StoragePropertySet.h

//---------------------------------------------------------------------------
// Class StoragePropertySetEnumerator
//
// StoragePropertySetEnumerator generates an IEnumerator<> implementation.
// The enumeration is based on a 'snapshot' of the current set of propsets
// that are contained within the parent container rather than a cached off
// collection. This allows for much more advanced techniques than were 
// possible with the old collection implementation.
//---------------------------------------------------------------------------

STRUCTURED_STORAGE_PUBLIC ref class StoragePropertySetEnumerator sealed : 
	public Generic::IEnumerator<StoragePropertySet^>
{
public:

	//-----------------------------------------------------------------------
	// IEnumerator<T> Implementation

	virtual property StoragePropertySet^ Current { StoragePropertySet^ get(void); }
	
	virtual bool MoveNext(void);
	virtual void Reset(void);

internal:

	// INTERNAL CONSTRUCTOR
	StoragePropertySetEnumerator(StructuredStorage^ root, ComStorage^ storage);

private:

	// DESTRUCTOR
	~StoragePropertySetEnumerator() { m_disposed = true; }

	//-----------------------------------------------------------------------
	// Private Member Functions

	virtual property Object^ _Current {
		Object^ get(void) sealed = Collections::IEnumerator::Current::get { return Current; }
	}

	//-----------------------------------------------------------------------
	// Member Variables

	bool						m_disposed;			// Object disposal status
	StructuredStorage^			m_root;				// Root Storage object
	ComStorage^					m_storage;			// Contained ComStorage
	int							m_current;			// Current item index
	array<Guid>^				m_items;			// Enumerated object guids
};

//---------------------------------------------------------------------------

END_NAMESPACE(structured)
END_NAMESPACE(data)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __STORAGEPROPERTYSETENUMERATOR_H_
