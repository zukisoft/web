//---------------------------------------------------------------------------
// WebServiceConfiguration Implementation
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
#include "WebServiceConfiguration.h"	// Include WebServiceConfiguration decls

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(config)

//---------------------------------------------------------------------------
// WebServiceConfiguration::EditWebReservationSecurity
//
// Allows a user to modify the contents of a WebReservation's DACL using the
// Windows ACL editor property sheet
//
// Arguments:
//
//	hwnd			- Parent window handle
//	reservation		- The WebReservation instance to modify the DACL on

void WebServiceConfiguration::EditWebReservationSecurity(IntPtr hwnd, WebReservation^ reservation)
{
	ReservationSecurityInfo*	pInfoObject;	// ISecurityInformation implementation

	pInfoObject = new ReservationSecurityInfo(reservation->Security->Sddl, reservation->Prefix);
	if(!pInfoObject) throw gcnew OutOfMemoryException();

	try {
	
		// Invoke the standard Windows ACL editor, and reset the reservation SDDL with the
		// string as it stands after the editor has been dismissed by the user

		EditSecurity(reinterpret_cast<HWND>(hwnd.ToPointer()), pInfoObject);
		reservation->Security->SetSecurityDescriptorSddlForm(pInfoObject->Sddl, AccessControlSections::Access);
	}
	
	finally { pInfoObject->Release(); }
}

//---------------------------------------------------------------------------

END_NAMESPACE(config)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
