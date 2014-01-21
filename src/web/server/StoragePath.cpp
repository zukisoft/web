//---------------------------------------------------------------------------
// StoragePath Implementation
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
#include "StoragePath.h"				// Include StoragePath declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// StoragePath Constructor
//
// Constructs a virtual path instance based on a root virtual path
//
// Arguments:
//
//	root			- The root virtual path instance
//	virtualPath		- The complete virtual path to break up

StoragePath::StoragePath(StoragePath^ root, String^ virtualPath)
{
	if(root == nullptr) throw gcnew ArgumentNullException();
	if(virtualPath == nullptr) throw gcnew ArgumentNullException();

	virtualPath = virtualPath->TrimEnd(s_splitPaths);	// Remove trailing slashes

	// ROOT = "/rootpath/"
	m_root = root->Root;

	// FULL = "/rootpath/path"
	m_full = (virtualPath->StartsWith("/")) ? virtualPath : "/" + virtualPath;

	// The full virtual path has to start with "/rootpath" or it's not valid

	if(!m_full->StartsWith(root->Full, StringComparison::OrdinalIgnoreCase)) throw gcnew ArgumentException();

	// RELATIVE = "/path"
	m_relative = m_full->Substring(m_root->Length - 1);
}

//---------------------------------------------------------------------------
// StoragePath Constructor (private)
//
// Constructs a root virtual path object instance
//
// Arguments:
//
//	rootPath		- The root virtual path

StoragePath::StoragePath(String^ rootPath)
{
	// FULL AND RELATIVE = "/rootpath"
	m_full = m_relative = (rootPath->StartsWith("/")) ? rootPath : "/" + rootPath;

	// ROOT = "/rootpath/"
	m_root = (m_full->EndsWith("/")) ? m_full : m_full + "/";
}

//---------------------------------------------------------------------------
// StoragePath::Append
//
// Generates a new StoragePath by appending a relative path to this one
//
// Arguments:
//
//	relative	- The new relative path to be appended

StoragePath^ StoragePath::Append(String^ relative)
{
	if(relative == nullptr) throw gcnew ArgumentNullException();

	relative = relative->StartsWith("/") ? relative : "/" + relative;
	return gcnew StoragePath(this, (m_full->Length > 1) ? m_full + relative : relative);
}

//---------------------------------------------------------------------------
// StoragePath::Directory::get
//
// Extracts the directory portion of a file virtual path

String^ StoragePath::Directory::get(void)
{
	int				length;					// Overall full path length
	int				current;				// Current location in the path

	length = m_full->Length;				// Start at the end of the string
	current = length - 1;					// Start at the end of the string

	// Loop backwards over the full virtual path, and stop if/when we find
	// a slash character.  Everything before that is the directory name

	while(current >= 1) {			// <-- 1: All paths must start with a /

		if(m_full[current] == '/') return m_full->Substring(0, current);
		current--;
	}

	// If no separator was found, return the root string without the trailing
	// slash (except in the case of the true root "/", of course)

	return (m_root->Length > 1) ? m_root->Substring(0, m_root->Length - 1) : m_root;
}

//---------------------------------------------------------------------------
// StoragePath::FileName::get
//
// Extracts the file name portion of a file virtual path

String^ StoragePath::FileName::get(void)
{
	int				length;					// Overall full path length
	int				current;				// Current location in the path

	length = m_full->Length;				// Start at the end of the string
	current = length - 1;					// Start at the end of the string

	// Loop backwards over the full virtual path, and stop if/when we find
	// a slash character.  Everything after that is the file name

	while(current >= 0) {

		if(m_full[current] == '/') return m_full->Substring(current + 1, (length - current) - 1);
		current--;
	}

	return m_full;			// No separators present .. return full string
}

//---------------------------------------------------------------------------
// StoragePath::MakeRoot (static)
//
// Generates a root StoragePath object instance
//
// Arguments:
//
//	rootPath	- The root virtual path

StoragePath^ StoragePath::MakeRoot(String^ rootPath)
{
	if(rootPath == nullptr) throw gcnew ArgumentNullException();
	return gcnew StoragePath(rootPath);
}

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
