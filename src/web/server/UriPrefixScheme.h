//---------------------------------------------------------------------------
// UriPrefixScheme Declarations
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

#ifndef __URIPREFIXSCHEME_H_
#define __URIPREFIXSCHEME_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class UriPrefixScheme
//
// Defines the possible listener schemes (http/https) as a strongly typed
// class instead of simple string constants to ensure type safety
//---------------------------------------------------------------------------

[Serializable()]
public value class UriPrefixScheme sealed
{
public:

	//-----------------------------------------------------------------------
	// Overloaded Operators

	bool operator ==(UriPrefixScheme rhs) { return rhs.m_scheme == this->m_scheme; }
	bool operator !=(UriPrefixScheme rhs) { return rhs.m_scheme != this->m_scheme; }

	static operator String^(UriPrefixScheme val) { return val.m_scheme; }

	//-----------------------------------------------------------------------
	// Member Functions

	// ToString (Object)
	//
	// Converts this object instance into it's string representation
	virtual String^ ToString(void) override { return m_scheme; }

	//-----------------------------------------------------------------------
	// Fields

	static initonly UriPrefixScheme Http	= UriPrefixScheme(Uri::UriSchemeHttp);
	static initonly UriPrefixScheme Https	= UriPrefixScheme(Uri::UriSchemeHttps);

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Parse
	//
	// Attempts to convert a string into a valid UriPrefixScheme value
	static UriPrefixScheme Parse(String^ value)
	{
		if(String::IsNullOrEmpty(value)) throw gcnew ArgumentNullException();

		// Nothing overly fancy here, it has to be "http" or "https" ...

		if(String::Compare(value, Uri::UriSchemeHttp, StringComparison::OrdinalIgnoreCase) == 0) return UriPrefixScheme::Http;
		else if(String::Compare(value, Uri::UriSchemeHttps, StringComparison::OrdinalIgnoreCase) == 0) return UriPrefixScheme::Https;
		else throw gcnew FormatException();
	}

	//-----------------------------------------------------------------------
	// Internal Properties

	// Value
	//
	// Gets the underlying scheme string
	property String^ Value { String^ get(void) { return m_scheme; } }

private:

	// PRIVATE CONSTRUCTOR
	UriPrefixScheme(String^ scheme) : m_scheme(scheme) {}

	//-----------------------------------------------------------------------
	// Member Variables
	
	initonly String^			m_scheme;		// Prefix scheme string
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif		// __URIPREFIXSCHEME_H_