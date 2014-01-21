//---------------------------------------------------------------------------
// StorageChildCollection Declarations
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

#ifndef __STORAGECHILDCOLLECTION_H_
#define __STORAGECHILDCOLLECTION_H_
#pragma once

#include "StorageChildEnumerator.h"			// Include StorageChildEnumerator
#include "StoragePath.h"					// Include StoragePath declarations

#pragma warning(push, 4)					// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Web::Hosting;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class StorageChildCollection (internal)
//
// StorageChildCollection implements an enumerable collection of StorageFiles
// and StorageDirectories based on the contents of a StorageContainer instance
//---------------------------------------------------------------------------

ref class StorageChildCollection sealed : public Generic::IEnumerable<VirtualFileBase^>
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// GetEnumerator (IEnumerable<T>)
	//
	// Creates and returns a new collection enumerator instance
	virtual Generic::IEnumerator<VirtualFileBase^>^ GetEnumerator(void)
	{ 
		return gcnew StorageChildEnumerator(m_path, m_container); 
	}

internal:

	// INTERNAL CONSTRUCTOR
	StorageChildCollection(StoragePath^ path, StorageContainer^ container) : 
		m_path(path), m_container(container) {}

private:

	//-----------------------------------------------------------------------
	// Private Member Functions

	// IEnumerator::GetEnumerator
	virtual Collections::IEnumerator^ _GetEnumerator(void) sealed = 
		Collections::IEnumerable::GetEnumerator { return GetEnumerator(); }

	//-----------------------------------------------------------------------
	// Member Variables

	StoragePath^			m_path;				// Base virtual directory
	StorageContainer^		m_container;		// Referenced container object
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __STORAGECHILDCOLLECTION_H_
