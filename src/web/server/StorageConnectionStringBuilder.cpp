//---------------------------------------------------------------------------
// StorageConnectionStringBuilder Implementation
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

#include "stdafx.h"								// Include project pre-compiled headers
#include "StorageConnectionStringBuilder.h"		// Include class declarations

#pragma warning(push, 4)						// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// StorageConnectionStringBuilder Constructor
//
// Arguments:
//
//	NONE

StorageConnectionStringBuilder::StorageConnectionStringBuilder()
{
	ResetAll();						// Initialize to all defaults
}

//---------------------------------------------------------------------------
// StorageConnectionStringBuilder Constructor
//
// Arguments:
//
//	connectionString	- Existing connection string to be parsed

StorageConnectionStringBuilder::StorageConnectionStringBuilder(String^ connectionString)
{
	ResetAll();									// Initialize to all defaults

	// If the provided connection string is not empty, try to set it up.
	// (The base class kindly calls our indexer for us on each item)

	if((connectionString != nullptr) && (connectionString->Length > 0))
		__super::ConnectionString = connectionString;
}

//---------------------------------------------------------------------------
// StorageConnectionStringBuilder::AllowPassThrough::set
//
// Sets the option for allowing local file system pass-through or not

void StorageConnectionStringBuilder::AllowPassThrough::set(bool value)
{
	DbConnectionStringBuilder::default[s_keywords[static_cast<int>(KeywordCode::AllowPassThrough)]] = value.ToString();
	m_allowPassThrough = value;
}

//---------------------------------------------------------------------------
// StorageConnectionStringBuilder::Clear
//
// Removes all items from the collection and returns the settings back to
// their default states
//
// Arguments:
//
//	NONE

void StorageConnectionStringBuilder::Clear(void)
{
	__super::Clear();			// Remove all collection items
	ResetAll();					// Reset all connection properties
}

//---------------------------------------------------------------------------
// StorageConnectionStringBuilder::ContainsKey
//
// Determines if the specified keyword exists or not
//
// Arguments:
//
//	keyword		- Keyword to be tested

bool StorageConnectionStringBuilder::ContainsKey(String^ keyword)
{
	if(keyword == nullptr) throw gcnew ArgumentNullException();
	return s_keywordMap->ContainsKey(keyword);
}

//---------------------------------------------------------------------------
// StorageConnectionStringBuilder::DataSource::set
//
// Sets the path to the structured storage file

void StorageConnectionStringBuilder::DataSource::set(String^ value)
{
	if(value == nullptr) throw gcnew ArgumentNullException();

	DbConnectionStringBuilder::default[s_keywords[static_cast<int>(KeywordCode::DataSource)]] = value;
	m_dataSource = value;
}

//---------------------------------------------------------------------------
// StorageConnectionStringBuilder::default::get
//
// Retrieves a connection string value by keyword
//
// Arguments:
//
//	keyword		- Keyword of the item to be retrieved

Object^ StorageConnectionStringBuilder::default::get(String^ keyword)
{
	return GetAt(GetIndex(keyword));		// Not too difficult ...
}

//---------------------------------------------------------------------------
// StorageConnectionStringBuilder::default::set
//
// Changes the value of a specific connection string item by name
//
// Arguments:
//
//	keyword		- Keyword of the item to be set
//	value		- Value to set the connection string item to

void StorageConnectionStringBuilder::default::set(String^ keyword, Object^ value)
{
	if(value == nullptr) { Remove(keyword); return; }	// NULL: Remove and reset

	// This is pretty ugly stuff.  I've followed the model used to implement the .NET
	// SqlConnectionStringBuilder, but in retrospect I wish I did it my way instead.
	// Of course, the REALLY ugly stuff comes from my own use of enumerations :)

	switch(GetIndex(keyword)) {

		case KeywordCode::AllowPassThrough:
			AllowPassThrough = Convert::ToBoolean(value);
			return;

		case KeywordCode::DataSource:
			DataSource = Convert::ToString(value);
			return;
	}

	// UNSUPPORTED ARGUMENT
	throw gcnew KeyNotFoundException(String::Format("Keyword not supported: [{0}]", keyword));
}

//---------------------------------------------------------------------------
// StorageConnectionStringBuilder::GetAt (private)
//
// Returns the property value at the specified code index
//
// Arguments:
//
//	code		- KeywordCode of the property to get at

