//---------------------------------------------------------------------------
// Host Declarations
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

#ifndef __HOST_H_
#define __HOST_H_
#pragma once

#include "exceptions.h"						// Include exception declarations
#include "ApplicationRestarter.h"			// Include ApplicationRestarter decls
#include "LogEntry.h"						// Include LogEntry declarations
#include "RequestHandler.h"					// Include RequestHandler declarations
#include "StoragePathProvider.h"			// Include StoragePathProvider decls
#include "UriPrefix.h"						// Include UriPrefix declarations
#include "WebApplicationErrorPage.h"		// Include WebApplicationErrorPage
#include "WebApplicationConfiguration.h"	// Include WebApplicationConfig decls
#include "WebServerVersion.h"				// Include WebServerVersion decls

#pragma warning(push, 4)					// Enable maximum compiler warnings

using namespace System;
using namespace System::Diagnostics;
using namespace System::IO;
using namespace System::Net;
using namespace System::Reflection;
using namespace System::Threading;
using namespace System::Web;
using namespace System::Web::Hosting;
using namespace	msclr;
using namespace zuki::web::server::exceptions;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class Host (internal)
// 
// Host implements the actual web application host instance.  This implementation
// uses the new HttpListener object model that works directly with HTTP.SYS
// on Windows XP Service Pack 2 and higher systems.  The implementation is
// far more simple than that of Cassini of other lower level socket versions
//---------------------------------------------------------------------------

///////////////////////////////////////
//// TEMPORARY: ASYNCHRONOUS MODEL FLAG
////
//// Due to the extreme problems I had getting the HttpListener asynchronous
//// model to work in the first place, I've left the hacked up workaround code
//// in place.  Comment out the #define ASYNCHRONOUS_HOST_MODEL to put the old
//// code back ...
////
#define ASYNCHRONOUS_HOST_MODEL (1)

