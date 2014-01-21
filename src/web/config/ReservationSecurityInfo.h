//---------------------------------------------------------------------------
// ReservationSecurityInfo Declarations
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

#ifndef __RESERVATIONSECURITYINFO_H_
#define __RESERVATIONSECURITYINFO_H_
#pragma once

#pragma warning(push, 4)					// Enable maximum compiler warnings

using namespace System;
using namespace System::Runtime::InteropServices;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(config)

//---------------------------------------------------------------------------
// Class ReservationSecurityInfo
//
// Unmanaged class that implements the ISecurityInformation interface, allowing
// us to use the built-in Windows ACL editor
//---------------------------------------------------------------------------

class ReservationSecurityInfo : public ISecurityInformation
{
public:

	// CONSTRUCTOR
	explicit ReservationSecurityInfo(String^ sddl, String^ objName = nullptr);

	//-----------------------------------------------------------------------
	// IUnknown

	// AddRef
	//
	// Increments the object reference count
	STDMETHOD_(ULONG, AddRef)(void);

	// QueryInterface
	//
	// Requests a pointer to an implemented interface
	STDMETHOD(QueryInterface)(REFIID riid, void **ppv);

	// Release
	//
	// Decrements the object reference count
	STDMETHOD_(ULONG, Release)(void);

	//-----------------------------------------------------------------------
	// ISecurityInformation

	// GetAccessRights
	//
	// Requests information about the access rights that can be controlled for a securable object
	STDMETHOD(GetAccessRights)(const GUID *pguidObjectType, DWORD dwFlags, PSI_ACCESS *ppAccess,
		ULONG *pcAccesses, ULONG *piDefaultAccess);

	// GetInheritTypes
	//
	// Requests information about how ACEs can be inherited by child objects
	STDMETHOD(GetInheritTypes)(PSI_INHERIT_TYPE *ppInheritTypes, ULONG *pcInheritTypes);

	// GetObjectInformation
	//
	// Requests information that the access control editor uses to initialize its pages
	STDMETHOD(GetObjectInformation)(PSI_OBJECT_INFO pObjectInfo);

	// GetSecurity
	//
	// Requests a security descriptor for the securable object whose security descriptor is being edited
	STDMETHOD(GetSecurity)(SECURITY_INFORMATION requestedInformation, PSECURITY_DESCRIPTOR *ppSecurityDescriptor,
		BOOL fDefault);

	// MapGeneric
	//
	// Requests that the generic access rights in an access mask be mapped
	STDMETHOD(MapGeneric)(const GUID *pguidObjectType, UCHAR *pAceFlags, ACCESS_MASK *pMask);

	// PropertySheetPageCallback
	//
	// Notifies that the property sheet is being created or destroyed
	STDMETHOD(PropertySheetPageCallback)(HWND hwnd, UINT uMsg, SI_PAGE_TYPE uPage);

	// SetSecurity
	//
	// Provides a security descriptor containing the security information the user wants to apply 
	STDMETHOD(SetSecurity)(SECURITY_INFORMATION securityInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor);

	//-----------------------------------------------------------------------
	// Properties

	// Sddl
	//
	// Gets the managed SDDL string as set by the property sheet editor
	__declspec(property(get=GetSddl)) String^ Sddl;

	//-----------------------------------------------------------------------
	// Property Accessors

	String^ GetSddl(void) const { return m_sddl; }

private:

	// PRIVATE DESTRUCTOR
	~ReservationSecurityInfo();

	// DISABLE COPY CTOR AND ASSIGNMENT OPERATOR
	ReservationSecurityInfo(const ReservationSecurityInfo &rhs);
	ReservationSecurityInfo& operator=(const ReservationSecurityInfo &rhs);

	//-----------------------------------------------------------------------
	// Member Variables

	volatile long			m_cRef;				// COM instance reference count
	gcroot<String^>			m_sddl;				// Current SDDL string for object
	LPWSTR					m_pwszObjName;		// Object name string (allocated)
};

//---------------------------------------------------------------------------

END_NAMESPACE(config)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __RESERVATIONSECURITYINFO_H_