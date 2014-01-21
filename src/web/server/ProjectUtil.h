//---------------------------------------------------------------------------
// ProjectUtil Declarations
//
// Zucchini Embedded ASP.NET Web Server
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

#ifndef __PROJECTUTIL_H_
#define __PROJECTUTIL_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::ComponentModel;
using namespace System::Web::Hosting;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Forward Class Declarations
//---------------------------------------------------------------------------

ref class Host;							// Host.h

//---------------------------------------------------------------------------
// Class ProjectUtil (internal)
//
// ProjectUtil wraps up some helpful things used throughout the library. The
// debugging support was put in so that OutputDebugString could be used instead
// of System::Diagnostics::Debug, which was problematic with the domain manager
//---------------------------------------------------------------------------

ref class ProjectUtil abstract sealed
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// FastPtrToStringAnsi
	//
	// Significantly faster implementation of Marshal::PtrToStringAnsi
	static String^ FastPtrToStringAnsi(const char* psz);
	static String^ FastPtrToStringAnsi(const char* psz, size_t cch);

	// FindOpenTcpPort
	//
	// Finds an open TCP port that can be used by the web application
	static int FindOpenTcpPort(void);

	// TryGetHostWithWait
	//
	// Helper function that attempts to get a host instance from an
	// Application Manager and waits a bit before failing outright
	static bool TryGetHostWithWait(ApplicationManager^ manager, String^ appid,
		Host^% host);
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __PROJECTUTIL_H_