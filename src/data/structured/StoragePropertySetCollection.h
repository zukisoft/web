//---------------------------------------------------------------------------
// StoragePropertySetCollection Declarations
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

#ifndef __STORAGEPROPERTYSETCOLLECTION_H_
#define __STORAGEPROPERTYSETCOLLECTION_H_
#pragma once

#include "ComPropertyStorage.h"				// Include ComPropertyStorage decls
#include "ComStorage.h"						// Include ComStorage declarations
#include "StorageExceptions.h"				// Include exception declarations
#include "StoragePropertySetEnumerator.h"	// Include enumerator declarations

#pragma warning(push, 4)					// Enable maximum compiler warnings
#pragma warning(disable:4461)				// "finalizer without destructor"

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
// Class StoragePropertySetCollection
//
// StoragePropertySetCollection implements a collection of StoragePropertySets
// for a parent StorageContainer object.
//---------------------------------------------------------------------------

STRUCTURED_STORAGE_PUBLIC ref class StoragePropertySetCollection sealed : 
	public Generic::ICollection<StoragePropertySet^>
{
public:

	//-----------------------------------------------------------------------
	// IEnumerable<T> implementation

	virtual Generic::IEnumerator<StoragePropertySet^>^ GetEnumerator(void);

	//-----------------------------------------------------------------------
	// ICollection<T> implementation

	virtual StoragePropertySet^	Add(String^ name);
	virtual void				Clear(void);
	virtual bool				Contains(String^ name);
	virtual void				CopyTo(array<StoragePropertySet^>^ target, int index);
	virtual bool				Remove(String^ name);
	virtual bool				Remove(StoragePropertySet^) = Generic::ICollection<StoragePropertySet^>::Remove;

	virtual property int  Count { int get(void); }
	virtual property bool IsReadOnly { bool get(void) { return m_readOnly; } }

	//-----------------------------------------------------------------------
	// Properties

	property StoragePropertySet^ default[String^] { StoragePropertySet^ get(String^); }
	property StoragePropertySet^ default[int] { StoragePropertySet^ get(int index); }

internal:

	// INTERNAL CONSTRUCTOR
	StoragePropertySetCollection(StructuredStorage^ root, ComStorage^ storage);

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
	property StoragePropertySet^ default[Guid]
	{
		StoragePropertySet^ get(Guid propsetid);
	}

private:

	//-----------------------------------------------------------------------
	// Private Member Functions

	String^	LookupIndex(int index);

	// ICollection<T>::Add
	virtual void Add(StoragePropertySet^) sealed =
		Generic::ICollection<StoragePropertySet^>::Add { throw gcnew NotImplementedException(); }

	// ICollection<T>::Contains
	virtual bool Contains(StoragePropertySet^ item) sealed = 
		Generic::ICollection<StoragePropertySet^>::Contains;

	// IEnumerator::GetEnumerator
	virtual Collections::IEnumerator^ _GetEnumerator(void) sealed = 
		Collections::IEnumerable::GetEnumerator { return GetEnumerator(); }

	//-----------------------------------------------------------------------
	// Member Variables

	StructuredStorage^		m_root;				// Root Storage object
	ComStorage^				m_storage;			// Contained ComStorage instance
	bool					m_readOnly;			// Read-only collection?
};

//---------------------------------------------------------------------------

END_NAMESPACE(structured)
END_NAMESPACE(data)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __STORAGEPROPERTYSETCOLLECTION_H_
