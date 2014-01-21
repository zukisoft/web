//---------------------------------------------------------------------------
// StoragePath Declarations
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

#ifndef __STORAGEPATH_H_
#define __STORAGEPATH_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class StoragePath (internal)
//
// StoragePath implements a simple virtual path wrapper that is passed
// around the object hierarchy of the storage path provider so it can
// access both the full virtual paths as well as the relative storage path
//---------------------------------------------------------------------------

ref class StoragePath
{
public:

	//-----------------------------------------------------------------------
	// Constructor
	//
	// Arguments:
	//
	//	root			- Root path information
	//	virtualPath		- The virtual path to be parsed/contained

	StoragePath(StoragePath^ root, String^ virtualPath);

	//-----------------------------------------------------------------------
	// Member Functions

	// Append
	//
	// Generates a new StoragePath by appending a relative path
	StoragePath^ Append(String^ relative);

	// MakeRoot
	//
	// Generates a root virtual path instance
	static StoragePath^ MakeRoot(String^ rootPath);

	//-----------------------------------------------------------------------
	// Properties

	// Directory
	//
	// Gets the directory name in a file virtual path
	property String^ Directory
	{
		String^ get(void);
	}

	// FileName
	//
	// Gets the file name from a file virtual path
	property String^ FileName
	{
		String^ get(void);
	}

	// Full
	//
	// Gets the full and complete virtual path
	property String^ Full
	{
		String^ get(void) { return m_full; }
	}

	// Relative
	//
	// Gets the relative virtual path off of the root path
	property String^ Relative
	{
		String^ get(void) { return m_relative; }
	}

	// Root
	//
	// Gets the root path string
	property String^ Root
	{
		String^ get(void) { return m_root; }
	}

private:

	// PRIVATE CONSTRUCTOR
	StoragePath(String^ rootPath);

	//-----------------------------------------------------------------------
	// Member Variables

	String^					m_root;				// The root virtual path
	String^					m_full;				// The full virtual path
	String^					m_relative;			// The relative virtual path

	initonly static array<__wchar_t>^ s_splitPaths = { '/' };
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __STORAGEPATH_H_