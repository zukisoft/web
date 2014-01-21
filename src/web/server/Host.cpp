//---------------------------------------------------------------------------
// Host Implementation
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
#include "Host.h"						// Include Host declarations
#include "WebServer.h"					// Include WebServer declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Host Constructor (protected)
//
// Arguments:
//
//	NONE

Host::Host(void)
{
	m_listener = gcnew HttpListener();				// Create listener
	m_stopEvent = gcnew ManualResetEvent(false);	// Create the event
	HostingEnvironment::RegisterObject(this);		// Register ourselves
}

//---------------------------------------------------------------------------
// Host Destructor

Host::~Host()
{
	HostingEnvironment::UnregisterObject(this);		// Unregister ourselves
	m_stopEvent->Set();								// We're totally dead now
	m_disposed = true;								// Set as disposed
}

//---------------------------------------------------------------------------
// Host::AuthenticateUser (private)
//
// Attempts to create a logon token for the username and password specified.
// If no domain has been provided as part of the user name, it is derived
// from the configuration's DefaultDomain property
//
// Arguments:
//
//	username	- User account name to be authenticated
//	password	- User password

IntPtr Host::AuthenticateUser(String^ username, String^ password)
{
	PinnedStringPtr			pinUserName;		// Pinned user name
	PinnedStringPtr			pinPassword;		// Pinned password
	PinnedStringPtr			pinDomain;			// Pinned domain name
	HANDLE					hToken = NULL;		// Logon token handle

	Debug::Assert(m_config != nullptr);			// Let's hope not

	// The username cannot be NULL or blank, and the password cannot be NULL

	if(String::IsNullOrEmpty(username)) return IntPtr::Zero;
	if(password == nullptr) return IntPtr::Zero;

	pinPassword = PtrToStringChars(password);	// Pin the password string

	// If the username is in DOMAIN\USERNAME format, we need to break it up
	// into it's separate components for LogonUser

	if(username->IndexOf('\\') >= 0) {

		array<String^>^ parts = username->Split(s_splitUserName);
		pinDomain = PtrToStringChars(parts[0]);
		pinUserName = PtrToStringChars(parts[1]);
	}

	// If the username is in UPN format (user@domain), we must provide NULL
	// as the domain argument to LogonUser

	else if(username->IndexOf('@') >= 0) {

		pinUserName = PtrToStringChars(username);
		pinDomain = nullptr;
	}

	// In all other cases, we use the default domain specified in the
	// WebApplicationConfiguration for this host instance

	else {

		pinUserName = PtrToStringChars(username);
		pinDomain = PtrToStringChars(m_config->Authentication->DefaultDomain);
	}

	// Attempt to create a NETWORK logon token for the provided credentials,
	// and return a wrapped handle. Since we only support XPSP2 and 2003,
	// we don't need to be concerned about the old SE_TCB_NAME requirement

	return (LogonUser(pinUserName, pinDomain, pinPassword, LOGON32_LOGON_NETWORK, 
		LOGON32_PROVIDER_DEFAULT, &hToken)) ? IntPtr(hToken) : IntPtr::Zero;
}

//---------------------------------------------------------------------------
// Host::Configure (private)
//
// Configures the application host after it's been created in the target
// application domain by the application manager
//
// Arguments:
//
//	appManager		- OUTER application manager instance (for restarting)
//	parentDomain	- Parent application domain (for events)
//	appid			- Application identifier string
//	config			- Application configuration information
//	restarts		- Number of times this host has been restarted

