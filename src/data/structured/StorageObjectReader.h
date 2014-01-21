//---------------------------------------------------------------------------
// StorageObjectReader Declarations
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

#ifndef __STORAGEOBJECTREADER_H_
#define __STORAGEOBJECTREADER_H_
#pragma once

#include "ComStream.h"					// Include ComStream declarations
#include "StorageObjectStreamMode.h"	// Include stream mode enumeration
#include "StorageObjectStream.h"		// Include StorageObjectStream declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(data)
BEGIN_NAMESPACE(structured)

//---------------------------------------------------------------------------
// Class StorageObjectReader
//
// Used to declare a read-only StorageObjectStream
//---------------------------------------------------------------------------

STRUCTURED_STORAGE_PUBLIC ref class StorageObjectReader sealed : public StorageObjectStream
{
internal:

	StorageObjectReader(ComStream^ stream) : StorageObjectStream(stream, 
		StorageObjectStreamMode::Reader) {}
};

//---------------------------------------------------------------------------

END_NAMESPACE(structured)
END_NAMESPACE(data)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __STORAGEOBJECTREADER_H_
