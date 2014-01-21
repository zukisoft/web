//---------------------------------------------------------------------------
// IComPropertySetStorage Declarations
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

#ifndef __ICOMPROPERTYSETSTORAGE_H_
#define __ICOMPROPERTYSETSTORAGE_H_
#pragma once

#include "IComPointer.h"				// Include IComPointer declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(data)
BEGIN_NAMESPACE(structured)

//---------------------------------------------------------------------------
// Interface IComPropertySetStorage (internal)
//
// IComStorage is an identical interface to the COM IPropertySetStorage 
// interface.  This is used to allow multiple COM interfaces to be exposed 
// from a managed class
//---------------------------------------------------------------------------

interface class IComPropertySetStorage : public IComPointer
{
	//-----------------------------------------------------------------------
	// Methods

	// Create
	//
	// Creates and opens a new property set in the property set storage object
	HRESULT Create(REFFMTID fmtid, const CLSID* pclsid, DWORD grfFlags, DWORD grfMode,
		IPropertyStorage** ppPropStg);

	// Delete
	//
	// Deletes one of the property sets contained in the property set storage object
	HRESULT Delete(REFFMTID fmtid);

	// Enum
	//
	// Creates an enumerator object which contains information on the property sets
	HRESULT Enum(IEnumSTATPROPSETSTG** ppenum);

	// Open
	//
	// Opens a property set contained in the property set storage object
	HRESULT Open(REFFMTID fmtid, DWORD grfMode, IPropertyStorage** ppPropStg);
};

//---------------------------------------------------------------------------

END_NAMESPACE(structured)
END_NAMESPACE(data)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __ICOMPROPERTYSETSTORAGE_H_