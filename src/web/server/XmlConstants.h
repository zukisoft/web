//---------------------------------------------------------------------------
// XmlConstants Declarations
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

#ifndef __XMLCONSTANTS_H_
#define __XMLCONSTANTS_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class XmlConstants (static, internal)
//
// XmlConstants defines a series of constants used for working with the XML
// application configuration files
//---------------------------------------------------------------------------

ref struct XmlConstants abstract sealed
{
	ref struct Attributes abstract sealed
	{
		static initonly String^ Host = "host";
		static initonly String^ Name = "name";
		static initonly String^ Port = "port";
		static initonly String^ Scheme = "scheme";
		static initonly String^ Value = "value";
	};

	ref struct DocumentElements abstract sealed
	{
		static initonly String^ WebApplicationConfiguration = "webapplicationconfiguration";
	};

	ref struct Elements abstract sealed
	{
		static initonly String^ AccessMode = "accessmode";
		static initonly String^ Authentication = "authentication";
		static initonly String^ AutoRestart = "autorestart";
		static initonly String^ CacheNtlmCredentials = "cachentlmcredentials";
		static initonly String^ CustomRequestHeaders = "customrequestheaders";
		static initonly String^ DefaultDocuments = "defaultdocuments";
		static initonly String^ DefaultDomain = "defaultdomain";
		static initonly String^ Directory = "directory";
		static initonly String^ Document = "document";
		static initonly String^ Extension = "extension";
		static initonly String^ Header = "header";
		static initonly String^ LoggingFormat = "loggingformat";
		static initonly String^ NonCacheableExtensions = "noncacheableextensions";
		static initonly String^ PhysicalRoot = "physicalroot";
		static initonly String^ Prefix = "prefix";
		static initonly String^ Prefixes = "prefixes";
		static initonly String^ Realm = "realm";
		static initonly String^ RequireClientCertificate = "requireclientcertificate";
		static initonly String^ RequireSecureChannel = "requiresecurechannel";
		static initonly String^ RestrictedDirectories = "restricteddirectories";
		static initonly String^ Schemes = "schemes";
		static initonly String^ Scheme = "scheme";
		static initonly String^ Store = "store";
		static initonly String^ Type = "type";
		static initonly String^ VerifyClientCertificateChain = "verifyclientcertificatechain";
		static initonly String^ VerifyClientCertificateExpiration = "verifyclientcertificateexpiration";
		static initonly String^ VirtualRoot = "virtualroot";
		static initonly String^ VirtualFileSystem = "virtualfilesystem";
	};

};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __XMLCONSTANTS_H_