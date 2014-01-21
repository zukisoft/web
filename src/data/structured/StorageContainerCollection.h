//---------------------------------------------------------------------------
// StorageContainerCollection Declarations
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

#ifndef __STORAGECONTAINERCOLLECTION_H_
#define __STORAGECONTAINERCOLLECTION_H_
#pragma once

#include "ComStorage.h"						// Include ComStorage declarations
#include "StorageContainerEnumerator.h"		// Include enumerator declarations
#include "StorageExceptions.h"				// Include exception declarations

#pragma warning(push, 4)					// Enable maximum compiler warnings
#pragma warning(disable:4461)				// "finalizer without destructor"

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(data)
BEGIN_NAMESPACE(structured)

//---------------------------------------------------------------------------
// Forward Class Declarations
//
// Include the specified header files in the .CPP file for this class
//---------------------------------------------------------------------------

ref class StructuredStorage;				// <-- StructuredStorage.h
ref class StorageContainer;					// <-- StorageContainer.h

//---------------------------------------------------------------------------
// Class StorageContainerCollection
//
// StorageContainerCollection implements a collection of StorageContainers
// for a parent StorageContainer object.
//---------------------------------------------------------------------------

STRUCTURED_STORAGE_PUBLIC ref class StorageContainerCollection sealed : 
	public Generic::ICollection<StorageContainer^>
{
public:

	//-----------------------------------------------------------------------
	// IEnumerable<T> implementation

	virtual Generic::IEnumerator<StorageContainer^>^ GetEnumerator(void);

	//-----------------------------------------------------------------------
	// ICollection<T> implementation

	virtual StorageContainer^	Add(String^ name);
	virtual void				Clear(void);
	virtual bool				Contains(String^ name);
	virtual void				CopyTo(array<StorageContainer^>^ target, int index);
	virtual bool				Remove(String^ name);
	virtual bool				Remove(StorageContainer^) = Generic::ICollection<StorageContainer^>::Remove;

	virtual property int  Count { int get(void); }
	virtual property bool IsReadOnly { bool get(void) { return m_readOnly; } }

	//-----------------------------------------------------------------------
	// Properties

	property StorageContainer^ default[String^] { StorageContainer^ get(String^ name); }
	property StorageContainer^ default[int] { StorageContainer^ get(int index); }

internal:

	// INTERNAL CONSTRUCTOR
	StorageContainerCollection(StructuredStorage^ root, ComStorage^ storage);

	//-----------------------------------------------------------------------
	// Internal Member Functions
	
	// ToDictionary
	//
	// Creates a snapshot dictionary of NAME->GUIDs for this collection
	Dictionary<String^, Guid>^ ToDictionary(void);

	//-----------------------------------------------------------------------
	// Internal Properties

	// default[Guid]
	//
	// Accesses an object in the collection by it's GUID
	property StorageContainer^ default[Guid]
	{
		StorageContainer^ get(Guid contid);
	}

private:

	//-----------------------------------------------------------------------
	// Private Member Functions

	String^	LookupIndex(int index);

	// ICollection<T>::Add
	virtual void Add(StorageContainer^) sealed =
		Generic::ICollection<StorageContainer^>::Add { throw gcnew NotImplementedException(); }

	// ICollection<T>::Contains
	virtual bool Contains(StorageContainer^ item) sealed = 
		Generic::ICollection<StorageContainer^>::Contains;

	// IEnumerator::GetEnumerator
	virtual Collections::IEnumerator^ _GetEnumerator(void) sealed = 
		Collections::IEnumerable::GetEnumerator { return GetEnumerator(); }

	//-----------------------------------------------------------------------
	// Member Variables

	StructuredStorage^		m_root;				// Root Storage object
	ComStorage^				m_storage;			// Contained storage instance
	bool					m_readOnly;			// Read-only collection?
};

//---------------------------------------------------------------------------

END_NAMESPACE(structured)
END_NAMESPACE(data)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __STORAGECONTAINERCOLLECTION_H_
