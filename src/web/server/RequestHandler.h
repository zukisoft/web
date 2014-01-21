//---------------------------------------------------------------------------
// RequestHandler Declarations
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

#ifndef __REQUESTHANDLER_H_
#define __REQUESTHANDLER_H_
#pragma once

#include "ClientCertificate.h"			// Include ClientCertificate declarations
#include "RequestUri.h"					// Include RequestUri declarations
#include "ServerCertificate.h"			// Include ServerCertificate declarations
#include "StoragePathProvider.h"		// Include StoragePathProvider declarations
#include "WebServerVersion.h"			// Include WebServerVersion declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;
using namespace System::Globalization;
using namespace System::IO;
using namespace System::Net;
using namespace System::Runtime::InteropServices;
using namespace System::Security;
using namespace System::Security::Permissions;
using namespace System::Security::Principal;
using namespace System::Text;
using namespace System::Web;
using namespace System::Web::Hosting;
using namespace Microsoft::Win32::SafeHandles;
namespace zws = zuki::web::server;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class RequestHandler (internal)
//
// RequestHandler implements the request processing unit for the application.
// The HttpRuntime invokes the methods as necessary to process the request
// for us.  Much of this implementation is simple, thanks to the HttpListener
// and it's fairly robust Request and Response objects.
//
// PRELOADED ENTITY BODIES
//
// This class does not implement any methods regarding preloaded entity bodies.
// The HttpListenerRequest takes care of all the nitty-gritty with client
// communication and chunking.  All this object needs to handle are the
// standard ReadEntityBody methods.
//
// SUPERFLUOUS METHODS
//
// The following methods are not implemented by this class, since they don't
// appear to be implemented in ANY HttpWorkerRequest-based object I can find,
// including the internal IIS6 and IIS7 implementation:
//
// HttpWorkerRequest::GetAppPoolID
// HttpWorkerRequest::GetConnectionID
// HttpWorkerRequest::GetRequestReason
// HttpWorkerRequest::GetUrlContextID
// HttpWorkerRequest::GetVirtualPathToken
//---------------------------------------------------------------------------

