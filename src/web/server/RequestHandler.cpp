//---------------------------------------------------------------------------
// RequestHandler Implementation
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
#include "RequestHandler.h"				// Include RequestHandler declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// RequestHandler Constructor
//
// Arguments:
//
//	appid			- Parent host's application ID string
//	config			- Application configuration information
//	context			- HttpListenerContext for this request
//	token			- Impersonation token to use for this request

RequestHandler::RequestHandler(String^ appid, WebApplicationConfiguration^ config, 
	HttpListenerContext^ context, IntPtr token) : m_config(config), m_appid(appid), 
	m_context(context), m_token(token)
{
	String^				extension;			// File extension

	Debug::Assert(m_context != nullptr);

	// URI MANAGER --------------------------------------------------

	m_uri = gcnew RequestUri(context);
	
	// DYNAMIC vs. STATIC CONTENT -----------------------------------

	// This only applies when the Virtual File System is active.  There is
	// no need to do any of this work if we're not in control of the caching

	if(HostingEnvironment::VirtualPathProvider->GetType() == StoragePathProvider::typeid) {

		extension = VirtualPathUtility::GetExtension(m_uri->FilePath);
		if(!String::IsNullOrEmpty(extension)) {

			m_staticContent = true;					// Assume static

			// Iterate over all of the configured non-cacheable extensions to determine
			// if this is static content or not. Annoying and slow, but necessary to get
			// HTTP 304 cache responses working with the VFS, which speeds things up a lot.

			for each(String^ dyn in m_config->VirtualFileSystem->NonCacheableExtensions) {

				if(String::Compare(extension, dyn, StringComparison::OrdinalIgnoreCase) == 0) { 
					
					m_staticContent = false;		// Dynamic content detected
					break;							// Break the loop, we're done looking
				}
			}
		}

		if(m_staticContent) m_staticContent = AddStaticContentHeaders();
	}

	// REQUEST ENTITY BODY ------------------------------------------

	// If there is entity body data to be read for this request, open
	// up the stream now so we can track it's existance more easily

	if(m_context->Request->HasEntityBody) 
		m_requestStream = m_context->Request->InputStream;

	// CLIENT/SERVER IDENTITY ---------------------------------------

	m_clientCert = gcnew zws::ClientCertificate(m_context->Request);
	m_serverCert = gcnew zws::ServerCertificate(m_context->Request);

	// CLIENT CONNECTED FLAG ----------------------------------------

	// Assume the client is connected until an exception occurs writing
	// data to it. See SafeOutputWrite()
	m_clientConnected = true;
}

//---------------------------------------------------------------------------
// Request Destructor

RequestHandler::~RequestHandler()
{
	if(m_disposed) return;

	// If the request stream was opened but never closed by a call into
	// EndOfRequest, go ahead and close it now to be neat and tidy

	if(m_requestStream != nullptr) m_requestStream->Close();
	m_requestStream = nullptr;

	// Dispose of any helper objects we created that should be disposed

	if(m_serverCert != nullptr) delete m_serverCert;
	if(m_clientCert != nullptr) delete m_clientCert;

	m_uri = nullptr;					// Release URI object instance
	m_disposed = true;					// Object has been disposed of
}

//---------------------------------------------------------------------------
// RequestHandler::AddStaticContentHeaders (private)
//
// When the virtual file system has been enabled, the ASP.NET runtime is
// absolutely useless for cache control (or at least I can't figure it out),
// so we handle it ourselves.  This portion of the implementation adds the
// necessary response headers Expires, Last-Modified and ETag.  The Host deals
// with actually sending back an HTTP 304 when static content is unmodified
//
// Arguments:
//
//	NONE

bool RequestHandler::AddStaticContentHeaders(void)
{
	StoragePathProvider^		provider;		// Our virtual path provider
	DateTime					modified;		// Last-Modified date value

	// Check to see if we're using our VirtualPathProvider and get a reference
	// to it all at the same time.  If the standard (or someone else's) provider
	// is in place, don't do anything special here and just bail out

	provider = dynamic_cast<StoragePathProvider^>(HostingEnvironment::VirtualPathProvider);
	if(provider == nullptr) return false;

	modified = provider->StoreDateTime;			// Get Last-Modified (UTC) date/time

	// Accept-Ranges
	m_context->Response->Headers[HttpResponseHeader::AcceptRanges] = "bytes";

	// Cache-Control
	m_context->Response->Headers[HttpResponseHeader::CacheControl] = "public";

	// Last-Modified (UTC)
	m_context->Response->Headers[HttpResponseHeader::LastModified] = 
		modified.ToString("R", DateTimeFormatInfo::InvariantInfo);

	// Expires (24 hours)
	m_context->Response->Headers[HttpResponseHeader::Expires] = 
		modified.AddDays(1).ToString("R", DateTimeFormatInfo::InvariantInfo);

	// ETag
	m_context->Response->Headers[HttpResponseHeader::ETag] = GenerateETag(modified);

	return true;			// <-- We are handling the static content headers
}

//---------------------------------------------------------------------------
// RequestHandler::AppendHeaderWithBigHack (private)
//
// As one might guess, this one's a big hack.  The standard WebHeaderCollection
// class used by the runtime is not designed to handle cookies properly at all,
// and it mashes them up into a single comma-delimited string.  This hack
// uses reflection (once again!) to directly modify the header so it contains 
// multiple embedded headers, thus tricking the runtime into sending them back 
// to the browser properly.  This originally only applied to Set-Cookie, but I've
// learned through trial and error it's better to just always do it this way.
//
// Arguments:
//
//	name		- Name of the header to be appended
//	value		- Value of the header to be appended

