//---------------------------------------------------------------------------
// WebServiceReservationCollection Declarations
//
// Zucchini Service Configuration Library
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

#ifndef __WEBSERVICERESERVATIONCOLLECTION_H_
#define __WEBSERVICERESERVATIONCOLLECTION_H_
#pragma once

#include "WebReservation.h"					// Include WebReservation decls

#pragma warning(push, 4)					// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::ComponentModel;
using namespace System::Runtime::InteropServices;
using namespace System::Security;
using namespace System::Security::AccessControl;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(config)

//---------------------------------------------------------------------------
// Class WebServiceReservationCollection
//
// WebServiceReservationCollection implements a collection that is used to 
// manage the HTTP.SYS URL reservations on this system
//---------------------------------------------------------------------------

public ref class WebServiceReservationCollection sealed : public Generic::ICollection<WebReservation^>
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// Add (ICollection<T>)
	//
	// Adds a new prefix object to the collection
	virtual void Add(WebReservation^ reservation);

	// Add
	//
	// Adds a new reservation to the collection
	void Add(String^ prefix) { Add(gcnew WebReservation(prefix)); }
	void Add(String^ prefix, String^ sddl) { Add(gcnew WebReservation(prefix, sddl)); }
	void Add(String^ prefix, WebReservationSecurity^ security) { Add(gcnew WebReservation(prefix, security)); }

	// Contains (ICollection<T>)
	//
	// Determines if the specified reservation exists in the collection
	virtual bool Contains(WebReservation^ reservation);

	// Contains
	//
	// Determines if the specified reservation exists in this collection
	bool Contains(String^ prefix) { return Contains(gcnew WebReservation(prefix)); }

	// CopyTo (ICollection<T>)
	//
	// Copies the contents of the collection into an array
	virtual void CopyTo(array<WebReservation^>^ array, int offset) { return m_col->CopyTo(array, offset); }

	// GetEnumerator (IEnumerable<T>)
	//
	// Creates and returns a new collection enumerator instance
	virtual Generic::IEnumerator<WebReservation^>^ GetEnumerator(void) { return m_col->GetEnumerator(); }

	// Refresh
	//
	// Clears and reloads the contents of the collection from the system
	void Refresh(void);

	// Remove (ICollection<T>)
	//
	// Removes the specified reservation from the collection
	virtual bool Remove(WebReservation^ reservation);

	// Remove
	//
	// Removes the specified reservation from the collection
	bool Remove(String^ prefix) { return Remove(gcnew WebReservation(prefix)); }

	//-----------------------------------------------------------------------
	// Properties

	// default[int]
	//
	// Accesses an object in the collection by it's index
	property WebReservation^ default[int]
	{
		WebReservation^ get(int index) { return gcnew WebReservation(m_col[index]); }
	}

	// Count (ICollection<T>)
	//
	// Gets the number of running web application instances
	virtual property int Count
	{
		int get(void) { return m_col->Count; }
	}

	// IsReadOnly (ICollection<T>)
	//
	// Determines if the collection is read-only or not
	virtual property bool IsReadOnly
	{
		bool get(void) { return false; }
	}

internal:

	// INTERNAL CONSTRUCTOR
	WebServiceReservationCollection();

private:

	//-----------------------------------------------------------------------
	// Private Member Functions

	// Clear (ICollection<T>)
	//
	// Removes all reservations from the collection. Not implemented because it
	// would be very dangerous to let a single method call wipe out every single
	// namespace reservation on a system
	virtual void Clear(void) sealed =
		Generic::ICollection<WebReservation^>::Clear { throw gcnew NotImplementedException(); }

	// _GetEnumerator (IEnumerator::GetEnumerator)
	//
	// Privatized interface implementation
	virtual Collections::IEnumerator^ _GetEnumerator(void) sealed = 
		Collections::IEnumerable::GetEnumerator { return GetEnumerator(); }

	//-----------------------------------------------------------------------
	// Member Variables

	List<WebReservation^>^			m_col;			// Inner collection
};

//---------------------------------------------------------------------------

END_NAMESPACE(config)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __WEBSERVICERESERVATIONCOLLECTION_H_