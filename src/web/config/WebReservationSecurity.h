//---------------------------------------------------------------------------
// WebReservationSecurity Declarations
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

#ifndef __WEBRESERVATIONSECURITY_H_
#define __WEBRESERVATIONSECURITY_H_
#pragma once

#include "WebReservationAccessRule.h"		// Include WebReservationAccessRule decls
#include "WebReservationAuditRule.h"		// Include WebReservationAuditRule decls
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
// Class WebReservationSecurity
//
// WebReservationSecurity implements the access control for a reservation
//---------------------------------------------------------------------------

public ref class WebReservationSecurity sealed : public NativeObjectSecurity
{
public:

	// PUBLIC CONSTRUCTORS
	WebReservationSecurity() : NativeObjectSecurity(false, ResourceType::ProviderDefined) 
		{ __super::SetSecurityDescriptorSddlForm("D:(A;;GX;;;WD)", AccessControlSections::Access); }
	WebReservationSecurity(String^ sddl) : NativeObjectSecurity(false, ResourceType::ProviderDefined) 
		{ __super::SetSecurityDescriptorSddlForm(sddl, AccessControlSections::Access); }

	//-----------------------------------------------------------------------
	// Member Functions

	// AccessRuleFactory
	//
	// Initializes a new instance of the AccessRule class with the specified values
	virtual AccessRule^ AccessRuleFactory(IdentityReference^ identityReference, int accessMask, 
		bool isInherited, InheritanceFlags inheritanceFlags, PropagationFlags propagationFlags, 
		AccessControlType type) override
	{
		return gcnew WebReservationAccessRule(identityReference, accessMask, isInherited, inheritanceFlags, propagationFlags, type);
	}
	
	// AccessRuleFactory
	//
	// Initializes a new instance of the AccessRule class with the specified values
	AccessRule^ AccessRuleFactory(IdentityReference^ identityReference, WebReservationRights accessMask, 
		bool isInherited, InheritanceFlags inheritanceFlags, PropagationFlags propagationFlags, 
		AccessControlType type)
	{
		return gcnew WebReservationAccessRule(identityReference, static_cast<int>(accessMask), isInherited, inheritanceFlags, propagationFlags, type);
	}
	
	// AddAccessRule
	//
	// Searches for a matching access control rule with which the new rule can be merged
	void AddAccessRule(WebReservationAccessRule^ rule)
	{
		__super::AddAccessRule(rule);
	}

	// AddAuditRule
	//
	// Searches for a matching Audit control rule with which the new rule can be merged
	void AddAuditRule(WebReservationAuditRule^ rule)
	{
		__super::AddAuditRule(rule);
	}

	// AuditRuleFactory
	//
	// Initializes a new instance of the AuditRule class with the specified values
	virtual AuditRule^ AuditRuleFactory(IdentityReference^ identityReference, int accessMask, 
		bool isInherited, InheritanceFlags inheritanceFlags, PropagationFlags propagationFlags, 
		AuditFlags flags) override
	{
		return gcnew WebReservationAuditRule(identityReference, accessMask, isInherited, inheritanceFlags, propagationFlags, flags);
	}

	// AuditRuleFactory
	//
	// Initializes a new instance of the AuditRule class with the specified values
	AuditRule^ AuditRuleFactory(IdentityReference^ identityReference, WebReservationRights accessMask, 
		bool isInherited, InheritanceFlags inheritanceFlags, PropagationFlags propagationFlags, 
		AuditFlags flags)
	{
		return gcnew WebReservationAuditRule(identityReference, static_cast<int>(accessMask), isInherited, inheritanceFlags, propagationFlags, flags);
	}

	// RemoveAccessRule
	//
	// The rights contained in the specified access rule are removed
	bool RemoveAccessRule(WebReservationAccessRule^ rule)
	{
		return __super::RemoveAccessRule(rule);
	}

	// RemoveAccessRuleAll
	//
	// The rights contained in the specified access rule are removed
	void RemoveAccessRuleAll(WebReservationAccessRule^ rule)
	{
		__super::RemoveAccessRuleAll(rule);
	}

	// RemoveAccessRuleSpecific
	//
	// The rights contained in the specified access rule are removed
	void RemoveAccessRuleSpecific(WebReservationAccessRule^ rule)
	{
		__super::RemoveAccessRuleSpecific(rule);
	}

	// RemoveAuditRule
	//
	// The rights contained in the specified audit rule are removed
	bool RemoveAuditRule(WebReservationAuditRule^ rule)
	{
		return __super::RemoveAuditRule(rule);
	}

	// RemoveAuditRuleAll
	//
	// The rights contained in the specified audit rule are removed
	void RemoveAuditRuleAll(WebReservationAuditRule^ rule)
	{
		__super::RemoveAuditRuleAll(rule);
	}

	// RemoveAuditRuleSpecific
	//
	// The rights contained in the specified audit rule are removed
	void RemoveAuditRuleSpecific(WebReservationAuditRule^ rule)
	{
		__super::RemoveAuditRuleSpecific(rule);
	}

	// ResetAccessRule
	//
	// Removes all access control rules with the same user as the specified rule
	void ResetAccessRule(WebReservationAccessRule^ rule)
	{
		__super::ResetAccessRule(rule);
	}

	// SetAccessRule
	//
	// Sets the specified access rule
	void SetAccessRule(WebReservationAccessRule^ rule)
	{
		__super::SetAccessRule(rule);
	}

	// SetAuditRule
	//
	// Sets the specified audit rule
	void SetAuditRule(WebReservationAuditRule^ rule)
	{
		__super::SetAuditRule(rule);
	}

	// ToString (Object)
	//
	// Converts this object into a string (for display purposes)
	virtual String^ ToString(void) override;

	//-----------------------------------------------------------------------
	// Properties

	// AccessRightType
	//
	// Gets the Type of the securable object associated with this ObjectSecurity object
	virtual property Type^ AccessRightType
	{
		Type^ get(void) override { return WebReservationRights::typeid; }
	}

	// AccessRuleType
	//
	// Gets the Type of the access rules object associated with this ObjectSecurity object
	virtual property Type^ AccessRuleType
	{
		Type^ get(void) override { return WebReservationAccessRule::typeid; }
	}

	// AccessRightType
	//
	// Gets the Type of the audit rules object associated with this ObjectSecurity object
	virtual property Type^ AuditRuleType
	{
		Type^ get(void) override { return WebReservationAuditRule::typeid; }
	}

internal:

	// COPY CONSTRUCTOR
	WebReservationSecurity(WebReservationSecurity^ rhs) : NativeObjectSecurity(false, ResourceType::ProviderDefined)
		{ __super::SetSecurityDescriptorSddlForm(rhs->Sddl, AccessControlSections::Access); }

	//-----------------------------------------------------------------------
	// Internal Properties

	// Sddl
	//
	// Gets the current SDDL form of the security decriptor, DACL only
	property String^ Sddl
	{
		String^ get(void) { return NativeObjectSecurity::GetSecurityDescriptorSddlForm(AccessControlSections::Access); }
	}
};

//---------------------------------------------------------------------------

END_NAMESPACE(config)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __WEBRESERVATIONSECURITY_H_