void RequestHandler::AppendHeaderWithBigHack(String^ name, String^ value)
{
	WebHeaderCollection^	headers;		// Header collection 
	String^					existing;		// Existing header string

	Debug::Assert(s_changeInternalHeader != nullptr);
	if(s_changeInternalHeader == nullptr)
		throw gcnew Exception("Internal failure getting header collection - reflection failed.");

	headers = m_context->Response->Headers;		// Get collection reference

	// Grab the existing header.  If there is none, just add it normally

	existing = headers[name];
	if(existing == nullptr) return headers->Set(name, value);

	// Here comes the big hack ... use the internal methods of WebHeaderCollection
	// to jam multiple Set-Cookie headers into it's NameValueCollection.  Believe
	// it or not, this actually works quite well ...

	existing += "\r\n" + name + ": " + value;
	s_changeInternalHeader->Invoke(headers, gcnew array<Object^>{name, existing});
}

//---------------------------------------------------------------------------
// RequestHandler::ClientCertificate::get (internal)
//
// Accesses the contained client certificate for this request

zws::ClientCertificate^ RequestHandler::ClientCertificate::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_clientCert;
}

//---------------------------------------------------------------------------
// RequestHandler::CloseConnection
//
// Closes the connection with the client
//
// Arguments:
//
//	NONE

void RequestHandler::CloseConnection(void)
{
	CHECK_DISPOSED(m_disposed);
	return __super::CloseConnection();
}

//---------------------------------------------------------------------------
// RequestHandler::EndOfRequest
//
// Invoked when the current request is complete
//
// Arguments:
//
//	NONE

void RequestHandler::EndOfRequest(void)
{
	CHECK_DISPOSED(m_disposed);

	// If an end of request callback was registered, invoke it now that
	// the request has officially ended (at least from our perspective)

	try { if(m_onEndReq != nullptr) m_onEndReq(this, m_onEndReqState); }
	catch(Exception^) { /* DO NOTHING */ }

	// The close operation can fail if we previous ran into an HttpListenerException
	// when trying to write into the output stream, but there isn't anything we can
	// seemingly do about it except wrap it up in a very generic try/catch handler

	try { m_context->Response->Close(); }
	catch(Exception^) { /* DO NOTHING */ }

	delete this;				// Self-dispose once the request is done
}

//---------------------------------------------------------------------------
// RequestHandler::FlushResponse
//
// Sends all pending response data to the client
//
// Arguments:
//
//	finalFlush		- Flag indicating the last time data will be flushed

void RequestHandler::FlushResponse(bool)
{
	CHECK_DISPOSED(m_disposed);

	// When the response is in chunked transfer mode, we wait to send the
	// data to the client until FlushResponse() has been called.  The stream
	// will be set up and maintained by SendResponseFromMemory (below), all we
	// need to do is actually send the data and release that stream here

	if(m_chunkStream != nullptr) {

		SafeOutputWrite(m_chunkStream->GetBuffer(), 0, static_cast<int>(m_chunkStream->Length));		
		
		delete m_chunkStream;						// Dispose of the stream
		m_chunkStream = nullptr;					// Release strong reference
	}	

	m_context->Response->OutputStream->Flush();		// NOTE: Does nothing
}

//---------------------------------------------------------------------------
// RequestHandler::GenerateETag (private, static)
//
// Generates an ETag response header based on a Last-Modified date
//
// Arguments:
//
//	modified	- Last-Modified date/time stamp (UTC)

String^ RequestHandler::GenerateETag(DateTime modified)
{
	StringBuilder^		builder;		// ETag string builder
	__int64				ftModified;		// MODIFIED expressed as a FILETIME
	__int64				ftNow;			// NOW expressed as a FILETIME

	// This logic is *completely* ripped off from the .NET StaticFileHandler, so
	// certainly don't blame me if it's not as correct as it should be

	builder = gcnew StringBuilder();
	ftModified = modified.ToFileTime();
	ftNow = DateTime::Now.ToFileTime();

    builder->Append("\"");
	builder->Append(ftModified.ToString("X8", CultureInfo::InvariantCulture));
    builder->Append(":");
	builder->Append(ftNow.ToString("X8", CultureInfo::InvariantCulture));
    builder->Append("\"");

	if((ftNow - ftModified) <= 30000000) return "W/" + builder->ToString();
	else return builder->ToString();
}

//---------------------------------------------------------------------------
// RequestHandler::GetAppPath
//
// Returns the virtual path to the currently executing server application
//
// Arguments:
//
//	NONE

String^ RequestHandler::GetAppPath(void)
{
	CHECK_DISPOSED(m_disposed);
	return HostingEnvironment::ApplicationVirtualPath;
}

//---------------------------------------------------------------------------
// RequestHandler::GetAppPathTranslated
//
// Returns the physical path to the currently executing server application
//
// Arguments:
//
//	NONE

String^ RequestHandler::GetAppPathTranslated(void)
{
	CHECK_DISPOSED(m_disposed);
	return HostingEnvironment::ApplicationPhysicalPath;
}

//---------------------------------------------------------------------------
// RequestHandler::GetAppPoolID
//
// Returns the application pool ID for the current URL
//
// Arguments:
//
//	NONE

String^ RequestHandler::GetAppPoolID(void)
{
	CHECK_DISPOSED(m_disposed);
	return __super::GetAppPoolID();
}

//---------------------------------------------------------------------------
// RequestHandler::GetBytesRead
//
// Gets the number of bytes read in from the client
//
// Arguments:
//
//	NONE

__int64 RequestHandler::GetBytesRead(void)
{
	CHECK_DISPOSED(m_disposed);

	// All of the built-in request processors, including IIS throw an HTTP
	// exception if this method happens to be invoked.  Throw the same one.

	throw gcnew HttpException("Not Supported");
}

//---------------------------------------------------------------------------
// RequestHandler::GetClientCertificate
//
// Gets the certification fields from a request issued by the client
//
// Arguments:
//
//	NONE

