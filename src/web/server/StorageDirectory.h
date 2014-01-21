//---------------------------------------------------------------------------
// StorageDirectory Declarations
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

#ifndef __STORAGEDIRECTORY_H_
#define __STORAGEDIRECTORY_H_
#pragma once

#include "StorageChildCollection.h"		// Include StorageChildCollection
#include "StorageDirectoryCollection.h"	// Include StorageDirectoryCollection
#include "StorageFileCollection.h"		// Include StorageFileCollection
#include "StoragePath.h"				// Include StoragePath declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Security::Permissions;
using namespace System::Web;
using namespace System::Web::Hosting;
using namespace zuki::data::structured;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class StorageDirectory (internal)
//
// StorageDirectory is used by the custom VirtualPathProvider implementation 
// to work with a Structured Storage file instead of the local file system.
//---------------------------------------------------------------------------

[AspNetHostingPermission(SecurityAction::Demand, Level=AspNetHostingPermissionLevel::Minimal)]
[AspNetHostingPermission(SecurityAction::InheritanceDemand, Level=AspNetHostingPermissionLevel::Minimal)]
ref class StorageDirectory sealed : public VirtualDirectory
{
public:

	//-----------------------------------------------------------------------
	// Properties

	// Children (VirtualDirectory)
	//
	// Gets a list of the files and subdirectories contained in this directory
	 virtual property Collections::IEnumerable^ Children
	{
		Collections::IEnumerable^ get(void) override 
		{ 
			return gcnew StorageChildCollection(m_path, m_container); 
		}
	}

	// Directories (VirtualDirectory)
	//
	// Gets a list of the subdirectories contained in this directory
	virtual property Collections::IEnumerable^ Directories
	{
		Collections::IEnumerable^ get(void) override 
		{ 
			return gcnew StorageDirectoryCollection(m_path, m_container); 
		}
	}

	// Files (VirtualDirectory)
	//
	// Gets a list of the files contained in this directory
	virtual property Collections::IEnumerable^ Files
	{
		Collections::IEnumerable^ get(void) override 
		{ 
			return gcnew StorageFileCollection(m_path, m_container); 
		}
	}

internal:

	// INTERNAL CONSTRUCTOR
	StorageDirectory(StoragePath^ path, StorageContainer^ container) : 
	  m_path(path), VirtualDirectory(path->Full), m_container(container) {}

private:

	//-----------------------------------------------------------------------
	// Member Variables

	StoragePath^			m_path;			// Storage virtual path info
	StorageContainer^		m_container;	// Contained container object
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __STORAGEDIRECTORY_H_