void Host::Configure(System::Web::Hosting::ApplicationManager^ appManager, 
	AppDomain^ parentDomain, String^ appid, WebApplicationConfiguration^ config,
	int restarts)
{
	WebApplicationStartingEvent^	starting;		// STARTING event handler
	WebApplicationStartedEvent^		started;		// STARTED event handler

	CHECK_DISPOSED(m_disposed);

	if(appManager == nullptr) throw gcnew ArgumentNullException();
	if(parentDomain == nullptr) throw gcnew ArgumentNullException();
	if(String::IsNullOrEmpty(appid)) throw gcnew ArgumentNullException();
	if(config == nullptr) throw gcnew ArgumentNullException();

	// Save the application ID string and the configuration data.  It's no
	// longer necessary to create a new instance of WebAppConfiguration since
	// it's been marked as [Serializable()] and we're getting a copy already

	m_appid = appid;
	m_parentDomain = parentDomain;
	m_config = config;
	m_appManager = appManager;
	m_logging = (m_config->LoggingFormat != LoggingFormat::None);
	m_restarts = restarts;

	// Configure the HTTP listener based on the configuration data provided by 
	// the calling application. Note that exceptions here should be thrown OK
	// back to the calling AppDomain since Win32Exception is serializable

	// 03.15.2012 - Changed this to FALSE; streaming applications would live forever
	// and continue to send data if all exceptions are ignored. RequestHandler::SafeOutputWrite() 
	// will now set IsClientConnected to false after any write exceptions
	m_listener->IgnoreWriteExceptions = false;
	m_listener->AuthenticationSchemes = config->Authentication->Schemes;
	m_listener->Realm = config->Authentication->Realm;
	m_listener->UnsafeConnectionNtlmAuthentication = config->Authentication->CacheNtlmCredentials;

	for each(UriPrefix^ prefix in m_config->Prefixes) {

		// If the prefix has been set up to use a dynamically allocated port,
		// assign it to our copy of the configuration right before we add the
		// prefix to the listener

		if(prefix->Port == 0) prefix->Port = UriPrefixPort(ProjectUtil::FindOpenTcpPort());
		m_listener->Prefixes->Add(prefix->ToString(m_config->VirtualRoot));
	}

	// Set up the restricted path information, which is a combination of the
	// virtual root and the restricted path names.  The standard ASP.NET paths
	// are added by default, and the application can add more as it sees fit

	m_resPaths = gcnew List<String^>();
	for each(String^ resDir in s_resDirs) m_resPaths->Add(m_config->VirtualRoot + resDir);

	for each(String^ resDir in m_config->RestrictedDirectories) {

		if(!resDir->StartsWith("/")) resDir = "/" + resDir;
		m_resPaths->Add(m_config->VirtualRoot + resDir);
	}

	// Attempt to set up the virtual path provider as necessary for this app.  Note that
	// the runtime gives us no indication if it was installed or not, but currently the
	// only way this will fail is if the ASP.NET application is precompiled (see MSDN)
	
	if(config->VirtualFileSystem->Type == VirtualFileSystemType::StructuredStorage) {

		HostingEnvironment::RegisterVirtualPathProvider(gcnew StoragePathProvider(config));
		if(HostingEnvironment::VirtualPathProvider->GetType() != StoragePathProvider::typeid)
			throw gcnew StorageProviderNotInstalledException();
	}

	// Prior to the listener actually starting, give the host application a chance
	// to manipulate this application first.  This is useful for ensuring things like
	// registered objects are created before clients can connect to the server

	starting = gcnew WebApplicationStartingEvent(m_parentDomain, m_appid, gcnew WebApplicationEventArgs(this));
	starting->Invoke();

	m_listener->Start();				// <--- START THE HTTP LISTENER INSTANCE

#ifdef ASYNCHRONOUS_HOST_MODEL

	// Start listening for requests asynchronously using the mechanisms built into the
	// HttpListener class (maybe it will actually work better this time)

	m_listener->BeginGetContext(gcnew AsyncCallback(&Host::ProcessRequestAsync), this);

#else

	// To combat problems with the HttpListener's asynchronous model (which it seems
	// nobody can get to work properly), I've opted to go with a poor man's thread
	// pool implementation instead now.  The requests will be received synchronously
	// and then handed off to the thread pool for processing.  A slight hit, I know,
	// but at least it can be STABLE now.  Gotta go with 'works' over 'fastest'.

	Thread^ listenerThread = gcnew Thread(gcnew ThreadStart(this, &Host::ListenerThread));
	listenerThread->IsBackground = true;
	listenerThread->Priority = ThreadPriority::AboveNormal;
	listenerThread->Start();

#endif

	// Now that everything started up successfully, inform the main WebServer
	// class so it can invoke the necessary event(s) in it's AppDomain

	started = gcnew WebApplicationStartedEvent(m_parentDomain, m_appid, gcnew WebApplicationEventArgs(this));
	started->Invoke();
}