Object^ StorageConnectionStringBuilder::GetAt(KeywordCode code)
{
	switch(code) {

		case KeywordCode::AllowPassThrough:			return m_allowPassThrough;
		case KeywordCode::DataSource:				return m_dataSource;
	}

	// UNSUPPORTED ARGUMENT
	throw gcnew ArgumentOutOfRangeException();		// Invalid code
}

//---------------------------------------------------------------------------
// StorageConnectionStringBuilder::GetIndex (private)
//
// Returns the keyword code associated with a specific string keyword
//
// Arguments:
//
//	keyword		- Keyword to be mapped back into a code

StorageConnectionStringBuilder::KeywordCode StorageConnectionStringBuilder::GetIndex(String^ keyword)
{
	KeywordCode				code;			// Code to be returned to the caller

	if(keyword == nullptr) throw gcnew ArgumentNullException();

	if(s_keywordMap->TryGetValue(keyword, code)) return code;
	else throw gcnew KeyNotFoundException(String::Format("Keyword not supported: [{0}]", keyword));
}

//---------------------------------------------------------------------------
// StorageConnectionStringBuilder::Remove
//
// Removes a specific connection string property from the collection.  The
// underlying value will be reset to a default state, not really "removed"
//
// Arguments:
//
//	keyword		- Keyword to remove from the collection

bool StorageConnectionStringBuilder::Remove(String^ keyword)
{
	KeywordCode			code;			// Keyword code from the dictionary
	bool				result;			// Result from this function call

	if(keyword == nullptr) throw gcnew ArgumentNullException();

	if(!s_keywordMap->TryGetValue(keyword, code)) return false;
	result = __super::Remove(s_keywords[static_cast<int>(code)]);

	Reset(code);						// Always reset the underlying value
	return result;						// Return result from base class
}

//---------------------------------------------------------------------------
// StorageConnectionStringBuilder::Reset (private)
//
// Resets a specific connection string property to its default state
//
// Arguments:
//
//	code		- Code of the property value to be reset

void StorageConnectionStringBuilder::Reset(KeywordCode code)
{
	switch(code) {

		case KeywordCode::AllowPassThrough:	m_allowPassThrough = false; return;
		case KeywordCode::DataSource:		m_dataSource = String::Empty; return;
	}
}

//---------------------------------------------------------------------------
// StorageConnectionStringBuilder::ResetAll (private)
//
// Resets all connection properties to their default states
//
// Arguments:
//
//	NONE

void StorageConnectionStringBuilder::ResetAll(void)
{
	for(int index = 0; index < s_keywords->Length; index++) Reset(static_cast<KeywordCode>(index));
}

//---------------------------------------------------------------------------
// StorageConnectionStringBuilder::ShouldSerialize
//
// Determines if the specified key exists in this collection.  (No joke, that's
// what MSDN says this method is for ... look it up!)
//
// Arguments:
//
//	keyword		- Keyword to be tested for existance

bool StorageConnectionStringBuilder::ShouldSerialize(String^ keyword)
{
	KeywordCode			code;			// Keyword code from dictionary

	return s_keywordMap->TryGetValue(keyword, code);
}

//---------------------------------------------------------------------------
// StorageConnectionStringBuilder::StaticConstruct (static, private)
//
// Implements the class static constructor so it doesn't have to be
// declared inline in the header file
//
// Arguments:
//
//	NONE

void StorageConnectionStringBuilder::StaticConstruct(void)
{
	// The only thing we need to do in here is actually load up the keyword
	// dictionary with the appropriate entries.  I fail to see how this can
	// be done with initializers, although in theory you can?

	for(int index = 0; index < s_keywords->Length; index++)
		s_keywordMap->Add(s_keywords[index], static_cast<KeywordCode>(index));
}

//---------------------------------------------------------------------------
// StorageConnectionStringBuilder::TryGetValue
//
// Attempts to retrieve the specified value, but returns true/false
// instead of throwing an exception if it does not exist
//
// Arguments:
//
//	keyword		- Keyword to be looked up
//	value		- [out] On success, contains the requested value

bool StorageConnectionStringBuilder::TryGetValue(String^ keyword, Object^% value)
{
	KeywordCode				code;		// Keyword code looked up in dictionary
	
	value = nullptr;					// Initialize [out] variable to NULL

	if(keyword == nullptr) throw gcnew ArgumentNullException();

	// If we can find the specified keyword, return the current value, even 
	// if it's still set to a default.  Otherwise, leave it NULL and bail 

	if(s_keywordMap->TryGetValue(keyword, code)) { value = GetAt(code); return true; }
	else return false;
}

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
