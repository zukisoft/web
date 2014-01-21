//---------------------------------------------------------------------------
// ReservationSecurityInfo Implementation
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
#include "ReservationSecurityInfo.h"	// Include ReservationSecurityInfo decls

#pragma warning(push, 4)				// Enable maximum compiler warnings
#pragma warning(disable:4100)			// "unreferenced formal parameter"

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(config)

//---------------------------------------------------------------------------
// Global Variables
//---------------------------------------------------------------------------

// DEFAULT_SDDL
//
// Defines the SDDL used to define a "default" ACL for the reservation
const WCHAR DEFAULT_SDDL[] = L"D:(A;;GX;;;WD)";

// g_rgAccessRights
//
// Defines the access rights used in the security editor property sheet
SI_ACCESS g_rgAccessRights[] = {

	{ &GUID_NULL, GENERIC_EXECUTE,	L"Register Listener",	SI_ACCESS_GENERAL },
	{ &GUID_NULL, GENERIC_WRITE,	L"Delegate Ownership",	SI_ACCESS_GENERAL },
};

// g_cAccessRights
//
// Number of access rights defined for the security editor property sheet
const DWORD g_cAccessRights = sizeof(g_rgAccessRights) / sizeof(SI_ACCESS);

//---------------------------------------------------------------------------
// ReservationSecurityInfo Constructor
//
// Arguments:
//
//	sddl		- SDDL string describing the ACL to be edited
//	objName		- Name of the object being edited

ReservationSecurityInfo::ReservationSecurityInfo(String^ sddl, String^ objName) : m_cRef(1)
{
	m_sddl = gcnew String(sddl);
	m_pwszObjName = reinterpret_cast<LPWSTR>(Marshal::StringToHGlobalUni(objName).ToPointer());
}

//---------------------------------------------------------------------------
// ReservationSecurityInfo Destructor

ReservationSecurityInfo::~ReservationSecurityInfo()
{
	if(m_pwszObjName) Marshal::FreeHGlobal(IntPtr(m_pwszObjName));
}

//---------------------------------------------------------------------------
// ReservationSecurityInfo::AddRef
//
// Increments the object reference count
//
// Arguments:
//
//	NONE

STDMETHODIMP_(ULONG) ReservationSecurityInfo::AddRef(void)
{
	return InterlockedIncrement(&m_cRef);	// Increment the refcount
}

//---------------------------------------------------------------------------
// ReservationSecurityInfo::GetAccessRights
//
// Requests information about the access rights that can be controlled
//
// Arguments:
//
//	pguidObjectType	- GUID identifying the object for which rights are being requested
//	dwFlags			- Bit flags that indicate the property page being initialized
//	ppAccess		- Pointer to the array of SI_ACCESS structures
//	pcAccesses		- Number of SI_ACCESS elements pointed to
//	piDefaultAccess	- Indicates the index of the entry that contains the default rights

STDMETHODIMP ReservationSecurityInfo::GetAccessRights(const GUID *pguidObjectType, 
	DWORD dwFlags, PSI_ACCESS *ppAccess, ULONG *pcAccesses, ULONG *piDefaultAccess)
{
	*ppAccess = g_rgAccessRights;		// Set the pointer to the SI_ACCESS array
	*pcAccesses = g_cAccessRights;		// Set the size of the SI_ACCESS array
	*piDefaultAccess = 0;				// Set the "default" access permission
	
	return S_OK;
}

//---------------------------------------------------------------------------
// ReservationSecurityInfo::GetInheritTypes
//
// Requests information about how ACEs can be inherited by child objects
//
// Arguments:
//
//	ppInheritType	- Pointer to an array of SI_INHERIT_TYPE structures
//	pcInheritTypes	- Number of SI_INHERIT_TYPE elements pointed to

STDMETHODIMP ReservationSecurityInfo::GetInheritTypes(PSI_INHERIT_TYPE *ppInheritType, 
	ULONG *pcInheritTypes)
{
	*ppInheritType = NULL;				// No inheritance information
	*pcInheritTypes = 0;				// No inheritance information

	return S_OK;
}

//---------------------------------------------------------------------------
// ReservationSecurityInfo::GetObjectInformation
//
// Requests information that the access control editor uses to initialize its pages
//
// Arguments:
//
//	pObjectInfo		- Pointer to the SI_OBJECT_INFO structure to fill out

STDMETHODIMP ReservationSecurityInfo::GetObjectInformation(PSI_OBJECT_INFO pObjectInfo)
{
	ZeroMemory(pObjectInfo, sizeof(SI_OBJECT_INFO));		// Initialize structure

	// Set up the SI_OBJECT_INFO structure with the information to display.  There
	// is absolutely no value in allowing anything but the basic DACL settings ...

	pObjectInfo->dwFlags = SI_EDIT_PERMS | SI_NO_ACL_PROTECT | SI_NO_TREE_APPLY;
	pObjectInfo->pszObjectName = m_pwszObjName;

	return S_OK;
}

//---------------------------------------------------------------------------
// ReservationSecurityInfo::GetSecurity
//
// Requests a security descriptor for the securable object
//
// Arguments:
//
//	requestedInformation	- Indicates the parts of the security descriptor being requested
//	ppSecurityDescriptor	- Pointer to the object's security descriptor
//	fDefault				- Flag to return a default security descriptor or not

