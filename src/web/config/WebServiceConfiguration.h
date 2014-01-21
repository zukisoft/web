//---------------------------------------------------------------------------
// WebServiceConfiguration Declarations
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

#ifndef __WEBSERVICECONFIGURATION_H_
#define __WEBSERVICECONFIGURATION_H_
#pragma once

#include "HttpInit.h"							// Include HttpInit declarations
#include "ReservationSecurityInfo.h"			// Include ReservationSecurityInfo decls
#include "WebReservation.h"						// Include WebReservation declarations
#include "WebServiceCertificateCollection.h"	// Include WebServiceCertificateCollection
#include "WebServiceListenerCollection.h"		// Include WebServiceListenerCollection
#include "WebServiceReservationCollection.h"	// Include WebServiceReservationCollection

#pragma warning(push, 4)						// Enable maximum compiler warnings

using namespace System;
using namespace System::Security;
using namespace System::Security::AccessControl;
using namespace System::Windows::Forms;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(config)

//---------------------------------------------------------------------------
// Class WebServiceConfiguration (static)
//
// WebServiceConfiguration implements a static collection of methods that
// can be used to both view and change the HTTP service configuration of the
// local system.  Matching zuki.web.server, this only exposes configuration
// information relevant to HTTP.SYS version 1 (Windows XP SP2 and higher)
//---------------------------------------------------------------------------

public ref class WebServiceConfiguration abstract sealed
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// EditWebReservationSecurity
	//
	// Allows the use of the built-in Windows security editor property sheet
	// to manipulate the contents of a WebReservationSecurity object
	static void EditWebReservationSecurity(Form^ form, WebReservation^ reservation)
		{ EditWebReservationSecurity(form->Handle, reservation); }
	static void EditWebReservationSecurity(IntPtr hwnd, WebReservation^ reservation);

	//-----------------------------------------------------------------------
	// Properties

	// Certificates
	//
	// Accesses the SSL certificate management collection
	static property WebServiceCertificateCollection^ Certificates
	{
		WebServiceCertificateCollection^ get(void) { return s_certificates; }
	}

	// Listeners
	//
	// Accesses the listener management collection
	static property WebServiceListenerCollection^ Listeners
	{
		WebServiceListenerCollection^ get(void) { return s_listeners; }
	}

	// Reservations
	//
	// Accesses the reservation management collection
	static property WebServiceReservationCollection^ Reservations
	{
		WebServiceReservationCollection^ get(void) { return s_reservations; }
	}

private:

	// STATIC CONSTRUCTOR
	static WebServiceConfiguration() 
	{
		// The first thing we need to do is initialize the HTTP API
		s_init = gcnew HttpInit(HTTP_INITIALIZE_CONFIG);

		s_certificates = gcnew WebServiceCertificateCollection();
		s_listeners = gcnew WebServiceListenerCollection();
		s_reservations = gcnew WebServiceReservationCollection();
	}

	//-----------------------------------------------------------------------
	// Member Variables

	static HttpInit^						s_init;			// API initializer
	static WebServiceCertificateCollection^	s_certificates;	// Contained collection
	static WebServiceListenerCollection^	s_listeners;	// Contained collection
	static WebServiceReservationCollection^	s_reservations;	// Contained collection
};

//---------------------------------------------------------------------------

END_NAMESPACE(config)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __WEBSERVICECONFIGURATION_H_