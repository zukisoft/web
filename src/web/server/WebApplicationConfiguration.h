//---------------------------------------------------------------------------
// WebApplicationConfiguration Declarations
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

#ifndef __WEBAPPLICATIONCONFIGURATION_H_
#define __WEBAPPLICATIONCONFIGURATION_H_
#pragma once

#include "exceptions.h"							// Include custom exception declarations
#include "AuthenticationConfiguration.h"		// Include AuthenticationConfiguration
#include "LoggingFormat.h"						// Include LoggingForamt declarations
#include "ReadOnlyDictionary.h"					// Include ReadOnlyDictionary declarations
#include "StorageConnectionStringBuilder.h"		// Include StorageConnectionStringBuilder
#include "UriPrefixCollection.h"				// Include UriPrefixCollection decls
#include "VirtualFileSystemConfiguration.h"		// Include VirtualFileSystemConfiguration
#include "XmlConstants.h"						// Include XML constant declarations

#pragma warning(push, 4)						// Enable maximum compiler warnings

using namespace System;
using namespace System::IO;
using namespace System::Net;
using namespace System::Security::Permissions;
using namespace System::Xml;
using namespace zuki::data::structured;
using namespace zuki::web::server::exceptions;
namespace zws = zuki::web::server;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Constants / Macros
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Class WebApplicationConfiguration
//
// WebApplicationConfiguration is used to provide all of the information
// necessary for a hosted application to be created on the server.  This is
// also passed into the Host across application domains for it's setup, so
// everything needs to be MarshalByRef or provide a marshaling implementation
//---------------------------------------------------------------------------

[Serializable()]
public ref class WebApplicationConfiguration sealed
{
public:

	WebApplicationConfiguration();

	//-----------------------------------------------------------------------
	// Properties

	// Authentication
	//
	// Accesses the contained AuthenticationConfiguration class
	property AuthenticationConfiguration^ Authentication
	{
		AuthenticationConfiguration^ get(void) { return m_authConfig; }
	}

	// AutoRestart
	//
	// Gets/set the flag that controls the application's restart ability
	property bool AutoRestart
	{
		bool get(void) { return m_restart; }
		void set(bool value);
	}

	// CustomRequestHeaders
	//
	// Accesses the dictionary of STATIC request headers that will be
	// automatically set on every incoming request to this application
	property Generic::IDictionary<String^, String^>^ CustomRequestHeaders
	{
		Generic::IDictionary<String^, String^>^ get(void);
	}

	// DefaultDocuments
	//
	// Accesses the collection of default documents to search for when
	// a user accesses the root of the application (/)
	property Generic::IList<String^>^ DefaultDocuments
	{
		Generic::IList<String^>^ get(void); 
	}

	// FromXml
	//
	// Reads the configuration as an XML document
	static WebApplicationConfiguration^ FromXml(Stream^ stream);
	static WebApplicationConfiguration^ FromXml(String^ fileName);
	static WebApplicationConfiguration^ FromXml(TextReader^ reader);
	static WebApplicationConfiguration^ FromXml(XmlReader^ reader);

	// IsComplete
	//
	// Ensures that there is enough data to actually start an application,
	// but does not verify that the information is valid
	property bool IsComplete
	{
		bool get(void);
	}

	// LoggingFormat
	//
	// Defines the logging format to use for this application
	property zws::LoggingFormat LoggingFormat
	{
		zws::LoggingFormat get(void) { return m_logFormat; }
		void set(zws::LoggingFormat value);
	}

	// PhysicalRoot
	//
	// Defines the physical file system root for the application
	property String^ PhysicalRoot
	{
		String^ get(void) { return m_physicalRoot; }
		void set(String^ value);
	}

	// Prefixes
	//
	// Access the contained listener prefix collection instance
	property UriPrefixCollection^ Prefixes
	{
		UriPrefixCollection^ get(void) { return m_prefixes; }
	}

	// RequireSecureChannel
	//
	// Configues the application to require an SSL connection
	property bool RequireSecureChannel
	{
		bool get(void) { return m_requireSSL; }
		void set(bool value); 
	}

	// RestrictedDirectories
	//
	// Accesses the collection of restricted directory names to be added
	// to the default ASP.NET set.  Should start with a leading slash
	property Generic::IList<String^>^ RestrictedDirectories
	{
		Generic::IList<String^>^ get(void);
	}

	// ToXml
	//
	// Saves the current configuration as an XML document
	void ToXml(Stream^ stream);
	void ToXml(String^ fileName);
	void ToXml(TextWriter^ writer);
	void ToXml(XmlWriter^ writer);

	// VirtualRoot
	//
	// Defines the virtual root for the application on the server
	property String^ VirtualRoot
	{
		String^ get(void);
		void set(String^ value);
	}

	// VirtualFileSystem
	//
	// Accesses the contained VirtualFileSystemConfiguration class
	property VirtualFileSystemConfiguration^ VirtualFileSystem
	{
		VirtualFileSystemConfiguration^ get(void) { return m_vwebConfig; }
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Validate
	//
	// Validates the contents of a WebApplicationConfiguration (on the surface)
	// and throws an appropriate exception as to what the problem is
	static void Validate(WebApplicationConfiguration^ config);

	//-----------------------------------------------------------------------
	// Internal Properties

	// AllowsAnonymous
	//
	// Determines if this configuration will allow for anonymous access
	property bool AllowsAnonymous
	{
		bool get(void);
	}

	// PhysicalRootFull
	//
	// Returns the fully qualified physical root
	property String^ PhysicalRootFull
	{
		String^ get(void);
	}

private:

	//-----------------------------------------------------------------------
	// Private Member Functions

	// ReadXml
	//
	// Reads the contents of this collection from an XmlNode
	void ReadXml(XmlNode^ parent);

	//-----------------------------------------------------------------------
	// Member Variables

	[NonSerialized()] bool				m_writable;			// Writable config?
	String^								m_virtualRoot;		// Application virtual root
	String^								m_physicalRoot;		// Application physical root
	bool								m_requireSSL;		// Flag to require SSL
	UriPrefixCollection^				m_prefixes;			// Listener prefix collection
	List<String^>^						m_defaultDocuments;	// Site default documents
	List<String^>^						m_restrictedDirs;	// Restricted directories
	AuthenticationConfiguration^		m_authConfig;		// Contained auth config
	VirtualFileSystemConfiguration^		m_vwebConfig;		// Contained vweb config
	bool								m_restart;			// Auto-restart flag
	zws::LoggingFormat					m_logFormat;		// Logging format
	Dictionary<String^, String^>^		m_customHeaders;	// Custom request headers
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __WEBAPPLICATIONCONFIGURATION_H_