//---------------------------------------------------------------------------
// RequestUri Implementation
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
#include "RequestUri.h"					// Include RequestUri declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// RequestUri Constructor
//
// Arguments:
//
//	context		- HttpListenerContext for this request

RequestUri::RequestUri(HttpListenerContext^ context) : m_baseUri(context->Request->Url)
{
	if(context == nullptr) throw gcnew ArgumentNullException();
	ParseUriComponents(m_baseUri);
}

//---------------------------------------------------------------------------
// RequestUri::AppendPath
//
// Appends a new document/directory onto the end of the URI and reparses
// everything to properly indicate it.  This is used to tack on a default
// document by the application host
//
// Arguments:
//
//	path		- Path to be added to the URI

void RequestUri::AppendPath(String^ path)
{
	StringBuilder^			builder;		// Used to generate the new URI

	builder = gcnew StringBuilder(m_filePath);		// Start with file path

	// Another case of 'looks worse than it is' here.  All we're doing is
	// reconstructing the URL with a new path wedged in-between the file
	// path and the URL extension path information ...

	if(!m_filePath->EndsWith("/")) builder->Append("/");
	builder->Append(path);
	if(m_pathInfo->Length > 0) builder->Append(m_pathInfo);
	if(m_queryString->Length > 0) builder->Append("?" + m_queryString);

	m_baseUri = gcnew Uri(m_baseUri, builder->ToString()->Replace("//", "/"));
	ParseUriComponents(m_baseUri);
}

//---------------------------------------------------------------------------
// RequestUri::ParseUriComponents (private)
//
// Reparses all of the URI components after a change to the URI has been 
// made, or on the initial URI set in the constructor
//
// Arguments:
//
//	uri		- URI to be parsed into component pieces

void RequestUri::ParseUriComponents(Uri^ uri)
{
	int						lastDot;		// Last dot in absolute path
	int						lastSlash;		// Last slash in absolute path

	// The URI path, raw URL and query strings are simple enough to get

	m_uriPath = HttpUtility::UrlDecode(uri->AbsolutePath);
	m_rawUrl = HttpUtility::UrlDecode(uri->PathAndQuery);
	m_queryString = HttpUtility::UrlDecode(uri->Query->TrimStart(s_trimQuery));

	m_filePath = m_uriPath;					// Assume no path information
	m_pathInfo = String::Empty;				// Assume no path information

	// Using the logic from the Cassini web server sample, determine if
	// a URL extension is present, and if it is reset the file path and
	// path info members appropriately ...

	lastDot = m_uriPath->LastIndexOf('.');	
	lastSlash = m_uriPath->LastIndexOf('/');

    if((lastDot >= 0) && (lastSlash >= 0) && (lastDot < lastSlash)) {

		int ipi = m_uriPath->IndexOf('/', lastDot);
		m_filePath = m_uriPath->Substring(0, ipi);
		m_pathInfo = m_uriPath->Substring(ipi);
	}

	m_baseUri = uri;					// Save for reparsing as necessary
}

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
