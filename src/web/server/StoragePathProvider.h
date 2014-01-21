//---------------------------------------------------------------------------
// StoragePathProvider Declarations
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

#ifndef __STORAGEPATHPROVIDER_H_
#define __STORAGEPATHPROVIDER_H_
#pragma once

#include "HashCodeCombiner.h"				// Include HashCodeCombiner decls
#include "ProjectUtil.h"					// Include ProjectUtil declarations
#include "StorageConnectionStringBuilder.h"	// Include StorageConnectionStringBuilder
#include "StorageDirectory.h"				// Include StorageDirectory decls
#include "StorageFile.h"					// Include StorageFile declarations
#include "StoragePath.h"					// Include StoragePath declarations
#include "WebApplicationConfiguration.h"	// Include WebApplicationConfiguration

#pragma warning(push, 4)					// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;
using namespace System::IO;
using namespace System::Security::Permissions;
using namespace System::Web;
using namespace System::Web::Caching;
using namespace System::Web::Hosting;
using namespace zuki::data::structured;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class StoragePathProvider (internal)
//
// StoragePathProvider is the custom VirtualPathProvider implementation to
// work with a Structured Storage file instead of the local file system.
//---------------------------------------------------------------------------

[AspNetHostingPermission(SecurityAction::Demand, Level=AspNetHostingPermissionLevel::Medium)]
[AspNetHostingPermission(SecurityAction::InheritanceDemand, Level=AspNetHostingPermissionLevel::High)]
ref class StoragePathProvider sealed : public VirtualPathProvider
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// DirectoryExists (VirtualPathProvider)
	//
	// Determines if the specified directory exists or not
	virtual bool DirectoryExists(String^ virtualDir) override;

	// FileExists (VirtualPathProvider)
	//
	// Determines if the specified file exists or not
	virtual bool FileExists(String^ virtualPath) override;

	// GetCacheDependency (VirtualPathProvider)
	//
	// Creates a cache dependency based on the specified virtual paths
	virtual CacheDependency^ GetCacheDependency(String^ virtualPath, 
		Collections::IEnumerable^ virtualPathDependencies, DateTime utcStart) override;

	// GetDirectory (VirtualPathProvider)
	//
	// Retrieves a specific directory from the virtual storage implementation
	virtual VirtualDirectory^ GetDirectory(String^ virtualDir) override;

	// GetFile (VirtualPathProvider)
	//
	// Retrieves a specific file from the virtual storage implementation
	virtual VirtualFile^ GetFile(String^ virtualPath) override;

	// GetFileHash (VirtualPathProvider)
	//
	// Returns a hash of the specified virtual paths
	virtual String^ GetFileHash(String^ virtualPath, Collections::IEnumerable^ dependencies) override;

internal:

	// INTERNAL CONSTRUCTOR
	StoragePathProvider(WebApplicationConfiguration^ config);

	//-----------------------------------------------------------------------
	// Internal Properties

	// StoreDateTime
	//
	// Exposes the date/time of the virtual file system store
	property DateTime StoreDateTime
	{
		DateTime get(void) { return m_storeDate; }
	}

private:

	//-----------------------------------------------------------------------
	// Private Type Declarations

	// FileIndex
	//
	// Type used to implement a File index from Structured Storage
	typedef Dictionary<String^, Guid> FileIndex;

	// TableOfContentsEntry
	//
	// Type used to implement a table of contents entry
	typedef KeyValuePair<StorageContainer^, FileIndex^> TableOfContentsEntry;

	// TableOfContents
	//
	// Type representing the table of contents
	typedef Dictionary<String^, TableOfContentsEntry> TableOfContents;

	//-----------------------------------------------------------------------
	// Private Member Functions

	// ConstructTableOfContents
	//
	// Constructs a TableOfContents for the structured storage file
	static void ConstructTableOfContents(StoragePath^ root, StructuredStorage^ storage,
		TableOfContents^% toc);

	// ProcessTocContainer
	//
	// Helper function to ConstructTableOfContents that processes a container
	static void ProcessTocContainer(TableOfContents^ toc, StoragePath^ root, 
		StorageContainer^ container);

	//-----------------------------------------------------------------------
	// Member Variables
	
	bool						m_disposed;			// Object disposal flag
	StructuredStorage^			m_storage;			// Root Storage object
	DateTime					m_storeDate;		// Storage file date/time
	StoragePath^				m_root;				// The root virtual path
	TableOfContents^			m_toc;				// Table of contents
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __STORAGEPATHPROVIDER_H_