array<Byte>^ RequestHandler::GetClientCertificate(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_clientCert->Certificate;
}

//---------------------------------------------------------------------------
// RequestHandler::GetClientCertificateBinaryIssuer
//
// Gets the certificate issuer, in binary format
//
// Arguments:
//
//	NONE

array<Byte>^ RequestHandler::GetClientCertificateBinaryIssuer(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_clientCert->BinaryIssuer;
}

//---------------------------------------------------------------------------
// RequestHandler::GetClientCertificateEncoding
//
// Returns the Encoding object in which the client certificate was encoded 
//
// Arguments:
//
//	NONE

int RequestHandler::GetClientCertificateEncoding(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_clientCert->Encoding;
}

//---------------------------------------------------------------------------
// RequestHandler::GetClientCertificatePublicKey
//
// Gets a PublicKey object associated with the client certificate
//
// Arguments:
//
//	NONE

array<Byte>^ RequestHandler::GetClientCertificatePublicKey(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_clientCert->PublicKey;
}

//---------------------------------------------------------------------------
// RequestHandler::GetClientCertificateValidFrom
//
// Gets the date when the certificate becomes valid
//
// Arguments:
//
//	NONE

DateTime RequestHandler::GetClientCertificateValidFrom(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_clientCert->ValidFrom;
}

//---------------------------------------------------------------------------
// RequestHandler::GetClientCertificateValidUntil
//
// Gets the client certificate expiration date
//
// Arguments:
//
//	NONE

DateTime RequestHandler::GetClientCertificateValidUntil(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_clientCert->ValidUntil;
}

//---------------------------------------------------------------------------
// RequestHandler::GetConnectionID
//
// Returns the ID of the current connection
//
// Arguments:
//
//	NONE

__int64 RequestHandler::GetConnectionID(void)
{
	CHECK_DISPOSED(m_disposed);
	return __super::GetConnectionID();
}

//---------------------------------------------------------------------------
// RequestHandler::GetFilePath
//
// Returns the virtual path to the requested URI
//
// Arguments:
//
//	NONE

String^ RequestHandler::GetFilePath(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_uri->FilePath;
}

//---------------------------------------------------------------------------
// RequestHandler::GetFilePathTranslated
//
// Returns the physical file path to the requested URI
//
// Arguments:
//
//	NONE

String^ RequestHandler::GetFilePathTranslated(void)
{
	CHECK_DISPOSED(m_disposed);
	return HostingEnvironment::MapPath(m_uri->FilePath);
}

//---------------------------------------------------------------------------
// RequestHandler::GetHttpVerbName
//
// Returns the specified member of the request header
//
// Arguments:
//
//	NONE

String^ RequestHandler::GetHttpVerbName(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_context->Request->HttpMethod;
}

//---------------------------------------------------------------------------
// RequestHandler::GetHttpVersion
//
// Provides access to the HTTP version of the request
//
// Arguments:
//
//	NONE

String^ RequestHandler::GetHttpVersion(void)
{
	CHECK_DISPOSED(m_disposed);

	Version^ ver = m_context->Request->ProtocolVersion;
	return String::Format("HTTP/{0}.{1}", ver->Major, ver->Minor);
}

//---------------------------------------------------------------------------
// RequestHandler::GetKnownRequestHeader
//
// Returns the standard HTTP request header that corresponds to the specified index
//
// Arguments:
//
//	index		- Known request header index

String^ RequestHandler::GetKnownRequestHeader(int index)
{
	CHECK_DISPOSED(m_disposed);

	// Use properties of the HttpListenerRequest before accessing it's headers
	// collection with the 'known' index.  Certain ones, like HeaderUserAgent
	// don't work properly with the collection for some unknown reason ...

	switch(index) {

		case HttpWorkerRequest::HeaderContentType:		return m_context->Request->ContentType;
		case HttpWorkerRequest::HeaderContentLength:	return m_context->Request->ContentLength64.ToString();
		case HttpWorkerRequest::HeaderReferer:			return (m_context->Request->UrlReferrer != nullptr) ? m_context->Request->UrlReferrer->ToString() : String::Empty;
		case HttpWorkerRequest::HeaderUserAgent:		return m_context->Request->UserAgent;
	}

	// Not a special case .. just return whatever the header collection has

	return m_context->Request->Headers[GetKnownRequestHeaderName(index)];
}

//---------------------------------------------------------------------------
// RequestHandler::GetLocalAddress
//
// Returns the local address member of the request header
//
// Arguments:
//
//	NONE

String^ RequestHandler::GetLocalAddress(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_context->Request->LocalEndPoint->Address->ToString();
}

//---------------------------------------------------------------------------
// RequestHandler::GetLocalPort
//
// Provides access to the specified member of the request header
//
// Arguments:
//
//	NONE

int RequestHandler::GetLocalPort(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_context->Request->LocalEndPoint->Port;
}

//---------------------------------------------------------------------------
// RequestHandler::GetPathInfo
//
// Returns additional path information for a resource with a URL extension
//
// Arguments:
//
//	NONE

String^ RequestHandler::GetPathInfo(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_uri->PathInfo;
}

//---------------------------------------------------------------------------
// RequestHandler::GetPreloadedEntityBody
//
// Returns the portion of the HTTP request body that has currently been read
//
// Arguments:
//
//	NONE

array<Byte>^ RequestHandler::GetPreloadedEntityBody(void)
{
	CHECK_DISPOSED(m_disposed);
	return __super::GetPreloadedEntityBody();
}

//---------------------------------------------------------------------------
// RequestHandler::GetPreloadedEntityBody
//
// Returns the portion of the HTTP request body that has currently been read
//
// Arguments:
//
//	buffer		- The data to read
//	offset		- The offset at which to begin reading

