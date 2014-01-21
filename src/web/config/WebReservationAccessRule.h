//---------------------------------------------------------------------------
// WebReservationAccessRule Declarations
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

#ifndef __WEBRESERVATIONACCESSRULE_H_
#define __WEBRESERVATIONACCESSRULE_H_
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
// Class WebReservationAccessRule
//
// WebReservationAccessRule implements the access rule class for a reservation
// (Note: totally ripped off from MutexAccessRule class in .NET 2.0)
//---------------------------------------------------------------------------

public ref class WebReservationAccessRule sealed : public AccessRule
{
public:

	// PUBLIC CONSTRUCTOR
	WebReservationAccessRule(Principal::IdentityReference^ identity, WebReservationRights rights, 
		AccessControl::AccessControlType type) : AccessRule(identity, static_cast<int>(rights), false, 
		AccessControl::InheritanceFlags::None, AccessControl::PropagationFlags::None, type) {}

	// PUBLIC CONSTRUCTOR
	WebReservationAccessRule(String^ identity, WebReservationRights rights, AccessControl::AccessControlType type) :
		AccessRule(gcnew NTAccount(identity), static_cast<int>(rights), false, AccessControl::InheritanceFlags::None, 
		AccessControl::PropagationFlags::None, type) {}

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
	WebReservationAccessRule(Principal::IdentityReference^ identity, int rights, bool isInherited,
		AccessControl::InheritanceFlags inheritanceFlags, AccessControl::PropagationFlags propagationFlags,
		AccessControl::AccessControlType type) : AccessRule(identity, rights, isInherited, inheritanceFlags,
		propagationFlags, type) {}
};

//---------------------------------------------------------------------------

END_NAMESPACE(config)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __WEBRESERVATIONACCESSRULE_H_