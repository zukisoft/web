//---------------------------------------------------------------------------
// UriPrefixCollection Declarations
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

#ifndef __URIPREFIXCOLLECTION_H_
#define __URIPREFIXCOLLECTION_H_
#pragma once

#include "exceptions.h"						// Include exception declarations
#include "UriPrefix.h"						// Include UriPrefix declarations
#include "UriPrefixHost.h"					// Include UriPrefixHost declarations
#include "UriPrefixPort.h"					// Include UriPrefixPort declarations
#include "UriPrefixScheme.h"				// Include UriPrefixScheme declarations
#include "XmlConstants.h"					// Include XmlConstants declarations

#pragma warning(push, 4)					// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Net;
using namespace System::Xml;
using namespace zuki::web::server::exceptions;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class UriPrefixCollection
//
// UriPrefixCollection implements a collection of prefix strings that ultimately
// get passed into the HttpListener instance for it's configuration.  UriPrefix
// isn't very fancy, but it seemed a lot nicer than just having the application
// construct all of the prefixes without the use of any constants
//---------------------------------------------------------------------------

[Serializable()]
public ref class UriPrefixCollection sealed : public Generic::ICollection<UriPrefix^>
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// Add (ICollection<T>)
	//
	// Adds a new prefix object to the collection
	virtual void Add(UriPrefix^ uriPrefix);

	// Add
	//
	// Adds a new prefix object to the collection
	void Add(UriPrefixScheme scheme, UriPrefixHost host) { Add(gcnew UriPrefix(scheme, host)); }
	void Add(UriPrefixScheme scheme, String^ host) { Add(gcnew UriPrefix(scheme, host)); }
	void Add(UriPrefixScheme scheme, UriPrefixHost host, UriPrefixPort port) { Add(gcnew UriPrefix(scheme, host, port)); }
	void Add(UriPrefixScheme scheme, String^ host, UriPrefixPort port) { Add(gcnew UriPrefix(scheme, host, port)); }
	void Add(UriPrefixScheme scheme, UriPrefixHost host, int port) { Add(gcnew UriPrefix(scheme, host, port)); }
	void Add(UriPrefixScheme scheme, String^ host, int port) { Add(gcnew UriPrefix(scheme, host, port)); }

	// Clear (ICollection<T>)
	//
	// Removes all prefixes from the collection
	virtual void Clear(void) { m_col->Clear(); }

	// Contains (ICollection<T>)
	//
	// Determines if the specified URI prefix exists in the collection
	virtual bool Contains(UriPrefix^ uriPrefix);

	// Contains
	//
	// Determines if the specified URI prefix exists in this collection
	bool Contains(UriPrefixScheme scheme, UriPrefixHost host) { return Contains(gcnew UriPrefix(scheme, host)); }
	bool Contains(UriPrefixScheme scheme, String^ host) { return Contains(gcnew UriPrefix(scheme, host)); }
	bool Contains(UriPrefixScheme scheme, UriPrefixHost host, UriPrefixPort port) { return Contains(gcnew UriPrefix(scheme, host, port)); }
	bool Contains(UriPrefixScheme scheme, String^ host, UriPrefixPort port) { return Contains(gcnew UriPrefix(scheme, host, port)); }
	bool Contains(UriPrefixScheme scheme, UriPrefixHost host, int port) { return Contains(gcnew UriPrefix(scheme, host, port)); }
	bool Contains(UriPrefixScheme scheme, String^ host, int port) { return Contains(gcnew UriPrefix(scheme, host, port)); }

	// CopyTo (ICollection<T>)
	//
	// Copies the contents of the collection into an array
	virtual void CopyTo(array<UriPrefix^>^ array, int offset) { return m_col->CopyTo(array, offset); }

	// GetEnumerator (IEnumerable<T>)
	//
	// Creates and returns a new collection enumerator instance
	virtual Generic::IEnumerator<UriPrefix^>^ GetEnumerator(void) { return m_col->GetEnumerator(); }

	// Remove (ICollection<T>)
	//
	// Removes the specified prefix object from the collection
	virtual bool Remove(UriPrefix^ uriPrefix);

	// Remove
	//
	// Removes the specified prefix object from the collection
	bool Remove(UriPrefixScheme scheme, UriPrefixHost host) { return Remove(gcnew UriPrefix(scheme, host)); }
	bool Remove(UriPrefixScheme scheme, String^ host) { return Remove(gcnew UriPrefix(scheme, host)); }
	bool Remove(UriPrefixScheme scheme, UriPrefixHost host, UriPrefixPort port) { return Remove(gcnew UriPrefix(scheme, host, port)); }
	bool Remove(UriPrefixScheme scheme, String^ host, UriPrefixPort port) { return Remove(gcnew UriPrefix(scheme, host, port)); }
	bool Remove(UriPrefixScheme scheme, UriPrefixHost host, int port) { return Remove(gcnew UriPrefix(scheme, host, port)); }
	bool Remove(UriPrefixScheme scheme, String^ host, int port) { return Remove(gcnew UriPrefix(scheme, host, port)); }

	//-----------------------------------------------------------------------
	// Properties

	// default[int]
	//
	// Accesses an object in the collection by it's index
	property UriPrefix^ default[int]
	{
		UriPrefix^ get(int index) { return m_col[index]; }
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
	UriPrefixCollection() : m_col(gcnew List<UriPrefix^>()) {}

	// ReadXml
	//
	// Reads the state of this instance from an XmlNode
	void ReadXml(XmlNode^ parent);

	// ToXml
	//
	// Writes the state of this instance into an XmlWriter
	void ToXml(XmlWriter^ writer);

private:

	//-----------------------------------------------------------------------
	// Private Member Functions

	// _GetEnumerator (IEnumerator::GetEnumerator)
	//
	// Privatized interface implementation
	virtual Collections::IEnumerator^ _GetEnumerator(void) sealed = 
		Collections::IEnumerable::GetEnumerator { return GetEnumerator(); }

	//-----------------------------------------------------------------------
	// Member Variables

	List<UriPrefix^>^			m_col;			// Contained collection
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __URIPREFIXCOLLECTION_H_