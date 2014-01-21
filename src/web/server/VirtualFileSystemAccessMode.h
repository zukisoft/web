//---------------------------------------------------------------------------
// VirtualFileSystemAccessMode Declarations
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

#ifndef __VIRTUALFILESYSTEMACCESSMODE_H_
#define __VIRTUALFILESYSTEMACCESSMODE_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class VirtualFileSystemAccessMode
//
// Defines the available access modes for the virtual file system
//---------------------------------------------------------------------------

[Serializable()]
public value class VirtualFileSystemAccessMode sealed
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
		ReadOnly = 0,
		//ReadWrite = 1,
	};

public:

	//-----------------------------------------------------------------------
	// Overloaded Operators

	bool operator ==(VirtualFileSystemAccessMode rhs) { return rhs.m_mode == this->m_mode; }
	bool operator !=(VirtualFileSystemAccessMode rhs) { return rhs.m_mode != this->m_mode; }

	//-----------------------------------------------------------------------
	// Member Functions

	// ToString (Object)
	//
	// Converts this object instance into it's string representation
	virtual String^ ToString(void) override { return m_mode.ToString(); }

	//-----------------------------------------------------------------------
	// Fields

	static initonly VirtualFileSystemAccessMode ReadOnly	= VirtualFileSystemAccessMode(_Values::ReadOnly);
	//static initonly VirtualFileSystemAccessMode ReadWrite	= VirtualFileSystemAccessMode(_Values::ReadWrite);

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Parse
	//
	// Attempts to parse a string value back into a specific constants
	static VirtualFileSystemAccessMode Parse(String^ value)
	{
		try { return VirtualFileSystemAccessMode(static_cast<_Values>(Enum::Parse(_Values::typeid, value, true))); }
		catch(Exception^) { return VirtualFileSystemAccessMode::ReadOnly; }
	}

private:

	// PRIVATE CONSTRUCTOR
	VirtualFileSystemAccessMode(_Values mode) : m_mode(mode) {}

	//-----------------------------------------------------------------------
	// Member Variables
	
	initonly _Values		m_mode;			// Access mode enumeration value
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif		// __VIRTUALFILESYSTEMACCESSMODE_H_