//---------------------------------------------------------------------------
// Host::Create (static)
//
// Creates an application host appropriate for the current operating system
//
// Arguments:
//
//	manager			- ApplicationManager instance to use for creation
//	parentDomain	- Parent application domain (for events)
//	appid			- Application identifier string
//	config			- Application configuration information
//	restarts		- Number of times this host has been restarted

Host^ Host::Create(System::Web::Hosting::ApplicationManager^ manager, AppDomain^ parentDomain, 
	String^ appid, WebApplicationConfiguration^ config, int restarts)
{
	Host^			host;					// Newly created Host instance
	lock			cs(Host::typeid);		// Synchronize this method

	if(manager == nullptr) throw gcnew ArgumentNullException();
	if(parentDomain == nullptr) throw gcnew ArgumentNullException();
	if(String::IsNullOrEmpty(appid)) throw gcnew ArgumentNullException();
	if(config == nullptr) throw gcnew ArgumentNullException();

	// Ask the ApplicationManager to create the new application host for us
	// in it's own new application domain.  Note the toggle between HttpHost
	// and TcpHost depending on if HttpListener is supported here or not

	host = safe_cast<Host^>(manager->CreateObject(appid, Host::typeid, 
		config->VirtualRoot, config->PhysicalRootFull, true, true));

	// Now attempt to configure the new host, since we couldn't give it
	// a constructor argument.  If anything goes wrong, destroy the host.

	try { host->Configure(manager, parentDomain, appid, config, restarts); }
	catch(Exception^) { manager->ShutdownApplication(appid); throw; }

	return host;						// Return the Host instance
}

//---------------------------------------------------------------------------
// Host::GetLoadedAssemblies
//
// Generates an array of information about all the assemblies loaded into
// the hosted application domain
//
// Arguments:
//
//	NONE

array<AssemblyName^>^ Host::GetLoadedAssemblies(void)
{
	// Get the array of Assembly instances (which are not marshalable) and create
	// an array to store their AssemblyName instances (which are) ...

	array<Assembly^>^ assemblies = AppDomain::CurrentDomain->GetAssemblies();
	array<AssemblyName^>^ names = gcnew array<AssemblyName^>(assemblies->Length);

	// Copy the AssemblyName instances from the non-marshalable array into 
	// something that can be returned back to the WebApplication instance

	for(int index = 0; index < assemblies->Length; index++)
		names[index] = assemblies[index]->GetName();

	return names;						// Return the generated array
}

//---------------------------------------------------------------------------
// Host::GetRequestToken (private)
//
// Gets the logon token to be passed onto the Request object.  Returns
// IntPtr::Zero if the request is anonymous or a token cannot be acquired.
// The token must be eventually closed when close is set to true
//
// Arguments:
//
//	context		- HttpListenerContext for the pending request
//	close		- Flag to close the token when finished with it

IntPtr Host::GetRequestToken(HttpListenerContext^ context, bool% close)
{
	HttpListenerBasicIdentity^		basicIdentity;		// Basic auth identity

	close = false;					// Initialize [out] variable

	if((context->User == nullptr) || (context->User->Identity == nullptr)) return IntPtr::Zero;

	// If the context contains a WindowsIdentity, we're good to go and we
	// can just return that token for request impersonation

	if(context->User->Identity->GetType() == WindowsIdentity::typeid)
		return dynamic_cast<WindowsIdentity^>(context->User->Identity)->Token;

	// Basic authentication is a bit more challenging, since all we get are
	// the user name and password, and the application has to authenticate them

	if(context->User->Identity->GetType() == HttpListenerBasicIdentity::typeid) {

		basicIdentity = dynamic_cast<HttpListenerBasicIdentity^>(context->User->Identity);
		
		try { return AuthenticateUser(basicIdentity->Name, basicIdentity->Password); }
		catch(Exception^) { return IntPtr::Zero; }
	}

	return IntPtr::Zero;				// <--- Not a known Identity type
}