int RequestHandler::GetPreloadedEntityBody(array<Byte>^ buffer, int offset)
{
	CHECK_DISPOSED(m_disposed);
	return __super::GetPreloadedEntityBody(buffer, offset);
}

//---------------------------------------------------------------------------
// RequestHandler::GetPreloadedEntityBodyLength
//
// Gets the length of the portion of the HTTP request body that has been read
//
// Arguments:
//
//	NONE

int RequestHandler::GetPreloadedEntityBodyLength(void)
{
	CHECK_DISPOSED(m_disposed);
	return __super::GetPreloadedEntityBodyLength();
}

//---------------------------------------------------------------------------
// RequestHandler::GetProtocol
//
// Returns the HTTP protocol (HTTP or HTTPS)
//
// Arguments:
//
//	NONE

String^ RequestHandler::GetProtocol(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_context->Request->IsSecureConnection ? "https" : "http";
}

//---------------------------------------------------------------------------
// RequestHandler::GetQueryString
//
// Returns the query string specified in the request URL
//
// Arguments:
//
//	NONE

String^ RequestHandler::GetQueryString(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_uri->QueryString;
}

//---------------------------------------------------------------------------
// RequestHandler::GetQueryStringRawBytes
//
// Returns the response query string as an array of bytes
//
// Arguments:
//
//	NONE

array<Byte>^ RequestHandler::GetQueryStringRawBytes(void)
{
	CHECK_DISPOSED(m_disposed);

	// Unlike GetQueryString() it looks like we're supposed to return NULL
	// from this one if there is no query string to be had...

	String^ query = GetQueryString();
	return (query->Length > 0) ? Encoding::UTF8->GetBytes(query) : nullptr;
}

//---------------------------------------------------------------------------
// RequestHandler::GetRawUrl
//
// Returns the URL path contained in the request header with the query string
//
// Arguments:
//
//	NONE

String^ RequestHandler::GetRawUrl(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_uri->RawUrl;
}

//---------------------------------------------------------------------------
// RequestHandler::GetRemoteAddress
//
// Returns the remote address member of the request header
//
// Arguments:
//
//	NONE

String^ RequestHandler::GetRemoteAddress(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_context->Request->RemoteEndPoint->Address->ToString();
}

//---------------------------------------------------------------------------
// RequestHandler::GetRemoteName
//
// Returns the name of the client computer
//
// Arguments:
//
//	NONE

String^ RequestHandler::GetRemoteName(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_context->Request->RemoteEndPoint->Address->ToString();
}

//---------------------------------------------------------------------------
// RequestHandler::GetRemotePort
//
// Provides access to the specified member of the request header
//
// Arguments:
//
//	NONE

int RequestHandler::GetRemotePort(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_context->Request->RemoteEndPoint->Port;
}

//---------------------------------------------------------------------------
// RequestHandler::GetRequestReason
//
// Returns the reason for the request
//
// Arguments:
//
//	NONE

int RequestHandler::GetRequestReason(void)
{
	CHECK_DISPOSED(m_disposed);
	return __super::GetRequestReason();
}

//---------------------------------------------------------------------------
// RequestHandler::GetServerName
//
// Returns the name of the local server
//
// Arguments:
//
//	NONE

String^ RequestHandler::GetServerName(void)
{
	CHECK_DISPOSED(m_disposed);

	// First try to get the "Host" header from the request, and if that 
	// doesn't work just use the local listener's IP address instead.

	String^ host = GetKnownRequestHeader(HeaderHost);
	if(String::IsNullOrEmpty(host)) return GetLocalAddress();

	// For some ingenious reason, the HOST header will contain the port number if
	// it was explicitly specified, and that needs to be removed for ASP.NET

	return (host->Contains(":")) ? host->Substring(0, host->IndexOf(':')) : host;
}

//---------------------------------------------------------------------------
// RequestHandler::GetServerVariable
//
// Returns a single server variable from a dictionary of server variables
//
// Arguments:
//
//	name		- Name of the server variable to be retrieved

