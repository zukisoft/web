//---------------------------------------------------------------------------
// StorageUtil Declarations
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

#ifndef __STORAGEUTIL_H_
#define __STORAGEUTIL_H_
#pragma once

#include "IComPropertyStorage.h"		// Include IComPropertyStorage decls
#include "IComStorage.h"				// Include IComStorage declarations
#include "IComStream.h"					// Include IComStream declarations
#include "StorageException.h"			// Include StorageException declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(data)
BEGIN_NAMESPACE(structured)

//---------------------------------------------------------------------------
// Forward Class Declarations
//---------------------------------------------------------------------------

ref class StorageContainer;				// StorageContainer.h
ref class StorageObject;				// StorageObject.h

//---------------------------------------------------------------------------
// Function Prototypes
//---------------------------------------------------------------------------

Guid GetStreamGuid(IStream* pStream);

//---------------------------------------------------------------------------
// Class StorageUtil (internal)
//
// StorageUtil contains some handy-dandy functions used throughout the
// class library for general "stuff".
//---------------------------------------------------------------------------

ref class StorageUtil
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	static Guid		Base64ToSysGuid(String^ base64);
	static int		CompareUUIDs(const UUID &lhs, const UUID &rhs);
	static Guid		GetContainerID(IComStorage^ storage);
	static Guid		GetObjectID(IComStream^ stream);
	static Guid		GetPropertySetID(IComPropertyStorage^ propStorage);
	static DWORD	GetStorageChildOpenMode(IComStorage^ storage);
	static DWORD	GetStorageMode(IComStorage^ storage);
	static DWORD	GetStreamMode(IComStream^ stream);
	static bool		IsStorageReadOnly(IComStorage^ storage);
	static bool		IsStreamReadOnly(IComStream^ stream);
	static String^	SysGuidToBase64(Guid guid);
	static UUID		SysGuidToUUID(Guid guid);
	static Guid		UUIDToSysGuid(const UUID& guid);
};

//---------------------------------------------------------------------------

END_NAMESPACE(structured)
END_NAMESPACE(data)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __STORAGEUTIL_H_