//---------------------------------------------------------------------------
// Host::ListenerThread (private)
//
// Implements the worker thread that listens for and accepts new requests.
// Note that this is NOT how I originally had this, not by a long shot.  There
// seem to be some issues with the HttpListener asynchronous model that were
// causing me more problems than I could afford to deal with, so I put this
// in place instead.  IF they fix or better document HttpListener, there is
// no reason to have a worker thread at all ....
//
// Arguments:
//
//	NONE

#ifndef ASYNCHRONOUS_HOST_MODEL

void Host::ListenerThread(void)
{
	// This thread doesn't control the listener start/stop, it just keeps on
	// trucking along as long as it's actually listening for client requests

	while(m_listener->IsListening) {

		try { 
			
			// Since the async mode seems horribly screwed up in HttpListener, we can
			// simulate asynchronous behavior by accepting the request synchronously
			// and handing it off to a thread pool thread for processing.  This does
			// appear to function fairly well, and with minimal additional overhead

			ThreadPool::QueueUserWorkItem(gcnew WaitCallback(this, &Host::ProcessRequest), 
				m_listener->GetContext());
		}

		// Due to our use of a synchronous model here, what tends to happen is that
		// when the listener is being stopped, all hell breaks loose and tons of
		// listener exceptions are thrown because IsListening hasn't been toggled to
		// false yet.  While super cheesy and lame, pause a little on every exception
		// to let the listener catch up to us.  (Like this whole listener model isn't
		// 'super cheesy and lame' already ... PLEASE FIX THE ASYNC STUFF MICROSOFT!)

		catch(Exception^) { Thread::Sleep(50); continue; }
	}
}

#endif

//---------------------------------------------------------------------------
// Host::LoadErrorPage (private, static)
//
// Loads a canned HTML error response page from the resources.  Replacement
// strings are handled directly in here, at least for now.
//
// Arguments:
//
//	page			- Name of the HTML page to be loaded

array<Byte>^ Host::LoadErrorPage(String^ page)
{
	StreamReader^		reader = nullptr;			// Resource reader

	try {

		// The only replacement string currently in place is SERVER_SOFTWARE, so just
		// handle it inline with everything else until such time there are more ...

		reader = gcnew StreamReader(s_thisAssembly->GetManifestResourceStream(page));
		return Encoding::UTF8->GetBytes(reader->ReadToEnd()->Replace("%SERVER_SOFTWARE%", 
			WebServerVersion::SoftwareVersion));
	}

	catch(Exception^) { return s_emptyArray; }
	finally { if(reader != nullptr) reader->Close(); }
}

//---------------------------------------------------------------------------
// Host::PreProcessAuthentication (private)
//
// Checks the authentication status of the request.  If the request is
// anonymous and the site doesn't allow it, an HTTP 401 will be sent
//
// Arguments:
//
//	context		- HTTP listener context instance
//	handler		- RequestHandler instance being processed
//	handled		- Flag set if the request was completely handled

void Host::PreProcessAuthentication(HttpListenerContext^ context, RequestHandler^ handler,
	bool% handled)
{
	handled = false;					// Assume this was not handled

	if((!m_config->AllowsAnonymous) && (handler->IsAnonymous)) {

		// Unfortunately, we have to use reflection to add the WWW-Authenticate
		// header to the response for HTTP 401 errors ....

		try { s_addInternalHeader->Invoke(context->Response->Headers, gcnew array<Object^>{"WWW-Authenticate", 
			String::Format("Basic Realm=\"{0}\"", m_config->Authentication->Realm)}); }
		catch(Exception^) { /* DO NOTHING */ }

		handler->SendStatus(401, HttpWorkerRequest::GetStatusDescription(401));
		handler->SendResponseFromMemory(LoadErrorPage(HTML_UNAUTHORIZED), -1);
		handler->EndOfRequest();
		
		handled = true;				// Request was handled in pre-processing
	}
}

//---------------------------------------------------------------------------
// Host::PreProcessClientCertificate (private)
//
// Checks the client certificate status of the request.  If anything is wrong
// with the certificate, like it's not there, an HTTP 403 will be sent back
//
// Arguments:
//
//	handler		- RequestHandler instance being processed
//	handled		- Flag set if the request was completely handled