String^ RequestHandler::GetServerVariable(String^ name)
{
	PinnedStringPtr				pinName;		// Pinned name string

	CHECK_DISPOSED(m_disposed);

	if(String::IsNullOrEmpty(name)) return nullptr;
	pinName = PtrToStringChars(name);

	// A great deal of the proper server variables are set up by the HttpRequest object
	// internally to .NET.  By checking that class' source code, we can determine which
	// additional variables we have to set up ourselves if we want them to work.

	// CLIENT IDENTITY
	if(m_context->Request->IsAuthenticated) {

		if(wcscmp(pinName, L"LOGON_USER") == 0)		return m_context->User->Identity->Name;
		if(wcscmp(pinName, L"AUTH_TYPE") == 0)		return m_context->User->Identity->AuthenticationType;

		if(wcscmp(pinName, L"AUTH_PASSWORD") == 0) {

			// AUTH_PASSWORD is a little more difficult, since it's only returned when
			// basic authentication is used. Return NULL in all other cases ...

			if(m_context->User->Identity->GetType() == HttpListenerBasicIdentity::typeid)
				return dynamic_cast<HttpListenerBasicIdentity^>(m_context->User->Identity)->Password;
			else return nullptr;
		}
	}

	// HTTPS/SSL SERVER VARIABLES
	if(wcscmp(pinName, L"HTTPS") == 0)				return (m_context->Request->IsSecureConnection) ? "on" : "off";

	// MISCELLANEOUS VARIABLES
	if(wcscmp(pinName, L"APPL_MD_PATH") == 0)		return m_appid;
	if(wcscmp(pinName, L"REMOTE_PORT") == 0)		return m_context->Request->RemoteEndPoint->Port.ToString();
	if(wcscmp(pinName, L"SERVER_SOFTWARE") == 0)	return WebServerVersion::SoftwareVersion;

	// The following set of server variables only apply to secure connections. They
	// should be left at the default empty/null values on insecure connections.

	if(m_context->Request->IsSecureConnection) {

		// HTTPS/SSL SERVER VARIABLES
		if(wcscmp(pinName, L"HTTPS_KEYSIZE") == 0)			return m_serverCert->KeySize.ToString();
		if(wcscmp(pinName, L"HTTPS_SECRETKEYSIZE") == 0)	return m_serverCert->SecretKeySize.ToString();
		if(wcscmp(pinName, L"HTTPS_SERVER_ISSUER") == 0)	return m_serverCert->Issuer;
		if(wcscmp(pinName, L"HTTPS_SERVER_SUBJECT") == 0)	return m_serverCert->Subject;

		// SERVER CERTIFICATE SERVER VARIABLES
		if(wcscmp(pinName, L"CERT_KEYSIZE") == 0)			return m_serverCert->KeySize.ToString();
		if(wcscmp(pinName, L"CERT_SECRETKEYSIZE") == 0)		return m_serverCert->SecretKeySize.ToString();
		if(wcscmp(pinName, L"CERT_SERVER_ISSUER") == 0)		return m_serverCert->Issuer;
		if(wcscmp(pinName, L"CERT_SERVER_SUBJECT") == 0)	return m_serverCert->Subject;

		// CLIENT CERTIFICATE SERVER VARIABLES
		if(wcscmp(pinName, L"CERT_COOKIE") == 0)			return m_clientCert->Cookie;
		if(wcscmp(pinName, L"CERT_FLAGS") == 0)				return m_clientCert->Flags.ToString();
		if(wcscmp(pinName, L"CERT_ISSUER") == 0)			return m_clientCert->Issuer;
		if(wcscmp(pinName, L"CERT_SERIALNUMBER") == 0)		return m_clientCert->SerialNumber;
		if(wcscmp(pinName, L"CERT_SUBJECT") == 0)			return m_clientCert->Subject;
	}

	return nullptr;			// <--- Not a server variable we're changing
}

//---------------------------------------------------------------------------
// RequestHandler::GetTotalEntityBodyLength
//
// Gets the length of the entire HTTP request body
//
// Arguments:
//
//	NONE

int RequestHandler::GetTotalEntityBodyLength(void)
{
	CHECK_DISPOSED(m_disposed);

	// If the Content-Length is unknown, it will be set to -1.  Use
	// a return value of zero instead, like everyone else does

	__int64 length = m_context->Request->ContentLength64;
	return (length >= 0) ? static_cast<int>(length) : 0;
}

//---------------------------------------------------------------------------
// RequestHandler::GetUnknownRequestHeader
//
// Returns a nonstandard HTTP request header value
//
// Arguments:
//
//	name		- Name of the header to be returned

String^ RequestHandler::GetUnknownRequestHeader(String^ name)
{
	String^				value;			// Request header value

	CHECK_DISPOSED(m_disposed);
	
	// First go through the REAL mechanism, and then fall back into the
	// cheesy request header hack collection if we didn't find it ...

	value = m_context->Request->Headers[name];
	if(value == nullptr) m_config->CustomRequestHeaders->TryGetValue(name, value);

	return value;						// Return the request header value
}

//---------------------------------------------------------------------------
// RequestHandler::GetUnknownRequestHeaders
//
// Get all nonstandard HTTP header name-value pairs
//
// Arguments:
//
//	NONE

array<array<String^>^>^ RequestHandler::GetUnknownRequestHeaders(void)
{
	List<array<String^>^>^		headers;		// Generated headers collection
	String^						name;			// A single header name

	CHECK_DISPOSED(m_disposed);

	headers = gcnew List<array<String^>^>();	// Construct the collection

	// Iterate over all of the request headers. Any headers that cannot be matched
	// to a known header index by HttpWorkerRequest get added into the collection
	// as an array of strings (consisting of name/value pairs)

	for(int index = 0; index < m_context->Request->Headers->Count; index++) {
	
		name = m_context->Request->Headers->GetKey(index);
		if(GetKnownRequestHeaderIndex(name) >= 0) continue;

		headers->Add(gcnew array<String^> { name, m_context->Request->Headers[index] });
	}

	// Now iterate over the custom request headers, and add those as well.  Only
	// add headers that don't exist as real headers -- this will of course slow 
	// things down ever so slighly more, but using custom headers IS a lot easier
	// for the application to deal with than the IRegisteredObject methodology

	for each(KeyValuePair<String^, String^> header in m_config->CustomRequestHeaders) {

		if(m_context->Request->Headers[header.Key] == nullptr)
			headers->Add(gcnew array<String^> { header.Key, header.Value });
	}

	return headers->ToArray();		// Convert into an array and return the headers
}

//---------------------------------------------------------------------------
// RequestHandler::GetUriPath
//
// Returns the virtual path to the requested URI
//
// Arguments:
//
//	NONE

String^ RequestHandler::GetUriPath(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_uri->UriPath;
}

//---------------------------------------------------------------------------
// RequestHandler::GetUrlContextID
//
// Returns the context ID of the current connection
//
// Arguments:
//
//	NONE

__int64 RequestHandler::GetUrlContextID(void)
{
	CHECK_DISPOSED(m_disposed);
	return __super::GetUrlContextID();
}

//---------------------------------------------------------------------------
// RequestHandler::GetUserToken
//
// Gets the authenticated user's token
//
// Arguments:
//
//	NONE

IntPtr RequestHandler::GetUserToken(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_token;
}

//---------------------------------------------------------------------------
// RequestHandler::GetVirtualPathToken
//
// Gets the impersonation token for the request virtual path
//
// Arguments:
//
//	NONE

