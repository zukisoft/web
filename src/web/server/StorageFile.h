//---------------------------------------------------------------------------
// StorageFile Declarations
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

#ifndef __STORAGEFILE_H_
#define __STORAGEFILE_H_
#pragma once

#include "StoragePath.h"				// Include StoragePath declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Diagnostics;
using namespace System::IO;
using namespace System::Security::Permissions;
using namespace System::Web;
using namespace System::Web::Hosting;
using namespace zuki::data::structured;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class StorageFile (internal)
//
// StorageFile is used by the custom VirtualPathProvider implementation 
// to work with a Structured Storage file instead of the local file system.
//---------------------------------------------------------------------------

[AspNetHostingPermission(SecurityAction::Demand, Level=AspNetHostingPermissionLevel::Minimal)]
[AspNetHostingPermission(SecurityAction::InheritanceDemand, Level=AspNetHostingPermissionLevel::Minimal)]
ref class StorageFile sealed : public VirtualFile
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// GetHashCode (Object)
	//
	// Returns the hash code for this instance
	virtual int GetHashCode(void) override
	{
		// Use the GUID's hash code as the hash for this instance
		return m_object->ObjectID.GetHashCode();
	}

	// Open (VirtualFile)
	//
	// Returns a read-only stream to the virtual resource
	virtual Stream^ Open(void) override
	{
		return m_object->GetReader();			// Get a reader stream
	}

internal:

	// INTERNAL CONSTRUCTOR
	StorageFile(StoragePath^ path, StorageObject^ object) : VirtualFile(path->Full), 
		m_object(object) {}

private:

	//-----------------------------------------------------------------------
	// Member Variables

	StorageObject^			m_object;	// Contained StorageObject instance
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __STORAGEFILE_H_