void Host::PreProcessClientCertificate(RequestHandler^ handler, bool% handled)
{
	handled = false;					// Assume this was not handled

	// CHECK FOR REQUIRED CLIENT CERTIFICATE
	if((m_config->Authentication->RequireClientCertificate) && (!handler->HasClientCertificate)) {

		handler->SendStatus(403, HttpWorkerRequest::GetStatusDescription(403));
		handler->SendResponseFromMemory(LoadErrorPage(HTML_CERTREQUIRED), -1);
		handler->EndOfRequest();

		handled = true;				// Request was handled in pre-processing
		return;						// No further processing is required
	}

	if(!handler->HasClientCertificate) return;		// <--- NO NEED TO CONTINUE ON

	// CHECK CERTIFICATE EXPIRATION
	if((m_config->Authentication->VerifyClientCertificateExpiration) && (!handler->ClientCertificate->VerifyDates())) {

		handler->SendStatus(403, HttpWorkerRequest::GetStatusDescription(403));
		handler->SendResponseFromMemory(LoadErrorPage(HTML_CERTEXPIRED), -1);
		handler->EndOfRequest();

		handled = true;				// Request was handled in pre-processing
		return;						// No further processing is required
	}

	// CHECK CERTIFICATE CHAIN
	if((m_config->Authentication->VerifyClientCertificateChain) && (!handler->ClientCertificate->VerifyChain())) {

		handler->SendStatus(403, HttpWorkerRequest::GetStatusDescription(403));
		handler->SendResponseFromMemory(LoadErrorPage(HTML_CERTBADCHAIN), -1);
		handler->EndOfRequest();

		handled = true;				// Request was handled in pre-processing
		return;						// No further processing is required
	}
}

//---------------------------------------------------------------------------
// Host::PreProcessDefaultDocument (private)
//
// Checks for a default document request and adjusts the URI as necessary.
// Can also redirect the client browser as needed for a trailing slash issue.
//
// Arguments:
//
//	handler		- RequestHandler instance being processed
//	handled		- Flag set if the request was completely handled

void Host::PreProcessDefaultDocument(RequestHandler^ handler, bool% handled)
{
	String^					uriPath;		// Pulled out from request handler

	handled = false;						// Assume this was not handled
	uriPath = handler->Uri->UriPath;		// Pull this out to avoid function calls

	if(HostingEnvironment::VirtualPathProvider->DirectoryExists(uriPath)) {

		// If the directory exists, but the URL didn't end with a slash, we have
		// to redirect the client browser to prevent mucking up the ~ virtual path
		// operator provided by ASP.NET (trust me -- Cassini also does this)

		if(!uriPath->EndsWith("/")) {

			handler->Uri->AppendPath("/");
			handler->SendStatus(302, HttpWorkerRequest::GetStatusDescription(302));
			handler->SendKnownResponseHeader(HttpWorkerRequest::HeaderLocation, handler->Uri->RawUrl);
			handler->EndOfRequest();

			handled = true;				// Request has been completely handled
			return;
		}

		// The path was legit, test for a default document in that directory and
		// if we find one, silently append it to the request path before handing it off.
		// As noted above, my hokey design allows you to do this dynamically ...

		for each(String^ doc in m_config->DefaultDocuments) {

			if(HostingEnvironment::VirtualPathProvider->FileExists(uriPath + doc)) { 
				
				handler->Uri->AppendPath(doc);		// Append the default document
				break;								// Break the loop
			}
		}
	}
}

//---------------------------------------------------------------------------
// Host::PreProcessMethod (private)
//
// Handler for non-standard methods like OPTIONS that have a simple fixed
// response or require us to send an HTTP 405 back to the client
//
// Arguments:
//
//	handler		- RequestHandler instance being processed
//	handled		- Flag set if the request was completely handled

