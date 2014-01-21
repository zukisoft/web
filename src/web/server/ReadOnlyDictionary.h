//---------------------------------------------------------------------------
// ReadOnlyDictionary Declarations
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

#ifndef __READONLYDICTIONARY_H_
#define __READONLYDICTIONARY_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections;
using namespace System::Collections::ObjectModel;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class ReadOnlyDictionary (internal)
//
// Implements a basic read-only Dictionary<> implementation.  Not very fancy,
// just a quick way to get rid of .Add, .Remove, and things like that.
//---------------------------------------------------------------------------

generic<typename TKey, typename TValue>
ref class ReadOnlyDictionary : public Generic::IDictionary<TKey, TValue>
{
public:

	//-----------------------------------------------------------------------
	// Constructor
	
	ReadOnlyDictionary(Generic::IDictionary<TKey, TValue>^ inner) : m_inner(inner) {}

	//-----------------------------------------------------------------------
	// Member Functions

	// ContainsKey (IDictionary<T>)
	//
	// Determines if a key with the specified value exists
	virtual bool ContainsKey(TKey key)
	{
		return m_inner->ContainsKey(key);
	}

	// GetEnumerator (IEnumerable<T>)
	//
	// Creates and returns a new collection enumerator instance
	virtual Generic::IEnumerator<KeyValuePair<TKey, TValue>>^ GetEnumerator(void)
	{ 
		return m_inner->GetEnumerator();
	}

	// TryGetValue (IDictionary<T>)
	//
	// Attempts to retrieve the specified value, without throwing an exception
	virtual bool TryGetValue(TKey key, [OutAttribute] TValue% value)
	{
		return m_inner->TryGetValue(key, value);
	}

	//-----------------------------------------------------------------------
	// Properties

	// INDEXER (IDictionary<T>)
	//
	// Retrieves a value from the collection via it's key
	property TValue default [TKey] 
	{
		virtual TValue get(TKey key) { return m_inner[key]; }
	}

	// Count (ICollection<T>)
	//
	// Returns the number of key/value pairs in the collection
	property int Count
	{
		virtual int get(void) { return m_inner->Count; }
	}

	// IsReadOnly (ICollection<T>)
	//
	// Determines if this collection is read-only or not
	property bool IsReadOnly
	{
		virtual bool get(void) { return true; }
	}

	// Keys (IDictionary<T>)
	//
	// Retrieves a collection of the keys stored in the dictionary
	property Generic::ICollection<TKey>^ Keys
	{
		virtual Generic::ICollection<TKey>^ get(void) { return gcnew ReadOnlyCollection<TKey>(gcnew List<TKey>(m_inner->Keys)); }
	}

	// Keys (IDictionary<T>)
	//
	// Retrieves a collection of the values stored in the dictionary
	property Generic::ICollection<TValue>^ Values
	{
		virtual Generic::ICollection<TValue>^ get(void) { return gcnew ReadOnlyCollection<TValue>(gcnew List<TValue>(m_inner->Values)); }
	}

private:

	//-----------------------------------------------------------------------
	// Private Member Functions

	// ICollection<>::Add
	virtual void _Add(KeyValuePair<TKey, TValue>) sealed = Generic::ICollection<KeyValuePair<TKey, TValue>>::Add
		{ throw gcnew NotImplementedException(); }

	// IDictionary<>::Add
	virtual void Add(TKey, TValue) sealed = Generic::IDictionary<TKey, TValue>::Add
		{ throw gcnew NotImplementedException(); }
	
	// ICollection<>::CopyTo
	virtual void CopyTo(array<KeyValuePair<TKey, TValue>>^ arr, int arrayIndex) sealed = Generic::ICollection<KeyValuePair<TKey, TValue>>::CopyTo
		{ return static_cast<Generic::ICollection<KeyValuePair<TKey, TValue>>^>(m_inner)->CopyTo(arr, arrayIndex); }

	// IDictionary<>::Clear
	virtual void Clear(void) sealed = Generic::IDictionary<TKey, TValue>::Clear 
		{ throw gcnew NotImplementedException(); }

	// ICollection<>::Contains (ICollection<T>)
	virtual bool Contains(KeyValuePair<TKey, TValue> item) sealed = Generic::ICollection<KeyValuePair<TKey, TValue>>::Contains
		{ return static_cast<Generic::ICollection<KeyValuePair<TKey, TValue>>^>(m_inner)->Contains(item); }

	// IDictionary<>::default::set
	property TValue _Item [TKey]
	{
		virtual void set(TKey, TValue) sealed = Generic::IDictionary<TKey, TValue>::default::set
			{ throw gcnew NotImplementedException(); }
	}

	// IEnumerator::GetEnumerator
	virtual Collections::IEnumerator^ _GetEnumerator(void) sealed = 
		Collections::IEnumerable::GetEnumerator { return GetEnumerator(); }

	// ICollection<>::Remove
	virtual bool _Remove(KeyValuePair<TKey, TValue>) sealed = Generic::ICollection<KeyValuePair<TKey, TValue>>::Remove
		{ throw gcnew NotImplementedException(); }

	// IDictionary<>::Remove
	virtual bool Remove(TKey) sealed = Generic::IDictionary<TKey, TValue>::Remove
		{ throw gcnew NotImplementedException(); }

	//-----------------------------------------------------------------------
	// Member Variables

	Generic::IDictionary<TKey, TValue>^	m_inner;	// Contained collection
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __READONLYDICTIONARY_H_