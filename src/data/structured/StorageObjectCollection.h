//---------------------------------------------------------------------------
// StorageObjectCollection Declarations
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

#ifndef __STORAGEOBJECTCOLLECTION_H_
#define __STORAGEOBJECTCOLLECTION_H_
#pragma once

#include "ComStorage.h"						// Include ComStorage declarations
#include "ComStream.h"						// Include ComStream declarationss
#include "StorageExceptions.h"				// Include exception declarations
#include "StorageObjectEnumerator.h"		// Include enumerator declarations

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
ref class StorageObject;					// <-- StorageObject.h

//---------------------------------------------------------------------------
// Class StorageObjectCollection
//
// StorageObjectCollection implements a collection of StorageObjects
// for a parent StorageContainer object.
//---------------------------------------------------------------------------

STRUCTURED_STORAGE_PUBLIC ref class StorageObjectCollection sealed : 
	public Generic::ICollection<StorageObject^>
{
public:

	//-----------------------------------------------------------------------
	// IEnumerable<T> implementation

	virtual Generic::IEnumerator<StorageObject^>^ GetEnumerator(void);

	//-----------------------------------------------------------------------
	// ICollection<T> implementation

	virtual StorageObject^	Add(String^ name);
	virtual void			Clear(void);
	virtual bool			Contains(String^ name);
	virtual void			CopyTo(array<StorageObject^>^ target, int index);
	virtual bool			Remove(String^ name);
	virtual bool			Remove(StorageObject^) = Generic::ICollection<StorageObject^>::Remove;

	virtual property int  Count { int get(void); }
	virtual property bool IsReadOnly { bool get(void) { return m_readOnly; } }

	//-----------------------------------------------------------------------
	// Properties

	property StorageObject^ default[String^] { StorageObject^ get(String^ name); }
	property StorageObject^ default[int] { StorageObject^ get(int index); }

internal:

	// INTERNAL CONSTRUCTOR
	StorageObjectCollection(StructuredStorage^ root, ComStorage^ storage);

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
	property StorageObject^ default[Guid]
	{
		StorageObject^ get(Guid objid);
	}

private:

	//-----------------------------------------------------------------------
	// Private Member Functions

	String^	LookupIndex(int index);

	// ICollection<T>::Add
	virtual void Add(StorageObject^) sealed =
		Generic::ICollection<StorageObject^>::Add { throw gcnew NotImplementedException(); }

	// ICollection<T>::Contains
	virtual bool Contains(StorageObject^ item) sealed = 
		Generic::ICollection<StorageObject^>::Contains;

	// IEnumerator::GetEnumerator
	virtual Collections::IEnumerator^ _GetEnumerator(void) sealed = 
		Collections::IEnumerable::GetEnumerator { return GetEnumerator(); }

	//-----------------------------------------------------------------------
	// Member Variables

	bool					m_disposed;			// Object disposal flag
	StructuredStorage^		m_root;				// Root Storage object
	ComStorage^				m_storage;			// Contained storage reference
	bool					m_readOnly;			// Read-only collection?
};

//---------------------------------------------------------------------------

END_NAMESPACE(structured)
END_NAMESPACE(data)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __STORAGEOBJECTCOLLECTION_H_