ref class Host : public MarshalByRefObject, IRegisteredObject
{
public:

	Host();

	//-----------------------------------------------------------------------
	// Member Functions

	// Create
	//
	// Creates an application host instance appropriate for the operating system
	static Host^ Create(ApplicationManager^ manager, AppDomain^ parentDomain, String^ appid, 
		WebApplicationConfiguration^ config, int restarts);

	// GetLoadedAssemblies
	//
	// Generates an array of information about the loaded assemblies
	array<AssemblyName^>^ GetLoadedAssemblies(void);
	
	// InitializeLifetimeService (MarshalByRefObject)
	//
	// Obtains a lifetime service object to control the lifetime policy for this instance
	virtual Object^ InitializeLifetimeService(void) override { return nullptr; }

	//-----------------------------------------------------------------------
	// Properties

	// ApplicationID
	//
	// Returns this host instance's application ID string
	property String^ ApplicationID
	{
		String^ get(void) { return m_appid; }
	}

	// ApplicationManager
	//
	// Gets the OUTER application manager instance, used for restarts
	property System::Web::Hosting::ApplicationManager^ ApplicationManager
	{
		System::Web::Hosting::ApplicationManager^ get(void) { return m_appManager; }
	}

	// Configuration
	//
	// Exposes the configuration of the web application
	property WebApplicationConfiguration^ Configuration
	{
		WebApplicationConfiguration^ get(void) { return m_config; }
	}

	// ParentDomain
	//
	// Gets a reference to the parent AppDomain that created this host
	property AppDomain^ ParentDomain
	{
		AppDomain^ get(void) { return m_parentDomain; }
	}

	// RequestsHandled
	//
	// Number of requests handled by this host since last restart
	property __int64 RequestsHandled
	{
		__int64 get(void) { return m_requests; }
	}

	// RestartCount
	//
	// Indicates the number of times this host has been restarted
	property int RestartCount
	{
		int get(void) { return m_restarts; }
	}

	// StopWaitHandle
	//
	// Provides a reference to a WaitHandle that can be used to wait for stop
	property WaitHandle^ StopWaitHandle
	{
		WaitHandle^ get(void) { return m_stopEvent; }
	}

private:

	// STATIC CONSTRUCTOR
	static Host()
	{
		s_thisAssembly = Host::typeid->Assembly;	// Save this for later
		
		// We need to hack our way into WebHeaderCollection to properly handle
		// HTTP 401 errors on basic authentication requests.  Don't bother to 
		// demand ReflectionPermission, this isn't so important as to blow up

		try { s_addInternalHeader = WebHeaderCollection::typeid->GetMethod("AddInternal", 
			BindingFlags::Instance | BindingFlags::NonPublic); }
		catch(Exception^) { /* DO NOTHING */ }
	}

	// DESTRUCTOR
	~Host();

	//-----------------------------------------------------------------------
	// Private Constants

	literal String^		HTML_UNAUTHORIZED	= "401.htm";
	literal String^		HTML_FORBIDDEN		= "403.htm";
	literal String^		HTML_SSLREQUIRED	= "403-4.htm";
	literal String^		HTML_CERTREQUIRED	= "403-7.htm";
	literal String^		HTML_CERTBADCHAIN	= "403-16.htm";
	literal String^		HTML_CERTEXPIRED	= "403-17.htm";

	//-----------------------------------------------------------------------
	// Private Member Functions

	// AuthenticateUser
	//
	// Creates a logon token for a username/password combination
	IntPtr AuthenticateUser(String^ username, String^ password);

	// Configure
	//
	// Configures the new application host
	void Configure(System::Web::Hosting::ApplicationManager^ appManager, AppDomain^ parentDomain, 
		String^ appid, WebApplicationConfiguration^ config,	int restarts);

	// GetRequestToken
	//
	// Gets the user token to pass onto the Request handler
	IntPtr GetRequestToken(HttpListenerContext^ context, bool% close);

#ifndef ASYNCHRONOUS_HOST_MODEL
	// ListenerThread
	//
	// Entry point for the main HTTP listener worker thread
	void ListenerThread(void);
#endif

	// LoadErrorPage
	//
	// Loads a canned HTML error page from the resources
	static array<Byte>^ LoadErrorPage(String^ page);

	// PreProcessAuthentication
	//
	// Helper function to test/handle request authentication issues
	void PreProcessAuthentication(HttpListenerContext^ context, RequestHandler^ handler, bool% handled);

	// PreProcessClientCertificate
	//
	// Helper function to test/handle request client certificate issues
	void PreProcessClientCertificate(RequestHandler^ handler, bool% handled);

	// PreProcessDefaultDocument
	//
	// Helper function to adjust for/handle a default document request
	void PreProcessDefaultDocument(RequestHandler^ handler, bool% handled);

	// PreProcessMethod
	//
	// Helper function to deal with invalid or simple HTTP methods
	void PreProcessMethod(RequestHandler^ handler, bool% handled);

	// PreProcessRestrictedDirectories
	//
	// Helper function to test/handle request directory security issues
	void PreProcessRestrictedDirectories(RequestHandler^ handler, bool% handled);

	// PreProcessSecureChannel
	//
	// Helper function to test/handle request SSL issues
	void PreProcessSecureChannel(RequestHandler^ handler, bool% handled);

	// PreProcessStaticContent
	//
	// Helper function to deal with HTTP 304 responses
	void PreProcessStaticContent(HttpListenerContext^ context, RequestHandler^ handler, bool% handled);

	// ProcessLogEvent
	//
	// Thread Pool thread used to process a single log event
	void ProcessLogEvent(Object^ state);

	// ProcessRequest
	//
	// Called from the worker thread to process a single request
	void ProcessRequest(Object^ state);

#ifdef ASYNCHRONOUS_HOST_MODEL
	// ProcessRequestAsync
	//
	// Used to process requests in the asynchronous listener model
	static void ProcessRequestAsync(IAsyncResult^ async);
#endif

	// Shutdown (IRegisteredObject)
	//
	// Requests a registered object to unregister
	virtual void Shutdown(bool immediate) sealed = IRegisteredObject::Stop;

	//-----------------------------------------------------------------------
	// Member Variables

	bool							m_disposed;		// Object disposal flag
	String^							m_appid;		// Application ID string
	WebApplicationConfiguration^	m_config;		// Application configuration
	HttpListener^					m_listener;		// HTTP.SYS based listener
	List<String^>^					m_resPaths;		// Restricted paths
	AppDomain^						m_parentDomain;	// Parent application domain
	__int64							m_requests;		// Number of requests processed
	int								m_restarts;		// Number of domain restarts
	bool							m_logging;		// Flag to indicate logging
	ManualResetEvent^				m_stopEvent;	// Event to signal when stopped

	System::Web::Hosting::ApplicationManager^	m_appManager;	// OUTER application mgr

	static Assembly^		s_thisAssembly;			// This class' assembly
	static MethodInfo^		s_addInternalHeader;	// WebHeaderCollection.AddInternal
	
	// STATIC LITERAL ARRAYS

	initonly static array<Byte>^		s_emptyArray		= { };
	initonly static array<__wchar_t>^	s_splitUserName		= { '\\' };
	initonly static array<__wchar_t>^	s_splitPaths		= { '/' };

	initonly static array<String^>^	s_resDirs = { "/app_browsers", "/app_code", "/app_data",
		"/app_globalresources", "/app_localresources", "/app_webreferences", "/bin" };

	initonly static String^	s_ifModifiedSince = 
		HttpWorkerRequest::GetKnownRequestHeaderName(HttpWorkerRequest::HeaderIfModifiedSince);
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __HOST_H_