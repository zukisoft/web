//---------------------------------------------------------------------------
// UriPrefixPort Declarations
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

#ifndef __URIPREFIXPORT_H_
#define __URIPREFIXPORT_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class UriPrefixPort
//
// Defines common prefix ports (80/443) using a strongly typed wrapper class
//---------------------------------------------------------------------------

[Serializable()]
public value class UriPrefixPort sealed
{
private:

	//-----------------------------------------------------------------------
	// Private Type Declarations

	// _Values
	//
	// Defines an enumeration used for the underlying values for the sole purpose
	// of allowing .ToString() to provide enumeration-like output ...
	enum class _Values
	{
		Dynamic		= 0,
		Http		= 80,
		Https		= 443,
	};

public:

	//-----------------------------------------------------------------------
	// Overloaded Operators

	bool operator ==(UriPrefixPort rhs) { return rhs.m_port == this->m_port; }
	bool operator !=(UriPrefixPort rhs) { return rhs.m_port != this->m_port; }

	static operator int(UriPrefixPort val) { return val.m_port; }

	//-----------------------------------------------------------------------
	// Member Functions

	// ToString (Object)
	//
	// Converts this object instance into it's string representation
	virtual String^ ToString(void) override 
	{ 
		// Use the enumeration value whenever possible for the generic ToString,
		// since that will be used for things like persistance into XML ...

		String^ result = Enum::GetName(_Values::typeid, m_port);
		return (result != nullptr) ? result : m_port.ToString();
	}

	//-----------------------------------------------------------------------
	// Fields

	static initonly UriPrefixPort Dynamic	= UriPrefixPort(_Values::Dynamic);
	static initonly UriPrefixPort Http		= UriPrefixPort(_Values::Http);
	static initonly UriPrefixPort Https		= UriPrefixPort(_Values::Https);

internal:

	// INTERNAL CONSTRUCTOR
	UriPrefixPort(int port) : m_port(port) {}

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Parse
	//
	// Attempts to convert a string into a valid UriPrefixPort value
	static UriPrefixPort Parse(String^ value)
	{
		// First try to cast the enumeration name, and if that fails assume it's
		// an unsigned integer.  Let that throw FormatException() as necessary

		try { return UriPrefixPort(static_cast<int>(Enum::Parse(_Values::typeid, value, true))); }
		catch(Exception^) { return UriPrefixPort(static_cast<int>(Convert::ToUInt32(value))); }
	}

private:

	// PRIVATE CONSTRUCTOR
	UriPrefixPort(_Values port) : m_port(static_cast<int>(port)) {}

	//-----------------------------------------------------------------------
	// Member Variables
	
	initonly int			m_port;				// Prefix port number
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif		// __URIPREFIXPORT_H_