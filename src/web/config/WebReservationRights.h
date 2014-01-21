//---------------------------------------------------------------------------
// WebReservationRights Declarations
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

#ifndef __WEBRESERVATIONRIGHTS_H_
#define __WEBRESERVATIONRIGHTS_H_
#pragma once

#pragma warning(push, 4)					// Enable maximum compiler warnings

using namespace System;
using namespace System::Security;
using namespace System::Security::AccessControl;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(config)

//---------------------------------------------------------------------------
// Enum WebReservationRights
//
// Defines the various rights available for web reservations
//---------------------------------------------------------------------------

public enum class WebReservationRights
{
	RegisterListener	= GENERIC_EXECUTE,		// Ability to register
	DelegateOwnership	= GENERIC_WRITE,		// Ability to delegate
};

//---------------------------------------------------------------------------

END_NAMESPACE(config)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __WEBRESERVATIONRIGHTS_H_