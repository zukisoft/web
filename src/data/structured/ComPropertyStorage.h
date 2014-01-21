//---------------------------------------------------------------------------
// ComPropertyStorage Declarations
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

#ifndef __COMPROPERTYSTORAGE_H_
#define __COMPROPERTYSTORAGE_H_
#pragma once

#include "IComPropertyStorage.h"		// Include IComPropertyStorage decls
#include "StorageException.h"			// Include StorageException decls

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(data)
BEGIN_NAMESPACE(structured)

//---------------------------------------------------------------------------
// Class ComPropertyStorage (internal)
//
// ComPropertyStorage implements a safe pointer class that allows a COM pointer
// to be safely shared among managed object instances, and implements thread
// safety for all member functions
//---------------------------------------------------------------------------

ref class ComPropertyStorage sealed : public IComPropertyStorage
{
public:

	//-----------------------------------------------------------------------
	// Constructor
	
	ComPropertyStorage(IPropertyStorage* pPropStorage);

	//-----------------------------------------------------------------------
	// Member Functions

	// Commit (IComPropertyStorage)
	//
	// Saves changes made to a property storage object to the parent storage object
	virtual HRESULT Commit(DWORD grfCommitFlags);

	// DeleteMultiple (IComPropertyStorage)
	//
	// Deletes as many of the indicated properties as exist in this property set
	virtual HRESULT DeleteMultiple(ULONG cpspec, const PROPSPEC rgpspec[]);

	// DeletePropertyNames (IComPropertyStorage)
	//
	// Deletes specified string names from the current property set
	virtual HRESULT DeletePropertyNames(ULONG cpropid, const PROPID rgpropid[]);

	// Enum (IComPropertyStorage)
	//
	// Enumerates the contents of the property set
	virtual HRESULT Enum(IEnumSTATPROPSTG** ppenum);

	// IsDisposed (IComPointer)
	//
	// Exposes the object's internal disposed status
	virtual bool IsDisposed(void) { return m_disposed; }

	// ReadMultiple (IComPropertyStorage)
	//
	// Reads specified properties from the current property set.
	virtual HRESULT ReadMultiple(ULONG cpspec, const PROPSPEC rgpspec[], PROPVARIANT rgpropvar[]);

	// ReadPropertyNames (IComPropertyStorage)
	//
	// retrieves any existing string names for the specified property IDs.
	virtual HRESULT ReadPropertyNames(ULONG cpropid, const PROPID rgpropid[], LPWSTR rglpwstrName[]);

	// Revert (IComPropertyStorage)
	//
	// Discards all changes to the named property set
	virtual HRESULT Revert(void);

	// SetClass (IComPropertyStorage)
	//
	// Assigns a new CLSID to the current property storage object
	virtual HRESULT SetClass(REFCLSID clsid);

	// SetTimes (IComPropertyStorage)
	//
	// sets the modification, access, and creation times of this property set
	virtual HRESULT SetTimes(const ::FILETIME* pctime, const ::FILETIME* patime, const ::FILETIME* pmtime);

	// Stat (IComPropertyStorage)
	//
	// Retrieves information about the current open property set
	virtual HRESULT Stat(STATPROPSETSTG* pstatpsstg);

	// WriteMultiple (IComPropertyStorage)
	//
	// Writes a specified group of properties to the current property set
	virtual HRESULT WriteMultiple(ULONG cpspec, const PROPSPEC rgpspec[], const PROPVARIANT rgpropvar[], 
		PROPID propidNameFirst);

	// WritePropertyNames (IComPropertyStorage)
	//
	// Assigns string names to a specified array of property IDs in the current property set
	virtual HRESULT WritePropertyNames(ULONG cpropid, const PROPID rgpropid[], LPWSTR const rglpwstrName[]);

private:

	// DESTRUCTOR / FINALIZER
	~ComPropertyStorage() { this->!ComPropertyStorage(); m_disposed = true; }
	!ComPropertyStorage();

	//-----------------------------------------------------------------------
	// Member Variables

	bool					m_disposed;			// Object disposal flag
	IPropertyStorage*		m_pPropStorage;		// Contained IPropertyStorage
};

//---------------------------------------------------------------------------

END_NAMESPACE(structured)
END_NAMESPACE(data)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __COMPROPERTYSTORAGE_H_