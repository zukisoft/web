//---------------------------------------------------------------------------
// LogEntry Implementation
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

#include "stdafx.h"						// Include project pre-compiled headers
#include "LogEntry.h"					// Include LogEntry declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// LogEntry Constructor
//
// Arguments:
//
//	format		- Format to be used for the log entry
//	context		- HttpListenerContext to generate the log for

LogEntry::LogEntry(LoggingFormat format, HttpListenerContext^ context) : m_format(format)
{
	Version^ ver = context->Request->ProtocolVersion;		// For formatting

	// host rfc931 username date:time request
	//
	// {0}	- Client IP address
	// {1}	- User Name if available
	// {2}	- Date/Time of the request in Local time
	// {3}	- Verb
	// {4}	- Uri
	// {5}	- Protocol version

	m_baseEntry = String::Format("{0} - {1} [{2}] \"{3} {4} {5}\"",
		context->Request->RemoteEndPoint->Address->ToString(),
		(context->Request->IsAuthenticated) ? context->User->Identity->Name : "-",
		DateTime::Now.ToString("dd/MMM/yyyy:hh:mm:ss zzz"),
		context->Request->HttpMethod,
		context->Request->Url->LocalPath,
		String::Format("HTTP/{0}.{1}", ver->Major, ver->Minor)
		);

	// Currently the only format that needs the REFERRER and USER-AGENT is the
	// NCSA Combined log format.  Only store those if we're going to need them
	// when the final log string is generated in Complete()

	if(format != LoggingFormat::NCSACombined) return;

	m_referrer = (context->Request->UrlReferrer != nullptr) ? context->Request->UrlReferrer->AbsoluteUri : 
		String::Empty;
	m_userAgent = context->Request->UserAgent;
}

//---------------------------------------------------------------------------
// LogEntry::Complete
//
// Generates a log entry for the specified context
//
// Arguments:
//
//	response	- HttpListenerResponse (after the response is complete)

String^ LogEntry::Complete(HttpListenerResponse^ response)
{
	// The first (request) part of the log entry is already complete.  All we
	// need to do is add the response information and put all the pieces together

	// NCSA COMMON
	//
	// host rfc931 username date:time request statuscode bytes
	//
	// {0}	- Common request portion of the log entry
	// {1}  - HTTP status code
	// {2}	- Bytes sent in response
	
	if(m_format == LoggingFormat::NCSACommon) {

		return String::Format("{0} {1} {2}",
			m_baseEntry,
			response->StatusCode,
			response->ContentLength64
			);
	}

	// NCSA COMBINED (sans cookies)
	//
	// host rfc931 username date:time request statuscode bytes referrer user_agent
	//
	// {0}	- Common request portion of the log entry
	// {1}	- HTTP status code
	// {2}	- Bytes sent in response
	// {3}	- Referring URL
	// {4}	- Client's user agent (browser)

	else if(m_format == LoggingFormat::NCSACombined) {

		return String::Format("{0} {1} {2} \"{3}\" \"{4}\"",
			m_baseEntry,
			response->StatusCode,
			response->ContentLength64,
			m_referrer,
			m_userAgent
			);
	}

	return String::Empty;			// <--- Unknown logging format specified
}

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
