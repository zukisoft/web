//---------------------------------------------------------------------------
// IComPropertyStorage Declarations
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

#ifndef __ICOMPROPERTYSTORAGE_H_
#define __ICOMPROPERTYSTORAGE_H_
#pragma once

#include "IComPointer.h"				// Include IComPointer declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(data)
BEGIN_NAMESPACE(structured)

//---------------------------------------------------------------------------
// Interface IComPropertyStorage (internal)
//
// IComStorage is an identical interface to the COM IPropertyStorage 
// interface.  This is used to allow multiple COM interfaces to be exposed 
// from a managed class
//---------------------------------------------------------------------------

interface class IComPropertyStorage : public IComPointer
{
	//-----------------------------------------------------------------------
	// Methods

	// Commit
	//
	// Saves changes made to a property storage object to the parent storage object
	HRESULT Commit(DWORD grfCommitFlags);

	// DeleteMultiple
	//
	// Deletes as many of the indicated properties as exist in this property set
	HRESULT DeleteMultiple(ULONG cpspec, const PROPSPEC rgpspec[]);

	// DeletePropertyNames
	//
	// Deletes specified string names from the current property set
	HRESULT DeletePropertyNames(ULONG cpropid, const PROPID rgpropid[]);

	// Enum
	//
	// Enumerates the contents of the property set
	HRESULT Enum(IEnumSTATPROPSTG** ppenum);

	// ReadMultiple
	//
	// Reads specified properties from the current property set.
	HRESULT ReadMultiple(ULONG cpspec, const PROPSPEC rgpspec[], PROPVARIANT rgpropvar[]);

	// ReadPropertyNames
	//
	// retrieves any existing string names for the specified property IDs.
	HRESULT ReadPropertyNames(ULONG cpropid, const PROPID rgpropid[], LPWSTR rglpwstrName[]);

	// Revert
	//
	// Discards all changes to the named property set
	HRESULT Revert(void);

	// SetClass
	//
	// Assigns a new CLSID to the current property storage object
	HRESULT SetClass(REFCLSID clsid);

	// SetTimes
	//
	// sets the modification, access, and creation times of this property set
	HRESULT SetTimes(const ::FILETIME* pctime, const ::FILETIME* patime, const ::FILETIME* pmtime);

	// Stat
	//
	// Retrieves information about the current open property set
	HRESULT Stat(STATPROPSETSTG* pstatpsstg);

	// WriteMultiple
	//
	// Writes a specified group of properties to the current property set
	HRESULT WriteMultiple(ULONG cpspec, const PROPSPEC rgpspec[], const PROPVARIANT rgpropvar[], 
		PROPID propidNameFirst);

	// WritePropertyNames
	//
	// Assigns string names to a specified array of property IDs in the current property set
	HRESULT WritePropertyNames(ULONG cpropid, const PROPID rgpropid[], LPWSTR const rglpwstrName[]);
};

//---------------------------------------------------------------------------

END_NAMESPACE(structured)
END_NAMESPACE(data)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __ICOMPROPERTYSTORAGE_H_