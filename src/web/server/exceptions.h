//---------------------------------------------------------------------------
// exceptions.h
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

#ifndef __EXCEPTIONS_H_
#define __EXCEPTIONS_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)
BEGIN_NAMESPACE(exceptions)

//---------------------------------------------------------------------------
// zuki::web::server::exceptions
//
// Defines all of the custom exceptions for this module
//---------------------------------------------------------------------------

// ApplicationNotFoundException
//
// Thrown when an invalid application identifier is encountered
ref struct ApplicationNotFoundException sealed : public ArgumentException
{
	ApplicationNotFoundException(String^ appid) : ArgumentException(GenerateMessage(appid)) {}
	static String^ GenerateMessage(String^ appid)
	{
		return String::Format("The application [{0}] does not exist as part of this host process.", appid);
	}
};

// InvalidDefaultDocumentException
//
// Thrown when an invalid default document name has been specified
ref struct InvalidDefaultDocumentException sealed : public ArgumentException
{
	InvalidDefaultDocumentException(String^ doc) : ArgumentException(GenerateMessage(doc)) {}
	static String^ GenerateMessage(String^ doc)
	{
		return String::Format("The specified default document name [{0}] is invalid. "
			"Default document names must not contain any path information or invalid characters.", doc);
	}
};

// InvalidRestrictedDirectoryException
//
// Thrown when an invalid restricted directory name has been specified
ref struct InvalidRestrictedDirectoryException sealed : public ArgumentException
{
	InvalidRestrictedDirectoryException(String^ dir) : ArgumentException(GenerateMessage(dir)) {}
	static String^ GenerateMessage(String^ dir)
	{
		return String::Format("The specified restricted directory name [{0}] is invalid. "
			"Restricted directories must begin with a leading slash, must not end with a trailing "
			"slash and must not contain any invalid characters.", dir);
	}
};

// InvalidPhysicalRootException
//
// Thrown when an invalid physical root name has been specified
ref struct InvalidPhysicalRootException sealed : public ArgumentException
{
	InvalidPhysicalRootException(String^ dir) : ArgumentException(GenerateMessage(dir)) {}
	static String^ GenerateMessage(String^ dir)
	{
		return String::Format("The specified physical root [{0}] is invalid. The physical "
			"root must indicate a full path to a directory that exists on the local file system "
			"and the calling process must have a minimum of read access granted to it.", dir);
	}
};

// InvalidStorageFileException
//
// Thrown when an invalid structured storage file has been specified for the VFS
ref struct InvalidStorageFileException sealed : public ArgumentException
{
	InvalidStorageFileException(String^ fileName) : ArgumentException(GenerateMessage(fileName)) {}
	static String^ GenerateMessage(String^ fileName)
	{
		return String::Format("The specified virtual file system storage file [{0}] is invalid. "
			"The virtual file system storage file must exist on the local file system, must be "
			"a valid Structured Storage file and must not be opened exclusively by other processes", fileName);
	}
};

// InvalidUriPortException
//
// Thrown when an invalid port number has been specified for a UriPrefix
ref struct InvalidUriPortException sealed : public ArgumentException
{
	InvalidUriPortException(int port) : ArgumentException(GenerateMessage(port)) {}
	static String^ GenerateMessage(int port)
	{
		return String::Format("The specified port number [{0}] is invalid.  UriPort values "
			"must fall within the range of 0 to 65535.  This server does not support port "
			"values outside of this range.", port);
	}
};

// InvalidUriPrefixException
//
// Thrown when a persisted UriPrefix cannot be parsed back again
ref struct InvalidUriPrefixException sealed : public Exception
{
	InvalidUriPrefixException(String^ prefix) : Exception(GenerateMessage(prefix)) {}
	static String^ GenerateMessage(String^ prefix)
	{
		return String::Format("The URI prefix [{0}] cannot be parsed.  A URI prefix must formatted as "
			"[{scheme}://{host}:{port}/] and must not contain an invalid scheme name, an unknown host wildcard "
			"character or an invalid port number.", prefix);
	}
};

// InvalidVirtualRootException
//
// Thrown when an invalid virtual root name has been specified
ref struct InvalidVirtualRootException sealed : public ArgumentException
{
	InvalidVirtualRootException(String^ dir) : ArgumentException(GenerateMessage(dir)) {}
	static String^ GenerateMessage(String^ dir)
	{
		return String::Format("The specified virtual root [{0}] is invalid. Virtual roots "
			"must begin with a leading slash, must not end with a trailing slash and must "
			"not contain any invalid characters.", dir);
	}
};

// MissingPrefixesException
//
// Thrown when no prefixes have been specified for the application
ref struct MissingPrefixesException sealed : public ArgumentException
{
	MissingPrefixesException() : ArgumentException(GenerateMessage()) {}
	static String^ GenerateMessage(void)
	{
		return String::Format("At least one UriPrefix must be specified to create a "
			"web application instance.");
	}
};

// MissingSecurePrefixException
//
// Thrown when no HTTPS prefixes have been specified for the application, but the
// application has been configured to require a secure communication channel
ref struct MissingSecurePrefixException sealed : public ArgumentException
{
	MissingSecurePrefixException() : ArgumentException(GenerateMessage()) {}
	static String^ GenerateMessage(void)
	{
		return String::Format("Applications configured to require a secure communication "
			"channel (SSL) must also specify at least one prefix with an HTTPS: scheme.");
	}
};

// PrefixExistsException
//
// Thrown when a duplicate prefix is added to the prefix collection
ref struct PrefixExistsException sealed : public ArgumentException
{
	PrefixExistsException(String^ prefix) : ArgumentException(GenerateMessage(prefix)) {}
	static String^ GenerateMessage(String^ prefix)
	{
		return String::Format("The URI prefix [{0}] already exists in the prefix collection.", prefix);
	}
};

// StorageProviderNotInstalledException
//
// Thrown when the structured storage virtual path provider could not
// be installed into the hosting environment
ref struct StorageProviderNotInstalledException : public Exception
{
	StorageProviderNotInstalledException() : Exception(GenerateMessage()) {}
	static String^ GenerateMessage(void)
	{
		return "The Structured Storage virtual path provider could not be installed. The most likely "
			"cause of the failure is an attempt to use a pre-compiled ASP.NET application in "
			"conjunction with a virtual path provider, which is prohibited by the .NET runtime.";
	}
};

// XmlParseException
//
// Thrown when something bad happens parsing an XML document
ref struct XmlParseException : public Exception
{
	XmlParseException(Exception^ ex) : Exception(GenerateMessage(), ex) {}
	XmlParseException(String^ element, Exception^ ex) : Exception(GenerateMessage(element), ex) {}
	static String^ GenerateMessage(void)
	{
		return "The referenced XML document could not be parsed. The inner exception associated with " +
			" this exception may expose additional information.";
	}
	static String^ GenerateMessage(String^ element)
	{
		return "The referenced XML document could not be parsed. The content of the <" + 
			element + "> node is invalid.  The inner exception associated with this exception " +
			"may expose additional information.";
	}
};

//---------------------------------------------------------------------------

END_NAMESPACE(exceptions)
END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __EXCEPTIONS_H_