IntPtr RequestHandler::GetVirtualPathToken(void)
{
	CHECK_DISPOSED(m_disposed);
	return __super::GetVirtualPathToken();
}

//---------------------------------------------------------------------------
// RequestHandler::HasClientCertificate (internal)
//
// Determines if the request has a client certificate or not

bool RequestHandler::HasClientCertificate::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_clientCert->IsPresent;
}

//---------------------------------------------------------------------------
// RequestHandler::HeadersSent
//
// Value indicating whether HTTP response headers have been sent to the client 
//
// Arguments:
//
//	NONE

bool RequestHandler::HeadersSent(void)
{
	Object^					propValue;			// Boxed property value

	CHECK_DISPOSED(m_disposed);

	// Originally I avoided reflection by trying to set ContentLength64 to the
	// same value that it already had, but that can mess up chunked transfers.
	// So ... had to go with hacking into the HttpListenerResponse instead. 

	Debug::Assert(s_sentHeaders != nullptr);
	if(s_sentHeaders == nullptr) 
		throw gcnew Exception("Internal failure getting header status - reflection failed");

	propValue = s_sentHeaders->GetValue(m_context->Response, nullptr);
	if(propValue == nullptr) 
		throw gcnew Exception("Internal failure getting header status - unexpected result");

	return *dynamic_cast<bool^>(propValue);		// Unbox and return the value
}

//---------------------------------------------------------------------------
// RequestHandler::IsAnonymous::get (internal)
//
// Determines if the request is anonymous or not

bool RequestHandler::IsAnonymous::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return (m_token == IntPtr::Zero);
}

//---------------------------------------------------------------------------
// RequestHandler::IsClientConnected
//
// Returns a value indicating whether the client connection is still active
//
// Arguments:
//
//	NONE

bool RequestHandler::IsClientConnected(void)
{
	CHECK_DISPOSED(m_disposed);
	return (m_clientConnected) ? __super::IsClientConnected() : false;
}

//---------------------------------------------------------------------------
// RequestHandler::IsEntireEntityBodyIsPreloaded
//
// Value indicating whether all request data is available
//
// Arguments:
//
//	NONE

bool RequestHandler::IsEntireEntityBodyIsPreloaded(void)
{
	CHECK_DISPOSED(m_disposed);
	return __super::IsEntireEntityBodyIsPreloaded();
}

//---------------------------------------------------------------------------
// RequestHandler::IsSecure
//
// Determines if this is a secure SSL request or not
//
// Arguments:
//
//	NONE

bool RequestHandler::IsSecure(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_context->Request->IsSecureConnection;
}

//---------------------------------------------------------------------------
// RequestHandler::MachineConfigPath::get
//
// Gets the full physical path to the Machine.config file

String^ RequestHandler::MachineConfigPath::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return HttpRuntime::MachineConfigurationDirectory;
}

//---------------------------------------------------------------------------
// RequestHandler::MachineInstallDirectory::get
//
// Gets the physical path to the directory where the ASP.NET binaries are

String^ RequestHandler::MachineInstallDirectory::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return HttpRuntime::AspInstallDirectory;
}

//---------------------------------------------------------------------------
// RequestHandler::MapPath
//
// Returns the physical path corresponding to the specified virtual path
//
// Arguments:
//
//	virtualPath		- The virtual path to be mapped

String^ RequestHandler::MapPath(String^ virtualPath)
{
	CHECK_DISPOSED(m_disposed);
	return HostingEnvironment::MapPath(virtualPath);
}

//---------------------------------------------------------------------------
// RequestHandler::ReadEntityBody
//
// Reads request data from the client (when not preloaded)
//
// Arguments:
//
//	buffer		- The byte array to read the data into
//	size		- The maximum number of bytes to read

int RequestHandler::ReadEntityBody(array<Byte> ^buffer, int size)
{
	CHECK_DISPOSED(m_disposed);
	return ReadEntityBody(buffer, 0, size);
}

//---------------------------------------------------------------------------
// RequestHandler::ReadEntityBody
//
// Reads request data from the client (when not preloaded)
//
// Arguments:
//
//	buffer		- The byte array to read the data into
//	offset		- The byte offset at which to begin reading
//	size		- The maximum number of bytes to read

int RequestHandler::ReadEntityBody(array<Byte>^ buffer, int offset, int size)
{
	CHECK_DISPOSED(m_disposed);

	// If we have a cached off request stream, read the next chunk of
	// entity body data from it.  Otherwise just return zero ...

	try { return (m_requestStream != nullptr) ? m_requestStream->Read(buffer, offset, size) : 0; }
	catch(HttpListenerException^) { return 0; }
}

//---------------------------------------------------------------------------
// RequestHandler::RequestTraceIdentifer::get
//
// Gets the corresponding Event Tracking for Windows trace ID for the request

Guid RequestHandler::RequestTraceIdentifier::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_context->Request->RequestTraceIdentifier;
}

//---------------------------------------------------------------------------
// RequestHandler::RootWebConfigPath::get
//
// Gets the full physical path to the root web.config file

String^ RequestHandler::RootWebConfigPath::get(void)
{
	CHECK_DISPOSED(m_disposed);

	// This is how the internal HttpConfigurationSystem class does it,
	// and since it's seemingly not exposed any other way (in System.Web),
	// it's a pretty safe bet we can get away with the same method ...

	return Path::Combine(MachineConfigPath, "web.config");
}

//---------------------------------------------------------------------------
// RequestHandler::SafeOutputWrite (private)
//
// Handles the Response.OutputStream.Write with exception handling
//
// Arguments:
//
//	buffer		- Buffer of data to send into the response stream
//	offset		- Offset into the buffer to begin writing from
//	length		- Number of bytes to be written from the buffer

