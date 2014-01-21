//---------------------------------------------------------------------------
// WebReservation Declarations
//
// Zucchini Service Configuration Library
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

#ifndef __WEBRESERVATION_H_
#define __WEBRESERVATION_H_
#pragma once

#include "WebReservationSecurity.h"			// Include WebReservationSecurity decls

#pragma warning(push, 4)					// Enable maximum compiler warnings

using namespace System;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(config)

//---------------------------------------------------------------------------
// Class WebReservation
//
// WebReservation implements a wrapper around an HTTP namespace reservation,
// which consists of a URL prefix and a discretionary ACL
//---------------------------------------------------------------------------

public ref class WebReservation sealed
{
public:

	// CONSTRUCTORS
	WebReservation(String^ prefix) : m_prefix(CheckPrefix(prefix)), m_security(gcnew WebReservationSecurity()) {}
	WebReservation(String^ prefix, String^ sddl) : m_prefix(CheckPrefix(prefix)), m_security(gcnew WebReservationSecurity(sddl)) {}
	WebReservation(String^ prefix, WebReservationSecurity^ security) : m_prefix(CheckPrefix(prefix)), m_security(gcnew WebReservationSecurity(security)) {}

	//-----------------------------------------------------------------------
	// Member Functions

	// ToString (Object)
	//
	// Converts this object into a string (for display purposes)
	virtual String^ ToString(void) override
	{
		// Nothing overly fancy, at least not right now anyway
		return String::Format("Prefix={0}   Security={1}", m_prefix, m_security->ToString());
	}

	//-----------------------------------------------------------------------
	// Properties

	// Prefix
	//
	// Exposes the contained namespace prefix string
	property String^ Prefix
	{
		String^ get(void) { return m_prefix; }
	}

	// Security
	//
	// Exposes the contained WebReservationSecurity object
	property WebReservationSecurity^ Security
	{
		WebReservationSecurity^ get(void) { return m_security; }
	}

internal:

	// COPY CONSTRUCTOR
	WebReservation(WebReservation^ rhs) : m_prefix(rhs->Prefix), 
		m_security(gcnew WebReservationSecurity(rhs->Security)) {}

private:

	//-----------------------------------------------------------------------
	// Private Member Functions

	// CheckPrefix
	//
	// Checks a prefix string and massages it if possible
	static String^ CheckPrefix(String^ prefix);

	//-----------------------------------------------------------------------
	// Member Variables

	String^						m_prefix;		// Contained prefix string
	WebReservationSecurity^		m_security;		// Contained security object
};

//---------------------------------------------------------------------------

END_NAMESPACE(config)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __WEBRESERVATION_H_