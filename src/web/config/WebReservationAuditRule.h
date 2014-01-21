//---------------------------------------------------------------------------
// WebReservationAuditRule Declarations
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

#ifndef __WEBRESERVATIONAUDITRULE_H_
#define __WEBRESERVATIONAUDITRULE_H_
#pragma once

#include "WebReservationRights.h"			// Include WebReservationRights decls

#pragma warning(push, 4)					// Enable maximum compiler warnings

using namespace System;
using namespace System::Security;
using namespace System::Security::AccessControl;
using namespace System::Security::Principal;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(config)

//---------------------------------------------------------------------------
// Class WebReservationAuditRule
//
// WebReservationAuditRule implements the audit rule class for a reservation
// (Note: totally ripped off from MutexAuditRule class in .NET 2.0)
//---------------------------------------------------------------------------

public ref class WebReservationAuditRule sealed : public AuditRule
{
public:

	// PUBLIC CONSTRUCTOR
	WebReservationAuditRule(Principal::IdentityReference^ identity, WebReservationRights rights, 
		AccessControl::AuditFlags flags) : AuditRule(identity, static_cast<int>(rights), false, 
		AccessControl::InheritanceFlags::None, AccessControl::PropagationFlags::None, flags) {}

	//-----------------------------------------------------------------------
	// Properties

	// AccessMask
	//
	// Gets the access mask for this rule
	property WebReservationRights AccessMask
	{
		WebReservationRights get(void) new { return static_cast<WebReservationRights>(__super::AccessMask); }
	}

internal:

	// INTERNAL CONSTRUCTOR

	WebReservationAuditRule(Principal::IdentityReference^ identity, int rights, bool isInherited, 
		AccessControl::InheritanceFlags inheritanceFlags, AccessControl::PropagationFlags propagationFlags, 
		AccessControl::AuditFlags flags) : AuditRule(identity, rights, isInherited, inheritanceFlags, 
		propagationFlags, flags) {}
};

//---------------------------------------------------------------------------

END_NAMESPACE(config)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __WEBRESERVATIONAUDITRULE_H_