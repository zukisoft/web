//---------------------------------------------------------------------------
// HashCodeCombiner Declarations
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

#ifndef __HASHCODECOMBINER_H_
#define __HASHCODECOMBINER_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Globalization;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class HashCodeCombiner (internal)
//
// A simple utility class to combine multiple hash codes into a single one
//---------------------------------------------------------------------------

ref class HashCodeCombiner sealed
{
public:

	HashCodeCombiner() : m_hash(0x5555) {}

	//-----------------------------------------------------------------------
	// Member Functions

	// AddGuid
	//
	// Adds a GUID into the hash
	void AddGuid(Guid guid)
	{
		m_hash = ((m_hash << 5) + m_hash) ^ guid.GetHashCode();
	}

	// AddInt32
	//
	// Adds a 32-bit integer into the hash
	void AddInt32(int value)
	{
		m_hash = ((m_hash << 5) + m_hash) ^ value;
	}

	// AddString
	//
	// Adds a string into the hash
	void AddString(String^ value)
	{
		int hash = value->GetHashCode();
		m_hash = ((m_hash << 5) + m_hash) ^ hash;
	}

	// ToString (Object)
	//
	// Converts this object into it's string representation
	virtual String^ ToString(void) override 
	{
		return m_hash.ToString("x", CultureInfo::InvariantCulture);
	}

private:

	//-----------------------------------------------------------------------
	// Member Variables

	__int64					m_hash;		// Combined hash code value
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __HASHCODECOMBINER_H_