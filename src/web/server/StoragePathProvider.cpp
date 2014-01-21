//---------------------------------------------------------------------------
// StoragePathProvider Implementation
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

#include "stdafx.h"						// Include project pre-compiled headers
#include "StoragePathProvider.h"		// Include StoragePathProvider declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// StoragePathProvider Constructor (internal)
//
// Arguments:
//
//	config			- Main web application configuration instance

StoragePathProvider::StoragePathProvider(WebApplicationConfiguration^ config)
{
	StorageConnectionStringBuilder^	csb;		// Storage connection string
	String^							filename;	// Storage connection string

	if(config == nullptr) throw gcnew ArgumentNullException();
	if(config->VirtualFileSystem->Type != VirtualFileSystemType::StructuredStorage) 
		throw gcnew InvalidOperationException();

	// Use the StorageConnectionStringBuilder to parse/access the components

	csb = gcnew StorageConnectionStringBuilder(config->VirtualFileSystem->Store);

	// Extract the structured storage file name from the "Data Source" key

	filename = csb->DataSource;
	if(String::IsNullOrEmpty(filename)) throw gcnew InvalidOperationException();

	// Expand any embedded environment variables and convert to a full path

	filename = Environment::ExpandEnvironmentVariables(filename);
	filename = Path::GetFullPath(filename);

	// Attempt to open the root storage object, which will throw an appropriate
	// exception back to the Host if it cannot be accessed for some reason

	(gcnew FileIOPermission(FileIOPermissionAccess::Read, filename))->Demand();
	m_storage = StructuredStorage::Open(filename, StorageOpenMode::Open, StorageAccessMode::ReadOnlyShared);
	m_storeDate = File::GetLastWriteTimeUtc(filename);

	m_root = StoragePath::MakeRoot(config->VirtualRoot);	// Generate Vroot
	ConstructTableOfContents(m_root, m_storage, m_toc);		// Generate TOC
}

//---------------------------------------------------------------------------
// StoragePathProvider::CreateTableOfContents (private, static)
//
// Constructs a TableOfContents for the specified virtual root and structured
// storage instance
//
// Arguments:
//
//	storage			- StructuredStorage to build the cache for
//	root			- Rooted StoragePath for the application
//	toc				- On success, contains the new TableOfContents

void StoragePathProvider::ConstructTableOfContents(StoragePath^ root, StructuredStorage^ storage,
	TableOfContents^% toc)
{
	toc = gcnew TableOfContents(StringComparer::OrdinalIgnoreCase);
	ProcessTocContainer(toc, root, storage);

#ifdef _DEBUG

	// In DEBUG builds, it's nice to be able to see that this worked, so
	// dump the entire table of contents to the system debugger ....

	for each(KeyValuePair<String^, TableOfContentsEntry> dirEntry in toc) {

		Debug::WriteLine(dirEntry.Key + " [" + dirEntry.Value.Key->ContainerID.ToString() + "]");

		for each(KeyValuePair<String^, Guid> fileEntry in dirEntry.Value.Value)
			Debug::WriteLine("    " + fileEntry.Key + "[" + fileEntry.Value.ToString() + "]");
	}

#endif	// _DEBUG
}

//---------------------------------------------------------------------------
// StoragePathProvider::DirectoryExists
//
// Determines if the specified directory exists or not.
//
// Arguments:
//
//	virtualDir		- Name of the virtual directory to locate

bool StoragePathProvider::DirectoryExists(String^ virtualDir)
{
	StoragePath^			path;			// VirtualPath instance

	Debug::Assert(m_storage != nullptr);

	// Use the table of contents to determine if this directory exists in
	// structured storage, don't go into the storage itself -- too slow

	path = gcnew StoragePath(m_root, virtualDir);
	return m_toc->ContainsKey(path->Full);
}

//---------------------------------------------------------------------------
// StoragePathProvider::FileExists
//
// Determines if the specified file exists or not.
//
// Arguments:
//
//	virtualPath		- Full path of the virtual file to locate

bool StoragePathProvider::FileExists(String^ virtualPath)
{
	StoragePath^			path;			// VirtualPath instance
	TableOfContentsEntry	entry;			// TOC directory entry

	Debug::Assert(m_storage != nullptr);

	// Use the table of contents to determine if this file exists in
	// structured storage, don't go into the storage itself -- too slow

	path = gcnew StoragePath(m_root, virtualPath);

	if(!m_toc->TryGetValue(path->Directory, entry)) return false;
	return entry.Value->ContainsKey(path->FileName);
}