void Host::PreProcessMethod(RequestHandler^ handler, bool% handled)
{
	String^				verb;				// HTTP verb name

	handled = true;							// Assume this was handled
	verb = handler->GetHttpVerbName();		// Grab the verb name

	// CONNECT
	//
	// Connect is reserved for use with a proxy, we don't do anything with it
	if(String::Compare(verb, "CONNECT", StringComparison::OrdinalIgnoreCase) == 0) {

		handler->SendStatus(405, HttpWorkerRequest::GetStatusDescription(405));
		handler->EndOfRequest();
	}

	// OPTIONS
	//
	// Requests information about the communication options available from this server
	else if(String::Compare(verb, "OPTIONS", StringComparison::OrdinalIgnoreCase) == 0) {

		handler->SendStatus(405, HttpWorkerRequest::GetStatusDescription(405));
		handler->EndOfRequest();
	}

	// TRACE
	//
	// Used for diagnostic purposes, we're supposed to echo the request back as the
	// response with a status of 200 OK, but it's hardly worth implementing here
	else if(String::Compare(verb, "TRACE", StringComparison::OrdinalIgnoreCase) == 0) {

		handler->SendStatus(405, HttpWorkerRequest::GetStatusDescription(405));
		handler->EndOfRequest();
	}

	else handled = false;		// Not something we care to deal with here
}

//---------------------------------------------------------------------------
// Host::PreProcessRestrictedDirectories (private)
//
// Checks the client certificate status of the request.  If anything is wrong
// with the certificate, like it's not there, an HTTP 403 will be sent back
//
// Arguments:
//
//	handler		- RequestHandler instance being processed
//	handled		- Flag set if the request was completely handled

void Host::PreProcessRestrictedDirectories(RequestHandler^ handler, bool% handled)
{
	handled = false;					// Assume this was not handled

	for each(String^ path in m_resPaths) {

		if(handler->Uri->UriPath->StartsWith(path, StringComparison::OrdinalIgnoreCase)) {

			handler->SendStatus(403, HttpWorkerRequest::GetStatusDescription(403));
			handler->SendResponseFromMemory(LoadErrorPage(HTML_FORBIDDEN), -1);
			handler->EndOfRequest();

			handled = true;				// Request was completely handled
			return;						// Break loop and return to caller
		}
	}
}

//---------------------------------------------------------------------------
// Host::PreProcessSecureChannel (private)
//
// Checks the SSL status of the request.  If the request is insecure and
// the site doesn't allow it, an HTTP 403 will be sent to the client browser
//
// Arguments:
//
//	handler		- RequestHandler instance being processed
//	handled		- Flag set if the request was completely handled

void Host::PreProcessSecureChannel(RequestHandler^ handler,	bool% handled)
{
	handled = false;					// Assume this was not handled

	if((m_config->RequireSecureChannel) && (!handler->IsSecure())) {

		handler->SendStatus(403, HttpWorkerRequest::GetStatusDescription(403));
		handler->SendResponseFromMemory(LoadErrorPage(HTML_SSLREQUIRED), -1);
		handler->EndOfRequest();

		handled = true;				// Request was handled in pre-processing
	}
}

//---------------------------------------------------------------------------
// Host::PreProcessStaticContent (private)
//
// Handles the HTTP 304 Not Modified response for static content while under
// the control of the virtual file system
//
// Arguments:
//
//	context		- HttpListenerContext for this request
//	handler		- RequestHandler instance being processed
//	handled		- Flag set if the request was completely handled

void Host::PreProcessStaticContent(HttpListenerContext^ context, RequestHandler^ handler, bool% handled)
{
	StoragePathProvider^	provider;			// Custom virtual path provider
	String^					ifModifiedSince;	// If-Modified-Since header
	DateTime				since;				// If-Modified-Since as a DateTime

	handled = false;					// Assume this was not handled

	// If this isn't static content that we're handling internally, bail

	if(!handler->IsStaticContent) return;

	// Grab a reference to the virtual path provider, and if it's not ours
	// we just allow the HttpRuntime to deal with caching on it's own, since
	// it seems to work just fine until the custom provider is involved.

	provider = dynamic_cast<StoragePathProvider^>(HostingEnvironment::VirtualPathProvider);
	if(provider == nullptr) return;

	// The only header we're concerned with is If-Modified-Since.  Even though
	// RequestHandler sends across an ETag for completeness, we don't check it here

	ifModifiedSince = context->Request->Headers[s_ifModifiedSince];
	if(String::IsNullOrEmpty(ifModifiedSince)) return;

	try {

		// If the date/time of the virtual file system store is IDENTICAL to the
		// If-Modified-Since header send an HTTP 304 Not Modified to the client.
		// Do it this way in case the store was backdated. The idea is to only cache
		// the exact same things, not really deal with a "since" situation ...

		since = DateTime::Parse(ifModifiedSince);
		if(provider->StoreDateTime == since.ToUniversalTime()) {

			handler->SendStatus(304, HttpWorkerRequest::GetStatusDescription(304));
			handler->EndOfRequest();
			handled = true;
		}
	}

	catch(Exception^) { return; }		// <-- Allow runtime to handle it on exception
}

