//---------------------------------------------------------------------------
// StorageConnectionStringBuilder Declarations
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

#ifndef __STORAGECONNECTIONSTRINGBUILDER_H_
#define __STORAGECONNECTIONSTRINGBUILDER_H_
#pragma once

#pragma warning(push, 4)					// Enable maximum compiler warnings

using namespace System;
using namespace System::Data;
using namespace System::Data::Common;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class StorageConnectionStringBuilder
//
// Implements an ADO.NET connection string builder specific to the zucchini
// structured storage implementation
//---------------------------------------------------------------------------

public ref class StorageConnectionStringBuilder sealed : public DbConnectionStringBuilder
{
public:

	StorageConnectionStringBuilder();
	StorageConnectionStringBuilder(String^ connectionString);

	//-----------------------------------------------------------------------
	// Member Functions

	// Clear (DbConnectionStringBuilder)
	//
	// Removes all key/value pairs and resets all properties to default
	virtual void Clear(void) override;

	// ContainsKey (DbConnectionStringBuilder)
	//
	// Determines if a specific keyword is valid or not
	virtual bool ContainsKey(String^ keyword) override;

	// Remove (DbConnectionStringBuilder)
	//
	// Removes a key/value pair by keyword and resets its property to default
	virtual bool Remove(String^ keyword) override;

	// ShouldSerialize (DbConnectionStringBuilder)
	//
	// Determines if a specific keyword is valid or not
	virtual bool ShouldSerialize(String^ keyword) override;

	// TryGetValue (DbConnectionStringBuilder)
	//
	// Attempts to access a specific value without throwing any exceptions
	virtual bool TryGetValue(String^ keyword, Object^% value) override;

	//-----------------------------------------------------------------------
	// Properties

	// AllowPassThrough = { true | false }
	//
	// Determines if local file system pass-through is enabled or not
	property bool AllowPassThrough
	{
		bool get(void) { return m_allowPassThrough; }
		void set(bool value);
	}

	// DataSource = { path }
	//
	// Provides the location of the structured storage file
	property String^ DataSource
	{
		String^ get(void) { return m_dataSource; }
		void set(String^ value);
	}

	//-----------------------------------------------------------------------
	// Indexers

	// via String^ (DbConnectionStringBuilder)
	//
	// Gets or sets a specific connection string property by keyword
	virtual property Object^ default[String^]
	{
		Object^ get(String^ keyword) override;
		void set(String^ keyword, Object^ value) override;
	}

private:

	// STATIC CONSTRUCTOR
	static StorageConnectionStringBuilder() { StaticConstruct(); }

	//-----------------------------------------------------------------------
	// Private Constants

	// KeywordCode
	//
	// Defines codes for each of the valid connection stringkeywords
	enum struct KeywordCode
	{
		AllowPassThrough = 0,
		DataSource,
	};

	//-----------------------------------------------------------------------
	// Private Member Functions

	Object^		GetAt(KeywordCode code);
	KeywordCode	GetIndex(String^ keyword);
	void		Reset(KeywordCode code);
	void		ResetAll(void);
	static void StaticConstruct(void);

	//-----------------------------------------------------------------------
	// Member Variables

	bool						m_allowPassThrough;		// ALLOW PASS THROUGH=
	String^						m_dataSource;			// DATA SOURCE=

	// s_keywords
	//
	// Defines all of the connection string keywords available via an enumeration.
	// This used to be string constants with embedded spaces, but I switch it to 
	// mimick what the SqlConnectionStringBuilder does internally instead.
	static initonly array<String^>^ s_keywords = gcnew array<String^> 
	{
		"Allow Pass Through",
		"Data Source",
	};

	// s_keywordMap
	//
	// Defines a static string->code mapping for all of the keywords
	static initonly Generic::Dictionary<String^, KeywordCode>^ s_keywordMap =
		gcnew Generic::Dictionary<String^, KeywordCode>(StringComparer::OrdinalIgnoreCase);
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __STORAGECONNECTIONSTRINGBUILDER_H_
