//---------------------------------------------------------------------------
// StructuredStorage Declarations
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

#ifndef __STRUCTUREDSTORAGE_H_
#define __STRUCTUREDSTORAGE_H_
#pragma once

#include "ComCache.h"					// Include ComCache declarations
#include "ComPropertyStorage.h"			// Include ComPropertyStorage decls
#include "ComStorage.h"					// Include ComStorage declarations
#include "ComStream.h"					// Include ComStream declarations
#include "StorageAccessMode.h"			// Include StorageAccessMode declarations
#include "StorageContainer.h"			// Include StorageContainer declarations
#include "StorageException.h"			// Include StorageException declarations
#include "StorageObject.h"				// Include StorageObject declarations
#include "StorageOpenMode.h"			// Include StorageOpenMode declarations
#include "StoragePropertySet.h"			// Include StoragePropertySet decls
#include "StorageSummaryInformation.h"	// Include StorageSummaryInfo decls

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::IO;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(data)
BEGIN_NAMESPACE(structured)

//---------------------------------------------------------------------------
// Class StructuredStorage
//
// StructuredStorage is the top-level object that allows the application to 
// work with a structured storage file
//---------------------------------------------------------------------------

STRUCTURED_STORAGE_PUBLIC ref class StructuredStorage sealed : public StorageContainer
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	void Close(void) { delete this; }
	void Flush(void);

	//-----------------------------------------------------------------------
	// Properties

	property StorageSummaryInformation^ SummaryInformation { StorageSummaryInformation^ get(void); }

	//-----------------------------------------------------------------------
	// Static Member Functions

	static StructuredStorage^ Create(String^ path)
		{ return Open(path, StorageOpenMode::Create, StorageAccessMode::Exclusive); }

	static StructuredStorage^ CreateTemp(void)
		{ return Open(nullptr, StorageOpenMode::Create, StorageAccessMode::Exclusive); }

	static StructuredStorage^ Open(String^ path)
		{ return Open(path, StorageOpenMode::Open, StorageAccessMode::Exclusive); }

	static StructuredStorage^ Open(String^ path, StorageOpenMode mode)
		{ return Open(path, mode, StorageAccessMode::Exclusive); }

	static StructuredStorage^ Open(String^ path, StorageOpenMode mode, StorageAccessMode access);

internal:

	//-----------------------------------------------------------------------
	// Internal Properties

	// ComPropStorageCache
	//
	// Exposes the contained ComPropertyStorage cache for this instance
	property ComCache<ComPropertyStorage^>^ ComPropStorageCache
	{
		ComCache<ComPropertyStorage^>^ get(void);
	}

	// ComStorageCache
	//
	// Exposes the contained ComStorage cache for this instance
	property ComCache<ComStorage^>^ ComStorageCache
	{
		ComCache<ComStorage^>^ get(void);
	}

	// ComStreamCache
	//
	// Exposes the contained ComStream cache for this instance
	property ComCache<ComStream^>^ ComStreamCache
	{
		ComCache<ComStream^>^ get(void);
	}

	property String^ FileName { String^ get(void); }

private:

	// PRIVATE CONSTRUCTOR
	StructuredStorage(String^ fileName, ComStorage^ storage);

	// DESTRUCTOR / FINALIZER
	~StructuredStorage();

	//-----------------------------------------------------------------------
	// Member Variables

	bool								m_disposed;			// Object disposal flag
	ComStorage^							m_storage;			// Root storage pointer
	String^								m_fileName;			// Open file name
	ComCache<ComPropertyStorage^>^		m_pstgCache;		// PropertyStorage cache
	ComCache<ComStorage^>^				m_stgCache;			// Storage cache
	ComCache<ComStream^>^				m_stmCache;			// Stream cache
	StorageSummaryInformation^			m_summaryInfo;		// SummaryInfo pointer
};

//---------------------------------------------------------------------------

END_NAMESPACE(structured)
END_NAMESPACE(data)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __STRUCTUREDSTORAGE_H_
