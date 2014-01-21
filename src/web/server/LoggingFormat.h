//---------------------------------------------------------------------------
// LoggingFormat Declarations
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

#ifndef __LOGGINGFORMAT_H_
#define __LOGGINGFORMAT_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class LoggingFormat
//
// Defines all valid application logging format possibilities using a
// strongly typed wrapper class instead of an enumeration
//---------------------------------------------------------------------------

[Serializable()]
public value class LoggingFormat sealed
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
		None = 0,
		NCSACombined = 1,
		NCSACommon = 2,
	};

public:

	//-----------------------------------------------------------------------
	// Overloaded Operators

	bool operator ==(LoggingFormat rhs) { return rhs.m_format == this->m_format; }
	bool operator !=(LoggingFormat rhs) { return rhs.m_format != this->m_format; }

	//-----------------------------------------------------------------------
	// Member Functions

	// ToString (Object)
	//
	// Converts this object instance into it's string representation
	virtual String^ ToString(void) override { return m_format.ToString(); }

	//-----------------------------------------------------------------------
	// Fields

	static initonly	LoggingFormat None			= LoggingFormat(_Values::None);
	static initonly	LoggingFormat NCSACombined	= LoggingFormat(_Values::NCSACombined);
	static initonly	LoggingFormat NCSACommon	= LoggingFormat(_Values::NCSACommon);

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Parse
	//
	// Attempts to parse a string value back into a specific constants
	static LoggingFormat Parse(String^ value)
	{
		try { return LoggingFormat(static_cast<_Values>(Enum::Parse(_Values::typeid, value, true))); }
		catch(Exception^) { return LoggingFormat::None; }
	}

private:

	// PRIVATE CONSTRUCTOR
	LoggingFormat(_Values format) : m_format(format) {}

	//-----------------------------------------------------------------------
	// Member Variables
	
	initonly _Values			m_format;		// Formatting code
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif		// __LOGGINGFORMAT_H_