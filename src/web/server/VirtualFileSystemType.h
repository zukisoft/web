//---------------------------------------------------------------------------
// VirtualFileSystemType Declarations
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

#ifndef __VIRTUALFILESYSTEMTYPE_H_
#define __VIRTUALFILESYSTEMTYPE_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class VirtualFileSystemType
//
// Defines the available types of virtual file systems supported by the
// embedded web server
//---------------------------------------------------------------------------

[Serializable()]
public value class VirtualFileSystemType sealed
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
		StructuredStorage = 1,
		//VirtualHardDrive = 2,
	};

public:

	//-----------------------------------------------------------------------
	// Overloaded Operators

	bool operator ==(VirtualFileSystemType rhs) { return rhs.m_type == this->m_type; }
	bool operator !=(VirtualFileSystemType rhs) { return rhs.m_type != this->m_type; }

	//-----------------------------------------------------------------------
	// Member Functions

	// ToString (Object)
	//
	// Converts this object instance into it's string representation
	virtual String^ ToString(void) override { return m_type.ToString(); }

	//-----------------------------------------------------------------------
	// Fields

	static initonly VirtualFileSystemType None				= VirtualFileSystemType(_Values::None);
	static initonly VirtualFileSystemType StructuredStorage	= VirtualFileSystemType(_Values::StructuredStorage);
	//static initonly VirtualFileSystemType VirtualHardDrive	= VirtualFileSystemType(_Values::VirtualHardDrive);

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Parse
	//
	// Attempts to parse a string value back into a specific constants
	static VirtualFileSystemType Parse(String^ value)
	{
		try { return VirtualFileSystemType(static_cast<_Values>(Enum::Parse(_Values::typeid, value, true))); }
		catch(Exception^) { return VirtualFileSystemType::None; }
	}

private:

	// PRIVATE CONSTRUCTOR
	VirtualFileSystemType(_Values type) : m_type(type) {}

	//-----------------------------------------------------------------------
	// Member Variables
	
	initonly _Values			m_type;			// Type enumeration value
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif		// __VIRTUALFILESYSTEMTYPE_H_