//---------------------------------------------------------------------------
// Host::ProcessLogEvent (private)
//
// Thread Pool thread to deal with an activity log event asynchronously
//
// Arguments:
//
//	state			- Activity log message

void Host::ProcessLogEvent(Object^ state)
{
	String^							logEntry;		// The log entry string
	WebApplicationActivityEvent^	activity;		// The event to be fired

	// First cast out the System::String reference, and bail if it's no good

	logEntry = dynamic_cast<String^>(state);
	if(String::IsNullOrEmpty(logEntry)) return;
	
	try {
		
		// Attempt to create and invoke a new WebApplicationActivityEvent.  Since this
		// is asynchronous in nature and we're directly accessing member variables, need
		// to use yet another generic catch handler.  Nothing about this is important enough
		// to cause any real issues if something has disappeared on us (but I still don't
		// like doing it this way, believe it or not)

		activity = gcnew WebApplicationActivityEvent(m_parentDomain, m_appid, logEntry);
		activity->Invoke();
	}
	
	catch(Exception^) { /* DO NOTHING */ }
}

//---------------------------------------------------------------------------
// Host::ProcessRequest (private)
//
// Processes a single server request that was accepted by the worker thread 
//
// Arguments:
//
//	state			- Request/Response context object to be processed

void Host::ProcessRequest(Object^ state)
{
	HttpListenerContext^		context;			// Listener context object
	bool						handled;			// Pre-processing flag
	IntPtr						token;				// Impersonation token
	bool						closeToken;			// Flag to close the token
	RequestHandler^				handler;			// Request handler
	LogEntry^					logEntry;			// Activity log entry

	CHECK_DISPOSED(m_disposed);

	context = safe_cast<HttpListenerContext^>(state);	// Cast out the context object
	token = GetRequestToken(context, closeToken);		// Get impersonation token

	// If logging has been enabled, create the logging entry instance.  It needs to
	// access information from the Request, which ends up getting disposed of, so it's
	// unfortunately a two-part process AND an object creation to further waste time.
	// This can't handle entries for 401s and whatnot generated by the HttpListener
	// either, at least not without much pain and agony.  I'll work on that later.

	if(m_logging) logEntry = gcnew LogEntry(m_config->LoggingFormat, context);
	
	try {

		// CREATE REQUEST HANDLER INSTANCE
		handler = gcnew RequestHandler(m_appid, m_config, context, token);

		// CHECK METHOD
		PreProcessMethod(handler, handled);
		if(handled) return;

		// CHECK AUTHENTICATION STATUS
		PreProcessAuthentication(context, handler, handled);
		if(handled) return;

		// CHECK SSL STATUS
		PreProcessSecureChannel(handler, handled);
		if(handled) return;

		// CHECK CLIENT CERTIFICATE STATUS
		PreProcessClientCertificate(handler, handled);
		if(handled) return;

		// CHECK RESTRICTED DIRECTORIES
		PreProcessRestrictedDirectories(handler, handled);
		if(handled) return;

		// CHECK FOR DEFAULT DOCUMENT
		PreProcessDefaultDocument(handler, handled);
		if(handled) return;

		// CHECK FOR CACHEABLE STATIC CONTENT
		PreProcessStaticContent(context, handler, handled);
		if(handled) return;

		// PROCESS REQUEST NORMALLY
		HttpRuntime::ProcessRequest(handler);

		// GENERATE LOGS AND STATISTICS
		Interlocked::Increment(m_requests);
	}

	finally { 
	
		// Before leaving, fire off a thread pool worker to deal with the logging

		if(logEntry != nullptr) ThreadPool::QueueUserWorkItem(gcnew WaitCallback(this, 
			&Host::ProcessLogEvent), logEntry->Complete(context->Response));

		if(closeToken) CloseHandle(token.ToPointer());		// Close logon token
	}
}

