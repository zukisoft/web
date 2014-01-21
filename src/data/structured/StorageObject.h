//---------------------------------------------------------------------------
// StorageObject Declarations
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

#ifndef __STORAGEOBJECT_H_
#define __STORAGEOBJECT_H_
#pragma once

#include "ComStorage.h"					// Include ComStorage declarations
#include "ComStream.h"					// Include ComStream declarationss
#include "StorageException.h"			// Include StorageException declarations
#include "StorageExceptions.h"			// Include exception declarations
#include "StorageObjectStream.h"		// Include StorageObjectStream declarations
#include "StorageObjectReader.h"		// Include StorageObjectReader decls
#include "StorageObjectWriter.h"		// Include StorageObjectWriter decls

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::IO;
using namespace System::Runtime::Serialization::Formatters::Binary;

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
// Class StorageObject
//
// StorageObject implements an object stream (IStream)
//---------------------------------------------------------------------------

STRUCTURED_STORAGE_PUBLIC ref class StorageObject sealed
{
public:

	//-----------------------------------------------------------------------
	// Methods

	// NOTE: CopyTo and MoveTo disappeared when the COM pointers were
	// refactored.  Now the application has to do those things on it's own

	StorageObjectReader^	GetReader(void);
	StorageObjectWriter^	GetWriter(void);

	//-----------------------------------------------------------------------
	// Properties

	property array<Byte>^ Data
	{
		array<Byte>^ get(void);
		void set(array<Byte>^ value);
	}

	property String^ Name
	{
		String^ get(void);
		void set(String^ value);
	}

	property bool ReadOnly { bool get(void) { return m_readOnly; } }

internal:

	// INTERNAL CONSTRUCTOR
	StorageObject(StructuredStorage^ root, ComStorage^ parent, ComStream^ stream);

	//-----------------------------------------------------------------------
	// Internal Properties

	// ObjectID
	//
	// Exposes the object identifier GUID for this storage object
	property Guid ObjectID
	{
		Guid get(void) { return m_objid; }
	}

private:

	//-----------------------------------------------------------------------
	// Member Variables

	StructuredStorage^			m_root;				// Root storage object
	ComStorage^					m_parent;			// Parent ComStorage instance
	ComStream^					m_stream;			// Contained stream instance
	bool						m_readOnly;			// Read-Only flag
	Guid						m_objid;			// Object ID GUID
};

//---------------------------------------------------------------------------

END_NAMESPACE(structured)
END_NAMESPACE(data)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __STORAGEOBJECT_H_