//---------------------------------------------------------------------------
// StoragePathProvider::GetCacheDependency
//
// Creates a cache dependency based on the specified virtual paths
//
// Arguments:
//
//	virtualPath				- The path to the primary virtual resource
//	virtualPathDependencies	- An array of paths to other resources required
//	utcStart				- The UTC time at which the virtual resources were read

CacheDependency^ StoragePathProvider::GetCacheDependency(String^, Collections::IEnumerable^, DateTime)
{
	// We don't support cache dependencies since the storage files cannot
	// be changed once the structured storage has been opened by the provider

	return nullptr;
}

//---------------------------------------------------------------------------
// StoragePathProvider::GetDirectory
//
// Retrieves a specific directory object from the virtual storage
//
// Arguments:
//
//	virtualDir		- Virtual directory path

VirtualDirectory^ StoragePathProvider::GetDirectory(String^ virtualDir)
{
	StoragePath^				path;		// VirtualPath instance
	TableOfContentsEntry		entry;		// Table of contents entry

	Debug::Assert(m_storage != nullptr);

	// Handle ~/ properly by replacing it with the vritual root instead

	if(virtualDir->StartsWith("~/")) virtualDir = virtualDir->Replace("~/", m_root->Root);

	// Use the table of contents to get directly to the StorageContainer
	// instance we're looking for here if it even exists ...

	path = gcnew StoragePath(m_root, virtualDir);

	if(!m_toc->TryGetValue(path->Full, entry)) return nullptr;
	return gcnew StorageDirectory(path, entry.Key);
}

//---------------------------------------------------------------------------
// StoragePathProvider::GetFile
//
// Retrieves a specific file object from the virtual storage
//
// Arguments:
//
//	virtualPath		- Virtual file path

VirtualFile^ StoragePathProvider::GetFile(String^ virtualPath)
{
	StoragePath^				path;			// VirtualPath instance
	TableOfContentsEntry		entry;			// Table of contents entry
	Guid						objid;			// StorageObject ID GUID

	Debug::Assert(m_storage != nullptr);

	// Handle ~/ properly by replacing it with the vritual root instead

	if(virtualPath->StartsWith("~/")) virtualPath = virtualPath->Replace("~/", m_root->Root);

	// Use the table of contents to quickly determine if the file exists
	// and gain easy GUID-based access to the StorageObject instance

	path = gcnew StoragePath(m_root, virtualPath);

	if(!m_toc->TryGetValue(path->Directory, entry)) return nullptr;
	if(!entry.Value->TryGetValue(path->FileName, objid)) return nullptr;

	return gcnew StorageFile(path, entry.Key->Objects[objid]);
}

//---------------------------------------------------------------------------
// StoragePathProvider::GetFileHash
//
// Returns a hash of the specified virtual paths
//
// Arguments:
//
//	virtualPath		- The path to the primary virtual resource
//	dependencies	- An array of paths to other virtual resources required

String^ StoragePathProvider::GetFileHash(String^ virtualPath, Collections::IEnumerable^ dependencies)
{
	HashCodeCombiner^			combiner;		// Hash code combiner
	StoragePath^				path;			// StoragePath instance
	TableOfContentsEntry		entry;			// Table of contents entry
	Guid						objid;			// StorageObject ID GUID

	combiner = gcnew HashCodeCombiner();			// Instantiate the combiner
	combiner->AddString(virtualPath->ToLower());	// Start with the virtual path

	// Iterate over each of the dependencies and add their GUID into the hash.
	// It appears that the main file will always be in the dependency list, so
	// we don't need to hash the file represented by virtualPath on it's own

	for each(String^ dependency in dependencies) {

		path = gcnew StoragePath(m_root, dependency);
		if(m_toc->TryGetValue(path->Directory, entry)) {

			if(entry.Value->TryGetValue(path->FileName, objid)) combiner->AddGuid(objid);
		}
	}

	return combiner->ToString();		// Return the generated hash code
}

//---------------------------------------------------------------------------
// StoragePathProvider::ProcessTocContainer
//
// Helper function to ConstructTableOfContents that processes a container
//
// Arguments:
//
//	toc			- TableOfContents instance
//	path		- Current virtual path to work with
//	container	- Current StorageContainer to work with

void StoragePathProvider::ProcessTocContainer(TableOfContents^ toc, StoragePath^ path, 
	StorageContainer^ container)
{
	StoragePath^				newpath;			// The new container path

	newpath = path->Append((container->IsRoot) ? String::Empty : container->Name);
	toc->Add(newpath->Full, TableOfContentsEntry(container, container->Objects->ToDictionary()));
	for each(StorageContainer^ cont in container->Containers) ProcessTocContainer(toc, newpath, cont);
}

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)


