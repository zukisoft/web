//---------------------------------------------------------------------------
// WebServerVersion Declarations
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

#ifndef __WEBSERVERVERSION_H_
#define __WEBSERVERVERSION_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class WebServerVersion (static, internal)
//
// WebServerVersion implements a trivial class that exposes a string to
// indicate the SERVER_SOFTWARE information.  This used to be a property of
// the WebServer class, but the security permissions on that prevented it
// from being used without exception in all cases
//---------------------------------------------------------------------------

ref class WebServerVersion abstract sealed
{
public:

	//-----------------------------------------------------------------------
	// Properties

	// SoftwareVersion
	//
	// Returns a string used whenever a server software version is needed
	static property String^ SoftwareVersion
	{
		String^ get(void) { return s_version; }
	}

private:

	// STATIC CONSTRUCTOR
	static WebServerVersion() 
	{
		Assembly^				thisAssembly;			// This assembly
		StreamReader^			reader = nullptr;		// Resource reader
		Version^				webVersion;				// Assembly version

		// Generate the server version string, which is used for certain canned
		// HTML responses as well as the SERVER_SOFTWARE server variable
		//
		// PARTIAL TRUST ISSUE: This original code would fail in a partial trust
		// environment, since it needs FileIOPermission to the assembly. I had to
		// move the version information into an embedded resource. How sad!
		//
		// s_version = WebServerVersion::typeid->Namespace + "/" +
		//	 WebServerVersion::typeid->Assembly->GetName()->Version->ToString();

		try {

			thisAssembly = WebServerVersion::typeid->Assembly;
			reader = gcnew StreamReader(thisAssembly->GetManifestResourceStream(VERSION_RESOURCE));
			webVersion = gcnew Version(reader->ReadToEnd());
		}

		catch(Exception^) { webVersion = gcnew Version(0, 0, 0, 0); }
		finally { if(reader != nullptr) reader->Close(); }

		// Construct the web server version string that will be used for identification

		s_version = WebServerVersion::typeid->Namespace + "/" + webVersion->ToString();
	}

	//-----------------------------------------------------------------------
	// Private Constants

	// VERSION_RESOURCE
	//
	// Name of the resource with the server version information in it
	
	// 02.20.2012 - Changed from "webVersion.txt" to "version.txt", file name
	// changed as a result of enhancments to MKVERSION tool
	static initonly String^	VERSION_RESOURCE = "version.txt";

	//-----------------------------------------------------------------------
	// Member Variables

	static String^			s_version;			// Server version
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __WEBSERVERVERSION_H_