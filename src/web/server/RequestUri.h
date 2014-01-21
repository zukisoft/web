//---------------------------------------------------------------------------
// RequestUri Declarations
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

#ifndef __REQUESTURI_H_
#define __REQUESTURI_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Net;
using namespace System::Web;
using namespace System::Text;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class RequestUri (internal)
//
// RequestUri implements an extension to the Uri provided by the context
// object, in order to break it up into the necessary components as well
// as add any additional manipulations required for the request.  Note that
// URL fragments (#) are stripped off by the runtime before we ever see them
//
// scheme : host [: port] / virtualRoot / file.aspx / path extension ? query # fragment
//
// http://localhost:8888/myapp/default.aspx/0/?key=value#jump
//
//	.FilePath		= /myapp/default.aspx
//	.PathInfo		= /0/
//	.QueryString	= key=value
//	.RawUrl			= /myapp/default.aspx/0/?key=value
//	.UriPath		= /myapp/default.aspx/0/
//---------------------------------------------------------------------------

ref class RequestUri
{
public:

	//-----------------------------------------------------------------------
	// Constructor
	
	RequestUri(HttpListenerContext^ context);

	//-----------------------------------------------------------------------
	// Member Functions

	// AppendPath
	//
	// Appends a path onto the end of the URI (for default documents)
	void AppendPath(String^ path);

	//-----------------------------------------------------------------------
	// Properties

	// FilePath
	//
	// Returns the file path component of the request URI
	property String^ FilePath
	{
		String^ get(void) { return m_filePath; }
	}

	// PathInfo
	//
	// Returns the url extension path into component of the request URI
	property String^ PathInfo
	{
		String^ get(void) { return m_pathInfo; }
	}

	// QueryString
	//
	// Returns the query string component of the request URI
	property String^ QueryString
	{
		String^ get(void) { return m_queryString; }
	}

	// RawUrl
	//
	// Returns the raw request URL 
	property String^ RawUrl
	{
		String^ get(void) { return m_rawUrl; }
	}

	// UriPath
	//
	// Returns the uri path component of the request URI
	property String^ UriPath
	{
		String^ get(void) { return m_uriPath; }
	}

private:

	//-----------------------------------------------------------------------
	// Private Member Functions

	// ParseUriComponents
	//
	// Reparses the URI components after a change or on initial set
	void ParseUriComponents(Uri^ uri);

	//-----------------------------------------------------------------------
	// Member Variables

	Uri^					m_baseUri;			// Current base URI object
	String^					m_filePath;			// File path string
	String^					m_pathInfo;			// URL extension path info
	String^					m_queryString;		// Query string
	String^					m_rawUrl;			// Raw URL string
	String^					m_uriPath;			// URI path string

	static array<__wchar_t>^ s_trimQuery = {'?'};	// For trimming queries
	static array<__wchar_t>^ s_trimPath = {'/'};	// For trimming paths
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __REQUESTURI_H_