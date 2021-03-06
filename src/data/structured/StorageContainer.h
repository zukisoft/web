//---------------------------------------------------------------------------
// StorageContainer Declarations
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

#ifndef __STORAGECONTAINER_H_
#define __STORAGECONTAINER_H_
#pragma once

#include "ComStorage.h"						// Include ComStorage declarations
#include "StorageContainerCollection.h"		// Include StorageContainerCollection decls
#include "StorageException.h"				// Include StorageException declarations
#include "StorageExceptions.h"				// Include exception declarations
#include "StorageObjectCollection.h"		// Include StorageObjectCollection decls
#include "StoragePropertySetCollection.h"	// Include StoragePropertySetCollection

#pragma warning(push, 4)					// Enable maximum compiler warnings

using namespace System;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(data)
BEGIN_NAMESPACE(structured)

//---------------------------------------------------------------------------
// Forward Class Declarations
//
// Include the specified header files in the .CPP file for this class
//---------------------------------------------------------------------------

ref class StructuredStorage;				// <-- StructuredStorage.h

//---------------------------------------------------------------------------
// Class StorageContainer
//
// StorageContainer implements a container object (IStorage).  Every container
// can contain objects (Streams), property sets, and sub container objects.
//---------------------------------------------------------------------------

STRUCTURED_STORAGE_PUBLIC ref class StorageContainer
{
public:

	//-----------------------------------------------------------------------
	// Properties

	property String^ Name
	{
		String^ get(void);
		void set(String^ value);
	}

	property bool ReadOnly { bool get(void) { return m_readOnly; } }

	property StorageContainerCollection^ Containers { StorageContainerCollection^ get(void); }
	property StorageObjectCollection^ Objects { StorageObjectCollection^ get(void); }
	property StoragePropertySetCollection^ PropertySets { StoragePropertySetCollection^ get(void); }

internal:

	// INTERNAL CONSTRUCTOR
	StorageContainer(StructuredStorage^ root, ComStorage^ parent, ComStorage^ storage);

	//-----------------------------------------------------------------------
	// Internal Properties

	// ContainerID
	//
	// Exposes the object identifier GUID for this storage container
	property Guid ContainerID
	{
		Guid get(void) { return m_contid; }
	}

	// IsRoot
	//
	// Exposes if this container happens to be the root container
	property bool IsRoot
	{
		bool get(void) { return m_contid == Guid::Empty; }
	}

private:

	//-----------------------------------------------------------------------
	// Private Constants

	literal String^ CONTAINER_NAME_ROOT = gcnew String("StorageRoot");

	//-----------------------------------------------------------------------
	// Member Variables

	StructuredStorage^				m_root;				// Root storage object
	ComStorage^						m_parent;			// Parent storage instance
	ComStorage^						m_storage;			// This storage instance
	bool							m_readOnly;			// Read-Only flag
	Guid							m_contid;			// Container ID GUID
	StorageContainerCollection^		m_containers;		// Containers collection
	StorageObjectCollection^		m_objects;			// Objects collection
	StoragePropertySetCollection^	m_propsets;			// PropSets collection
};

//---------------------------------------------------------------------------

END_NAMESPACE(structured)
END_NAMESPACE(data)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __STORAGECONTAINER_H_
