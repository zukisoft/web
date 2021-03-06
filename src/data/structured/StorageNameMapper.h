//---------------------------------------------------------------------------
// StorageNameMapper Declarations
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

#ifndef __STORAGENAMEMAPPER_H_
#define __STORAGENAMEMAPPER_H_
#pragma once

#include "StorageException.h"			// Include StorageException decls
#include "StorageExceptions.h"			// Include StorageExceptions decls
#include "StorageUtil.h"				// Include StorageUtil declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections::Generic;
using namespace msclr;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(data)
BEGIN_NAMESPACE(structured)

//---------------------------------------------------------------------------
// Custom property set FMTID codes
//---------------------------------------------------------------------------

extern const FMTID FMTID_ContainerNameMapper;		// StorageNameMapper.cpp
extern const FMTID FMTID_ObjectNameMapper;			// StorageNameMapper.cpp
extern const FMTID FMTID_PropertySetNameMapper;		// StorageNameMapper.cpp

//---------------------------------------------------------------------------
// Forward class declarations
//---------------------------------------------------------------------------

ref class ComStorage;							// ComStorage.h

//---------------------------------------------------------------------------
// Class StorageNameMapper (internal)
//
// StorageNameMapper implements a specialized property set used by this module
// to allow a NAME->GUID association.  This association is used to provide
// container and object names longer than 31 characters, and in the case of
// property sets, a name at all (they're usually defined by GUIDs).
//
// This is a replacement for the original "NameMapper" class.  It's now
// an instance-based class rather than completely static, is thread-safe,
// and works with the new ComXXXX smart pointers instead of raw pointers.
//---------------------------------------------------------------------------

ref class StorageNameMapper sealed
{
public:

	//-----------------------------------------------------------------------
	// Constructor

	StorageNameMapper(const FMTID& fmtid, ComStorage^ storage);

	//-----------------------------------------------------------------------
	// Member Functions

	// AddMapping
	//
	// Adds a new NAME->GUID mapping
	void AddMapping(String^ name, Guid guid);

	// ContainsGuid
	//
	// Determines if the specified GUID exists in the mapper
	bool ContainsGuid(Guid guid);

	// ContainsName
	//
	// Determines if the specified NAME exists in the mapper
	bool ContainsName(String^ name);

	// MapGuidToName
	//
	// Maps a GUID into it's NAME representation
	String^ MapGuidToName(Guid guid);

	// MapNameToGuid
	//
	// Maps a NAME into it's GUID representation
	Guid MapNameToGuid(String^ name);

	// RemoveMapping
	//
	// Removes a mapping from the collection
	void RemoveMapping(String^ name);
	void RemoveMapping(Guid guid);

	// RenameMapping
	//
	// Renames a mapping in the collection
	void RenameMapping(Guid guid, String^ newName);

	// ToDictionary
	//
	// Converts the entire collection into a Dictionary<String^, Guid> instance
	Dictionary<String^, Guid>^ ToDictionary(void);

	// TryMapGuidToName
	//
	// Tries to map a GUID to a NAME without throwing an exception
	bool TryMapGuidToName(Guid guid, String^% name);

	// TryMapNameToGuid
	//
	// Tries to map a NAME to a GUID without throwing an exception
	bool TryMapNameToGuid(String^ name, Guid% guid);

	//-----------------------------------------------------------------------
	// Properties

	// Count
	//
	// Gets the number of mappings in the collection
	property int Count
	{
		int get(void);
	}

	// SyncRoot
	//
	// Provides an object that can be used for synchronization
	property Object^ SyncRoot
	{
		Object^ get(void) { return this; }
	}
	
private:

	// DESTRUCTOR / FINALIZER
	~StorageNameMapper() { this->!StorageNameMapper(); m_disposed = true; }
	!StorageNameMapper();

	//-----------------------------------------------------------------------
	// Private Constants

	// NAMEMAPPER_BASEID
	//
	// Used as the base property ID code for sets defined by the mapper
	literal int NAMEMAPPER_BASEID = 255;

	//-----------------------------------------------------------------------
	// Private Member Functions

	// GetPropStorage
	//
	// Instantiates and returns the contained IPropertyStorage
	HRESULT GetPropertyStorage(IPropertyStorage** ppPropStorage);

	// MapGuidInternal
	//
	// Internal version of a GUID->NAME mapper
	bool MapGuidInternal(Guid guid, String^% name, PROPID *pPropid);

	//-----------------------------------------------------------------------
	// Member Variables

	initonly Guid				m_fmtid;			// Reference FMTID
	bool						m_disposed;			// Object disposal flag
	ComStorage^					m_storage;			// Referenced ComStorage
	IPropertyStorage*			m_pPropStorage;		// Contained IPropertyStorage
};

//---------------------------------------------------------------------------

END_NAMESPACE(structured)
END_NAMESPACE(data)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __STORAGENAMEMAPPER_H_
