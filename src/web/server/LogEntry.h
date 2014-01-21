//---------------------------------------------------------------------------
// LogEntry Declarations
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

#ifndef __LOGENTRY_H_
#define __LOGENTRY_H_
#pragma once

#include "LoggingFormat.h"				// Include LoggingFormat declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Net;
using namespace System::Text;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class LogEntry (internal)
//
// Class used to generate server log strings from context information
//---------------------------------------------------------------------------

ref class LogEntry sealed
{
public:

	//-----------------------------------------------------------------------
	// Constructor
	//
	// Arguments:
	//
	//	format		- Logging format
	//	request		- HttpListenerRequest from which to seed the entry

	LogEntry(LoggingFormat format, HttpListenerContext^ context);

	//-----------------------------------------------------------------------
	// Member Functions

	// Complete
	//
	// Completes the log entry and returns it as a single string
	String^ Complete(HttpListenerResponse^ response);

private:

	//-----------------------------------------------------------------------
	// Member Variables

	LoggingFormat			m_format;		// Logging format information
	String^					m_baseEntry;	// Base log entry (in ctor)
	String^					m_referrer;		// Request referrer string
	String^					m_userAgent;	// Request user agent string

	initonly static array<__wchar_t>^ s_splitSemis = { ';' };	// For strings
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif		// __LOGENTRY_H_