STDMETHODIMP ReservationSecurityInfo::GetSecurity(SECURITY_INFORMATION requestedInformation, 
	PSECURITY_DESCRIPTOR *ppSecurityDescriptor, BOOL fDefault)
{
	PinnedStringPtr			pinSddl;			// Pinned managed SDDL string

	pinSddl = PtrToStringChars(m_sddl);			// Pin down the managed string

	// Use the extremely handy conversion API to convert the SDDL back into binary.
	// This function even uses LocalAlloc(), which is what GetSecurity is expecting

	if(!ConvertStringSecurityDescriptorToSecurityDescriptor((fDefault) ? DEFAULT_SDDL : pinSddl,
		SDDL_REVISION_1, ppSecurityDescriptor, NULL)) return HRESULT_FROM_WIN32(GetLastError());

	return S_OK;
}

//---------------------------------------------------------------------------
// ReservationSecurityInfo::MapGeneric
//
// Requests that the generic access rights in an access mask be mapped
//
// Arguments:
//
//	pguidObjectType	- GUID that identifies the type of object to which the mask applies
//	pAceFlags		- AceFlags from the ACE whose access mask is being mapped
//	pMask			- Access mask that contains the generic access rights to map

STDMETHODIMP ReservationSecurityInfo::MapGeneric(const GUID *pguidObjectType, UCHAR *pAceFlags, 
	ACCESS_MASK *pMask)
{
	// Since we're already working with GENERIC masks, we can't call into MapGenericMask,
	// but that's OK because the only thing that needs to be mapped is GENERIC_ALL to
	// break it back up into GENERIC_EXECUTE and GENERIC_WRITE masks. This really only
	// helps on the way IN, but setting "GXGW" instead of "GA" in the SDDL DACL on the way
	// out isn't the end of the world, until Microsoft adds something for "GR" and bones me.

	if(*pMask == GENERIC_ALL) *pMask = (GENERIC_EXECUTE | GENERIC_WRITE);
	return S_OK;
}

//---------------------------------------------------------------------------
// ReservationSecurityInfo::PropertySheetPageCallback
//
// Notifies that the property sheet is being created or destroyed
//
// Arguments:
//
//	hwnd		- Handle to the property page dialog box
//	uMsg		- Message being received
//	uPage		- Type of access control page being created/destroyed

STDMETHODIMP ReservationSecurityInfo::PropertySheetPageCallback(HWND hwnd, UINT uMsg, 
	SI_PAGE_TYPE uPage)
{
	return S_OK;				// <-- We're not interested in anything here
}

//---------------------------------------------------------------------------
// ReservationSecurityInfo::QueryInterface
//
// Requests a COM interface pointer from the object instance
//
// Arguments:
//
//	riid		- Requested interface IID
//	ppv			- On success, contains the requested interface pointer

STDMETHODIMP ReservationSecurityInfo::QueryInterface(REFIID riid, void **ppv)
{
	if(!ppv) return E_POINTER;				// Invalid [out] pointer
	*ppv = NULL;							// Initialize [out] pointer

	// This object only implements ISecurityInformation

	if((riid == IID_IUnknown) || (riid == IID_ISecurityInformation))
		*ppv = static_cast<ISecurityInformation*>(this);
	else return E_NOINTERFACE;

	// Increment the object's reference count before returning
	
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

//---------------------------------------------------------------------------
// ReservationSecurityInfo::Release
//
// Decrements the object reference count
//
// Arguments:
//
//	NONE

STDMETHODIMP_(ULONG) ReservationSecurityInfo::Release(void)
{
	ULONG ref = InterlockedDecrement(&m_cRef);		// Decrement refcount
	if(ref == 0) delete this;						// Release on zero
	return ref;										// Return refcount
}

//---------------------------------------------------------------------------
// ReservationSecurityInfo::SetSecurity
//
// Provides a security descriptor containing the security information
//
// Arguments:
//
//	securityInformation	- Indicates the parts of the security descriptor to set
//	pSecurityDescriptor	- Security descriptor containing the new security information

STDMETHODIMP ReservationSecurityInfo::SetSecurity(SECURITY_INFORMATION securityInformation, 
	PSECURITY_DESCRIPTOR pSecurityDescriptor)
{
	LPWSTR				pwszSddl = NULL;		// Converted security descriptor

	// Clear out any "AR", "AI" or "P" flags that would end up in our SDDL by clearing
	// them from the source security descriptor (sneaky and kinda cheesy). In particular,
	// I found that the "AR" flag kept getting set when it shouldn't be.

	SetSecurityDescriptorControl(pSecurityDescriptor, SE_DACL_PROTECTED |
		SE_DACL_AUTO_INHERIT_REQ | SE_DACL_AUTO_INHERITED, 0);

	// Use the extremely handy conversion routine to bring the SD back into SDDL.
	// We don't care about anything except the DACL, so don't bother with the rest

	if(!ConvertSecurityDescriptorToStringSecurityDescriptor(pSecurityDescriptor,
		SDDL_REVISION_1, DACL_SECURITY_INFORMATION, &pwszSddl, NULL)) 
		return HRESULT_FROM_WIN32(GetLastError());

	m_sddl = gcnew String(pwszSddl);		// Convert into a managed string
	if(pwszSddl) LocalFree(pwszSddl);		// Release the allocated buffer

	return S_OK;
}

//---------------------------------------------------------------------------

END_NAMESPACE(config)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