//---------------------------------------------------------------------------
// Host::ProcessRequestAsync (private, static)
//
// Used in conjunction with the Asynchronous request processing model, which
// didn't work very well for me in the past, so it's really just a wrapper
// around the existing ProcessRequest() method.  Not that there's anything
// wrong with that, but if you still see #ifdef ASYNCHRONOUS_HOST_MODEL in
// this code, I'm still worried that it won't work properly again.
//
// Arguments:
//
//	async			- IAsyncResult instance for this request

#ifdef ASYNCHRONOUS_HOST_MODEL

void Host::ProcessRequestAsync(IAsyncResult^ result)
{
	Host^					instance;						// Invoking Host instance
	HttpListener^			listener;						// Listener instance
	HttpListenerContext^	context = nullptr;				// Asynchronous context

	// The call to BeginGetContext needed to pass the parent Host instance in as the
	// argument so we can lock on it, get the listener, etc, etc, etc ...

	instance = safe_cast<Host^>(result->AsyncState);
	if(instance == nullptr) throw gcnew ArgumentNullException();

	listener = instance->m_listener;				// Pull this out for clarity below

	// One of the fatal flaws from the first time I tried this was an ObjectDisposedException
	// that happened at EndGetContext.  By locking the call to stop the listener and checking
	// to make sure it's actually running first, that problem seems to go away.  Of course,
	// that wasn't the BIG problem I was having with this model, but we'll see what happens

	lock cs(instance);
	try { 

		if(listener->IsListening) {

			context = listener->EndGetContext(result);
			listener->BeginGetContext(gcnew AsyncCallback(&Host::ProcessRequestAsync), instance);
		}
	}

	finally { cs.release(); }				// Always release the critical section

	if(context != nullptr) instance->ProcessRequest(context);	// PROCESS REQUEST
}

#endif	// ASYNCHRONOUS_HOST_MODEL

//---------------------------------------------------------------------------
// Host::Shutdown (private)
//
// Stops and unregisters the host
//
// Arguments:
//
//	immediate		- Flag indicating if object should be unregistered or not

void Host::Shutdown(bool)
{
	WebApplicationStoppedEvent^	stopped;						// STOPPED event handler

	CHECK_DISPOSED(m_disposed);

#ifdef ASYNCHRONOUS_HOST_MODEL

	// Wait for pending requests to all finish and shut down the listener.  Prevent
	// the asynchronous ProcessRequestAsync() method from calling EndGetContext or
	// trying to invoke a new BeginGetContext by locking on ourselves

	lock cs(this);

	try { m_listener->Close(); }				// Close the listener instance
	finally { cs.release(); }					// Always release the critical section			
	
#else

	// Wait for pending requests to all finish and shut down the listener.  This
	// can cause I/O exceptions from time to time that really aren't important
	// since it's being shut down anyway, so once again we have one of those
	// horrible generic catch-and-ignore handlers here

	try { m_listener->Close(); }
	catch(Exception^) { /* DO NOTHING */ }

#endif

	// Invoke the static event hook on the main WebServer class to inform the
	// calling application that this application has been stopped

	stopped = gcnew WebApplicationStoppedEvent(m_parentDomain, m_appid, 
		gcnew WebApplicationEventArgs(this));
	stopped->Invoke();

	// If the application host was shut down automatically for some reason
	// (reason will be 'None' on a manual shutdown), attempt to automatically
	// restart it with the assistance of the ApplicationRestarter class
	//
	// Now a manual shutdown comes through as 'HostingEnvironment', not 'None'.
	// I assume this must have happened in .NET 2.0 Service Pack 1, but regardless
	// we need to look for both reason codes now, otherwise the host will restart
	// as soon as it's shut down, which is probably not a very good thing at all

	ApplicationShutdownReason reason = HostingEnvironment::ShutdownReason;
	if((reason != ApplicationShutdownReason::None) && (reason != ApplicationShutdownReason::HostingEnvironment))
		if(m_config->AutoRestart) ApplicationRestarter::Restart(this);

	delete this;					// Destroy ourselves on shutdown
}

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)