void RequestHandler::SafeOutputWrite(array<Byte>^ buffer, int offset, int length)
{
	// This used to just absorb all errors, but streaming applications that continue
	// to send data to the client would potentially never stop since IsClientConnected
	// would continually return true
	try { m_context->Response->OutputStream->Write(buffer, offset, length); }
	catch(Exception^) { m_clientConnected = false; }
}

//---------------------------------------------------------------------------
// RequestHandler::SendCalculatedContentLength
//
// Adds a Content-Length HTTP header to the response.
//
// Arguments:
//
//	contentLength	- The length of the response, in bytes

void RequestHandler::SendCalculatedContentLength(int contentLength)
{
	CHECK_DISPOSED(m_disposed);
	m_context->Response->ContentLength64 = contentLength;
}

//---------------------------------------------------------------------------
// RequestHandler::SendKnownResponseHeader
//
// Adds a standard HTTP header to the response.
//
// Arguments:
//
//	index		- Known header index
//	value		- Value of the header to be sent

void RequestHandler::SendKnownResponseHeader(int index, String^ value)
{
	CHECK_DISPOSED(m_disposed);

	// There are a few headers we need to watch out for and either set
	// using the Response object properties, or ignore completely.  See the
	// documentation for HttpListenerResponse in the MSDN library for details

	switch(index) {

		case HttpWorkerRequest::HeaderContentLength:
		case HttpWorkerRequest::HeaderWwwAuthenticate:
			return;

		case HttpWorkerRequest::HeaderKeepAlive:		
			m_context->Response->KeepAlive = true;
			return;

		case HttpWorkerRequest::HeaderTransferEncoding:
			m_context->Response->SendChunked = (String::Compare(value, "chunked", 
				StringComparison::OrdinalIgnoreCase) == 0);
			return;

		// If we're already handling static content on our own, don't allow
		// the ASP.NET runtime to modify any of the following headers ...

		case HttpWorkerRequest::HeaderAcceptRanges:
		case HttpWorkerRequest::HeaderCacheControl:
		case HttpWorkerRequest::HeaderEtag:
		case HttpWorkerRequest::HeaderExpires:
		case HttpWorkerRequest::HeaderLastModified:
			if(m_staticContent) return;
			break;
	}

	// Always use the header hack method to append headers now since the problem
	// happens anytime the browser is expecting multiples of the same header ..

	return AppendHeaderWithBigHack(GetKnownResponseHeaderName(index), value);
}

//---------------------------------------------------------------------------
// RequestHandler::SendResponseFromFile
//
// Adds the contents of the specified file to the response
//
// Arguments:
//
//	handle		- The handle of the file to send
//	offset		- The starting position in the file
//	length		- The number of bytes to send

void RequestHandler::SendResponseFromFile(IntPtr handle, __int64 offset, __int64 length)
{
	FileStream^				inputStream;		// Input file data stream

	CHECK_DISPOSED(m_disposed);

	// The handle must obviously be valid, and there has to be at least one
	// byte of data to send from it before we bother attempting this operation

	if((handle == IntPtr::Zero) || (handle.ToPointer() == INVALID_HANDLE_VALUE)) return;
	if(length <= 0) return;

	// Attempt to create the new Stream instance against the provided handle, 
	// append it to the response, and be certain to close it when we're done

	inputStream = gcnew FileStream(gcnew SafeFileHandle(handle, false), FileAccess::Read);

	try { SendResponseFromStream(inputStream, offset, length); }
	finally { delete inputStream; }
}

//---------------------------------------------------------------------------
// RequestHandler::SendResponseFromFile
//
// Adds the contents of the specified file to the response
//
// Arguments:
//
//	filename	- The name of the file to be sent
//	offset		- The starting position in the file
//	length		- The number of bytes to send

void RequestHandler::SendResponseFromFile(String^ filename, __int64 offset, __int64 length)
{
	FileStream^				inputStream;		// Input file data stream

	CHECK_DISPOSED(m_disposed);

	// The handle must obviously be valid, and there has to be at least one
	// byte of data to send from it before we bother attempting this operation

	if(String::IsNullOrEmpty(filename)) return;
	if(length <= 0) return;

	// Since we're opening the file ourselves, it's probably a good idea to 
	// demand a FileIOPermission against it.  FileStream probably does that
	// anyway ... should look into that

	(gcnew FileIOPermission(FileIOPermissionAccess::Read, filename))->Demand();

	// Attempt to create the new Stream instance against the provided handle, 
	// append it to the response, and be certain to close it when we're done

	inputStream = gcnew FileStream(filename, FileMode::Open, FileAccess::Read, FileShare::Read);

	try { SendResponseFromStream(inputStream, offset, length); }
	finally { delete inputStream; }
}

//---------------------------------------------------------------------------
// RequestHandler::SendResponseFromMemory
//
// Adds the specified number of bytes from a byte array to the response
//
// Arguments:
//
//	data		- The byte array to send
//	length		- The number of bytes to send

