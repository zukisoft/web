//---------------------------------------------------------------------------
// WebReservationSecurity Implementation
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

#include "stdafx.h"						// Include project pre-compiled headers
#include "WebReservationSecurity.h"		// Include WebReservationSecurity decls

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(config)

//---------------------------------------------------------------------------
// WebReservationSecurity::ToString
//
// Converts this object into a string representation (for display purposes)
//
// Arguments:
//
//	NONE

String^ WebReservationSecurity::ToString(void)
{
	// TODO: Can fancy this up by looking up the names and accesses
	// rather than throwing the (internal) SDDL string back out
	return this->Sddl;
}

//---------------------------------------------------------------------------

END_NAMESPACE(config)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