ref class RequestHandler : public HttpWorkerRequest
{
public:

	//-----------------------------------------------------------------------
	// Constructor

	RequestHandler(String^ appid, WebApplicationConfiguration^ config, 
		HttpListenerContext^ context, IntPtr token); 

	//-----------------------------------------------------------------------
	// Member Functions

	// CloseConnection (HttpWorkerRequest)
	//
	// Terminates the connection with the client
	virtual void CloseConnection(void) override;

	// EndOfRequest (HttpWorkerRequest)
	//
	// Invoked when the current request is complete
	virtual void EndOfRequest(void) override;

	// FlushResponse (HttpWorkerRequest)
	//
	// Sends all pending response data to the client
	virtual void FlushResponse(bool finalFlush) override;

	// GetAppPath (HttpWorkerRequest)
	//
	// Returns the virtual path to the currently executing server application
	virtual String^ GetAppPath(void) override;

	// GetAppPathTranslated (HttpWorkerRequest)
	//
	// Returns the physical path to the currently executing server application
	virtual String^ GetAppPathTranslated(void) override;

	// GetAppPoolID (HttpWorkerRequest)
	//
	// Returns the application pool ID for the current URL
	virtual String^ GetAppPoolID(void) override;

	// GetBytesRead (HttpWorkerRequest)
	//
	// Gets the number of bytes read in from the client
	virtual __int64 GetBytesRead(void) override;

	// GetClientCertificate (HttpWorkerRequest)
	//
	// Gets the certification fields (specified in the X.509 standard)
	virtual array<Byte>^ GetClientCertificate(void) override;

	// GetClientCertificateBinaryIssuer (HttpWorkerRequest)
	//
	// Gets the certificate issuer, in binary format
	virtual array<Byte>^ GetClientCertificateBinaryIssuer(void) override;

	// GetClientCertificateEncoding (HttpWorkerRequest)
	//
	// Returns the Encoding object in which the client certificate was encoded
	virtual int GetClientCertificateEncoding(void) override;

	// GetClientCertificatePublicKey (HttpWorkerRequest)
	//
	// Gets a PublicKey object associated with the client certificate
	virtual array<Byte>^ GetClientCertificatePublicKey(void) override;

	// GetClientCertificateValidFrom (HttpWorkerRequest)
	//
	// Gets the date when the certificate becomes valid
	virtual DateTime GetClientCertificateValidFrom(void) override;

	// GetClientCertificateValidUntil (HttpWorkerRequest)
	//
	// Gets the certificate expiration date
	virtual DateTime GetClientCertificateValidUntil(void) override;

	// GetConnectionID (HttpWorkerRequest)
	//
	// Returns the ID of the current connection
	virtual __int64 GetConnectionID(void) override;

	// GetFilePath (HttpWorkerRequest)
	//
	// Returns the virtual path to the requested URI
	virtual String^ GetFilePath(void) override;

	// GetFilePathTranslated (HttpWorkerRequest)
	//
	// Returns the physical file path to the requested URI and translates it
	virtual String^ GetFilePathTranslated(void) override;

	// GetHttpVerbName (HttpWorkerRequest)
	//
	// Returns the http verb member of the request header
	virtual String^ GetHttpVerbName(void) override;

	// GetHttpVersion (HttpWorkerRequest)
	//
	// Returns the http version of the request
	virtual String^ GetHttpVersion(void) override;

	// GetKnownRequestHeader (HttpWorkerRequest)
	//
	// Returns the standard HTTP request header that corresponds to the specified index
	virtual String^ GetKnownRequestHeader(int index) override;

	// GetLocalAddress (HttpWorkerRequest)
	//
	// Returns the local address member of the request header
	virtual String^ GetLocalAddress(void) override;

	// GetLocalPort (HttpWorkerRequest)
	//
	// Returns the local port member of the request header
	virtual int GetLocalPort(void) override;

	// GetPathInfo (HttpWorkerRequest)
	//
	// Returns additional path information for a resource with a URL extension
	virtual String^ GetPathInfo(void) override;

	// GetPreloadedEntityBody (HttpWorkerRequest)
	//
	// Returns the portion of the HTTP request body that has already been read
	virtual array<Byte>^ GetPreloadedEntityBody(void) override;

	// GetPreloadedEntityBody (HttpWorkerRequest)
	//
	// Returns the portion of the HTTP request body that has already been read
	virtual int GetPreloadedEntityBody(array<Byte>^ buffer, int offset) override;

	// GetPreloadedEntityBodyLength (HttpWorkerRequest)
	//
	// Gets the length of the HTTP request body that has already been read
	virtual int GetPreloadedEntityBodyLength(void) override;

	// GetProtocol
	//
	// Gets the protocol (HTTP or HTTPS) of the request
	virtual String^ GetProtocol(void) override;

	// GetQueryString (HttpWorkerRequest)
	//
	// Returns the query string specified in the request URL
	virtual String^ GetQueryString(void) override;

	// GetQueryStringRawBytes (HttpWorkerRequest)
	//
	// Returns the response query string as an array of bytes
	virtual array<Byte>^ GetQueryStringRawBytes(void) override;

	// GetRawUrl (HttpWorkerRequest)
	//
	// Returns the URL path with the query string appended
	virtual String^ GetRawUrl(void) override;

	// GetRemoteAddress (HttpWorkerRequest)
	//
	// Returns the remote address member of the request header
	virtual String^ GetRemoteAddress(void) override;

	// GetRemoteName (HttpWorkerRequest)
	//
	// Returns the computer name of the client
	virtual String^ GetRemoteName(void) override;

	// GetRemotePort (HttpWorkerRequest)
	//
	// Returns the remote port member of the request header
	virtual int GetRemotePort(void) override;

	// GetRequestReason (HttpWorkerRequest)
	//
	// Returns the reason for the request
	virtual int GetRequestReason(void) override;

	// GetServerName (HttpWorkerRequest)
	//
	// Returns the name of the local server
	virtual String^ GetServerName(void) override;

	// GetServerVariable (HttpWorkerRequest)
	//
	// Returns a server variable from the dictionary of server variables
	virtual String^ GetServerVariable(String^ name) override;

	// GetTotalEntityBodyLength (HttpWorkerRequest)
	//
	// Gets the length of the entire HTTP request body
	virtual int GetTotalEntityBodyLength(void) override;

	// GetUnknownRequestHeader (HttpWorkerRequest)
	//
	// Returns a nonstandard HTTP request header value
	virtual String^ GetUnknownRequestHeader(String^ name) override;

	// GetUnknownRequestHeaders (HttpWorkerRequest)
	//
	// Get all nonstandard HTTP header name-value pairs
	virtual array<array<String^>^>^ GetUnknownRequestHeaders(void) override;

	// GetUriPath (HttpWorkerRequest)
	//
	// Returns the virtual path to the requested URI
	virtual String^ GetUriPath(void) override;

	// GetUrlContextID (HttpWorkerRequest)
	//
	// Returns the context ID of the current connection
	virtual __int64 GetUrlContextID(void) override;

	// GetUserToken (HttpWorkerRequest)
	//
	// Returns the client's impersonation token
	virtual IntPtr GetUserToken(void) override;

	// GetVirtualPathToken (HttpWorkerRequest)
	//
	// Gets the impersonation token for the request virtual path
	virtual IntPtr GetVirtualPathToken(void) override;

	// HeadersSent (HttpWorkerRequest)
	//
	// Returns a value indicating whether HTTP response headers have been sent
	virtual bool HeadersSent(void) override;

	// IsClientConnected (HttpWorkerRequest)
	//
	// Returns a value indicating whether the client connection is still active
	virtual bool IsClientConnected(void) override;

	// IsEntireEntityBodyIsPreloaded (HttpWorkerRequest)
	//
	// Indicates whether all request data is available and no further reads are required
	virtual bool IsEntireEntityBodyIsPreloaded(void) override;

	// IsSecure (HttpWorkerRequest)
	//
	// Returns a value indicating whether the connection uses SSL
	virtual bool IsSecure(void) override;

	// MapPath (HttpWorkerRequest)
	//
	// Returns the physical path corresponding to the specified virtual path
	virtual String^ MapPath(String^ path) override;

	// ReadEntityBody (HttpWorkerRequest)
	//
	// Reads request data from the client
	virtual int ReadEntityBody(array<Byte>^ buffer, int size) override;

	// ReadEntityBody (HttpWorkerRequest)
	//
	// Reads request data from the client
	virtual int ReadEntityBody(array<Byte>^ buffer, int offset, int size) override;

	// SendCalculatedContentLength (HttpWorkerRequest)
	//
	// Adds a Content-Length HTTP header to the response
	virtual void SendCalculatedContentLength(int contentLength) override;

	// SendKnownResponseHeader (HttpWorkerRequest)
	//
	// Adds a standard HTTP header to the response
	virtual void SendKnownResponseHeader(int index, String^ value) override;

	// SendResponseFromFile (HttpWorkerRequest)
	//
	// Adds the contents of a file to the response
	virtual void SendResponseFromFile(IntPtr handle, __int64 offset, __int64 length) override;

	// SendResponseFromFile (HttpWorkerRequest)
	//
	// Adds the contents of a file to the response
	virtual void SendResponseFromFile(String^ filename, __int64 offset, __int64 length) override;

	// SendResponseFromMemory (HttpWorkerRequest)
	//
	// Adds the contents of a memory block to the response
	virtual void SendResponseFromMemory(array<Byte>^ data, int length) override;

	// SendResponseFromMemory (HttpWorkerRequest)
	//
	// Adds the contents of a memory block to the response
	virtual void SendResponseFromMemory(IntPtr data, int length) override;

	// SendStatus (HttpWorkerRequest)
	//
	// Specifies the HTTP status code and status description of the response
	virtual void SendStatus(int statusCode, String^ statusDescription) override;

	// SendUnknownResponseHeader (HttpWorkerRequest)
	//
	// Adds a nonstandard HTTP header to the response
	virtual void SendUnknownResponseHeader(String^ name, String^ value) override;

	// SetEndOfSendNotification (HttpWorkerRequest)
	//
	// Registers for an optional notification when all the response data is sent
	virtual void SetEndOfSendNotification(EndOfSendNotification^ callback, Object^ extraData) override;

	//-----------------------------------------------------------------------
	// Properties

	// MachineConfigPath (HttpWorkerRequest)
	//
	// Gets the full physical path to the Machine.config file
	virtual property String^ MachineConfigPath
	{
		String^ get(void) override;
	}

	// MachineInstallDirectory (HttpWorkerRequest)
	//
	// Gets the physical path to the directory where the ASP.NET binaries are installed
	virtual property String^ MachineInstallDirectory
	{
		String^ get(void) override;
	}

	// RequestTraceIdentifier (HttpWorkerRequest)
	//
	// Gets the corresponding Event Tracking for Windows trace ID for the current request
	virtual property Guid RequestTraceIdentifier
	{
		Guid get(void) override;
	}

	// RootWebConfigPath (HttpWorkerRequest)
	//
	// Gets the full physical path to the root web.config file
	virtual property String^ RootWebConfigPath
	{
		String^ get(void) override;
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Properties

	// ClientCertificate
	//
	// Accesses the internal ClientCertificate object instance
	property zws::ClientCertificate^ ClientCertificate
	{
		zws::ClientCertificate^ get(void);
	}

	// HasClientCertificate
	//
	// Indicates if the request contains a client certificate or not
	property bool HasClientCertificate
	{
		bool get(void);
	}

	// IsAnonymous
	//
	// Indicates if the request is anonymous or not
	property bool IsAnonymous
	{
		bool get(void);
	}

	// IsDisposed
	//
	// Exposes the object disposal state for debugging purposes
	property bool IsDisposed
	{
		bool get(void) { return m_disposed; }
	}

	// IsStaticContent
	//
	// Exposes the flag indicating if this is static content or not
	property bool IsStaticContent
	{
		bool get(void) { return m_staticContent; }
	}

	// Uri
	//
	// Exposes the internal RequestUri object instance
	property RequestUri^ Uri
	{
		RequestUri^ get(void);
	}

private:

	// STATIC CONSTRUCTOR
	static RequestHandler()
	{
		// Unfortunately, we do need reflection to do some things in here
		(gcnew ReflectionPermission(ReflectionPermissionFlag::MemberAccess))->Demand();

		// We need to hack our way into WebHeaderCollection AGAIN to properly
		// handle Set-Cookie headers. This one's important, better demand the
		// ReflectionPermission this time and not use an empty catch handler
		s_changeInternalHeader = WebHeaderCollection::typeid->GetMethod("ChangeInternal", 
			BindingFlags::Instance | BindingFlags::NonPublic);

		// We also need to get at the internal .SentHeaders property of the 
		// HttpListenerResponse object to properly detect if headers have been sent
		s_sentHeaders = HttpListenerResponse::typeid->GetProperty("SentHeaders",
			BindingFlags::Instance | BindingFlags::NonPublic);
	}

	// DESTRUCTOR
	~RequestHandler();

	//-----------------------------------------------------------------------
	// Private Type Declarations

	// HeaderCollection
	//
	// Typedef to improve readability of the custom request header collection
	typedef Generic::IDictionary<String^, String^>	HeaderCollection;

	//-----------------------------------------------------------------------
	// Private Member Functions

	// AddStaticContentHeaders
	//
	// Adds static content response headers when the VFS has been enabled
	bool AddStaticContentHeaders(void);

	// AppendHeaderWithBigHack
	//
	// Appends a header using the big reflection hack
	void AppendHeaderWithBigHack(String^ name, String^ value);

	// GenerateETag
	//
	// Generates an ETag header value
	static String^ GenerateETag(DateTime modified);

	// SafeOutputWrite
	//
	// Performs an output stream write with exception handling
	void SafeOutputWrite(array<Byte>^ buffer, int offset, int length);

	// SendResponseFromStream
	//
	// Sends the response from the contents of an open stream
	void SendResponseFromStream(Stream^ stream, __int64 offset, __int64 length);

	//-----------------------------------------------------------------------
	// Member Variables

	bool							m_disposed;				// Object disposal flag
	HttpListenerContext^			m_context;				// Listener context object
	RequestUri^						m_uri;					// Request URI information
	String^							m_appid;				// Application ID string
	WebApplicationConfiguration^	m_config;				// Application configuration
	bool							m_staticContent;		// Flag to trap static content
	
	// REQUEST ENTITY BODY
	Stream^							m_requestStream;		// Input stream instance

	// CLIENT/SERVER IDENTITIES
	IntPtr							m_token;				// Authentication token
	zws::ClientCertificate^			m_clientCert;			// Client certificate
	zws::ServerCertificate^			m_serverCert;			// Server certificate

	// CHUNKED RESPONSE STREAM
	MemoryStream^					m_chunkStream;			// Chunked response stream

	// END OF SEND NOTIFICATION
	EndOfSendNotification^			m_onEndReq;				// End of request delegate
	Object^							m_onEndReqState;		// State object for callback

	// CLIENT CONNECTED FLAG
	bool							m_clientConnected;		// Flag if client is connected

	// HEADER HACKS
	static PropertyInfo^			s_sentHeaders;			// HttpListenerResponse.SentHeaders
	static MethodInfo^				s_changeInternalHeader;	// WebHeaderCollection.ChangeInternal

	// STATIC LITERAL ARRAYS
	static initonly array<__wchar_t>^ s_crlf = {'\r', '\n'};
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __REQUESTHANDLER_H_