void RequestHandler::SendResponseFromMemory(array<Byte>^ data, int length)
{
	int						size = 0;		// Chunked response size

	CHECK_DISPOSED(m_disposed);

	// If the response isn't chunked, just send the data as-is to the client.
	// Don't cache the data off and wait for the flush, that's a waste of memory

	if(!m_context->Response->SendChunked) 
		return SafeOutputWrite(data, 0, (length == -1) ? data->Length : length);

	// Chunked responses are a little weird (as I found out the hard way), because
	// HttpListenerResponse and HttpRuntime BOTH try to handle the chunk header and
	// terminator.  Blindly sending everything we're asked to yields very corrupt data.  
	//
	// The solution I've come up with is to generate a fixed length MemoryStream
	// based on the chunk header we get, and continually write to that stream until
	// we reach the specified size.  That way the terminator gets ignored but we don't
	// miss any real data that LOOKS like a terminator.  The data will be written to 
	// the client by FlushResponse (above), which is called by the HTTP runtime when
	// the chunk is ready to be sent

	// HEADER: Create the fixed size MemoryStream based on the specified length

	if(m_chunkStream == nullptr) {

		// By setting size to zero on a parse exception, we prevent the entire chunk from
		// being transmitted.  That's better than sending a corrupt chunk in my opinion

		try { size = int::Parse(Encoding::UTF8->GetString(data)->TrimEnd(s_crlf), NumberStyles::HexNumber); }
		catch(Exception^) { size = 0; }

		m_chunkStream = gcnew MemoryStream(size);		// Construct the memory stream
	}

	// DATA: Write the data into the MemoryStream, up to it's maximum allowed size. It's
	// expected that the final CRLF pair that terminates the chunk will be ignored

	else {
		
		length = Math::Min(m_chunkStream->Capacity - static_cast<int>(m_chunkStream->Length), length);
		if(length > 0) m_chunkStream->Write(data, 0, length);
	}
}

//---------------------------------------------------------------------------
// RequestHandler::SendResponseFromMemory
//
// Adds the specified number of bytes from a byte array to the response
//
// Arguments:
//
//	data		- Unmanaged pointer to the block of memory
//	length		- The number of bytes to send

void RequestHandler::SendResponseFromMemory(IntPtr data, int length)
{
	array<Byte>^		buffer;		// Buffer to pass to other version

	CHECK_DISPOSED(m_disposed);

	if((data == IntPtr::Zero) || (length <= 0)) return;

	buffer = gcnew array<Byte>(length);			// Allocate temporary buffer
	Marshal::Copy(data, buffer, 0, length);		// Copy in the unmanaged data

	SendResponseFromMemory(buffer, length);		// Send the response from byte[]
}

//---------------------------------------------------------------------------
// RequestHandler::SendResponseFromStream (private)
//
// Adds the contents of the specified stream to the response
//
// Arguments:
//
//	stream		- Input stream instance
//	offset		- Offset into the stream to begin reading from
//	length		- Maximum number of bytes to read from the stream

void RequestHandler::SendResponseFromStream(Stream^ stream, __int64 offset, __int64 length)
{
	int					bytesRead;				// Number of bytes read
	array<Byte>^		buffer;					// Local stream I/O buffer
	const int			BUFFER_SIZE = 65536;	// Data transfer buffer size

	// We can only deal with Int32 values for offset and length, so go ahead
	// and throw an HttpException if something bigger than ~2GB is being sent

	if(offset > Int32::MaxValue) throw gcnew HttpException("Invalid Range");
	if(length > Int32::MaxValue) throw gcnew HttpException("Invalid Range");

	buffer = gcnew array<Byte>(BUFFER_SIZE);	// Create the transfer buffer

	// Align the stream at the specified offset.  For streams that don't support
	// seeking, we have to actually read the data until we hit the offset

	if(stream->CanSeek) stream->Seek(offset, SeekOrigin::Begin);

	else {

		do {
		
			bytesRead = stream->Read(buffer, 0, Math::Min(static_cast<int>(offset), BUFFER_SIZE));
			offset -= bytesRead;
		
			// If nothing was returned from the stream, and we still expected to have
			// more to read, throw the same HttpException that IIS would given this problem

			if((offset > 0) && (bytesRead == 0)) throw gcnew HttpException("Invalid Range");

		} while(offset > 0);
	}

	// Perform the main stream copy from the input to the response, using the
	// 8KB buffer as the go-between to try and keep memory usage realistic here

	do {

		bytesRead = stream->Read(buffer, 0, Math::Min(static_cast<int>(length), BUFFER_SIZE));
		if(bytesRead > 0) SendResponseFromMemory(buffer, bytesRead);
		length -= bytesRead;

		// If nothing was returned from the stream, and we still expected to have
		// more to read, throw the same HttpException that IIS would given this problem

		if((bytesRead == 0) && (length > 0)) throw gcnew HttpException("Invalid Range");

	} while(length > 0);
}

//---------------------------------------------------------------------------
// RequestHandler::SendStatus
//
// Specifies the HTTP status code and status description of the response
//
// Arguments:
//
//	statusCode			- The status code to send
//	statusDescription	- The status description to send

void RequestHandler::SendStatus(int statusCode, String^ statusDescription)
{
	CHECK_DISPOSED(m_disposed);

	m_context->Response->StatusCode = statusCode;
	m_context->Response->StatusDescription = statusDescription;
}

//---------------------------------------------------------------------------
// RequestHandler::SendUnknownResponseHeader
//
// Adds a nonstandard HTTP header to the response
//
// Arguments:
//
//	name		- Name of the header to send
//	value		- Value of the header to send

void RequestHandler::SendUnknownResponseHeader(String^ name, String^ value)
{
	CHECK_DISPOSED(m_disposed);
	return AppendHeaderWithBigHack(name, value);
}

//---------------------------------------------------------------------------
// RequestHandler::SetEndOfSendNotification
//
// Registers for an optional notification when all the response data is sent
//
// Arguments:
//
//	callback	- Notification callback to be invoked
//	extraData	- An additional parameter to the callback

void RequestHandler::SetEndOfSendNotification(EndOfSendNotification^ callback, 
	Object^ extraData)
{
	CHECK_DISPOSED(m_disposed);

	if(m_onEndReq != nullptr) throw gcnew InvalidOperationException();
	if(callback == nullptr) throw gcnew ArgumentNullException();

	m_onEndReq = callback;				// Set the callback delegate
	m_onEndReqState = extraData;		// Save the context data
}

//---------------------------------------------------------------------------
// RequestHandler::Uri::get
//
// Exposes the internal RequestUri object instance

RequestUri^ RequestHandler::Uri::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_uri;
}

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)