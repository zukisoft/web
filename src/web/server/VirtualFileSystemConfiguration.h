//---------------------------------------------------------------------------
// VirtualFileSystemConfiguration Declarations
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

#ifndef __VIRTUALFILESYSTEMCONFIGURATION_H_
#define __VIRTUALFILESYSTEMCONFIGURATION_H_
#pragma once

#include "exceptions.h"						// Include custom exception declarations
#include "VirtualFileSystemAccessMode.h"	// Include VirtualFileSystemAccess decls
#include "VirtualFileSystemType.h"			// Include VirtualFileSystemType decls
#include "XmlConstants.h"					// Include XmlConstants declarations

#pragma warning(push, 4)					// Enable maximum compiler warnings

using namespace System;
using namespace System::Xml;
using namespace zuki::web::server::exceptions;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class VirtualFileSystemConfiguration
//
// VirtualFileSystemConfiguration is a child object underneath the main config
// class WebApplicationConfiguration, and contains the properties specific to
// the virtual Structured Storage file system implementation
//---------------------------------------------------------------------------

[Serializable()]
public ref class VirtualFileSystemConfiguration sealed
{
public:

	//-----------------------------------------------------------------------
	// Properties

	// AccessMode
	//
	// Defines the access mode for the virtual file system store
	property VirtualFileSystemAccessMode AccessMode
	{
		VirtualFileSystemAccessMode get(void) { return m_mode; }
		void set(VirtualFileSystemAccessMode value);
	}

	// NonCacheableExtensions
	//
	// Accesses the collection of non-cacheable file extensions
	property Generic::IList<String^>^ NonCacheableExtensions
	{
		Generic::IList<String^>^ get(void); 
	}

	// Store
	//
	// Path to the virtual web storage file
	property String^ Store
	{
		String^ get(void) { return m_store; }
		void set(String^ value);
	}

	// Type
	//
	// Defines what type of virtual file system is to be utilized
	property VirtualFileSystemType Type
	{
		VirtualFileSystemType get(void) { return m_type; }
		void set(VirtualFileSystemType value);
	}

internal:

	// INTERNAL CONSTRUCTOR
	VirtualFileSystemConfiguration(bool writable);

	// ReadXml
	//
	// Reads the state of this instance from an XmlNode
	void ReadXml(XmlNode^ parent);

	// ToXml
	//
	// Writes the state of this instance into an XmlWriter
	void ToXml(XmlWriter^ writer);

private:

	//-----------------------------------------------------------------------
	// Member Variables

	[NonSerialized()] bool			m_writable;			// Writable configuration flag
	VirtualFileSystemAccessMode		m_mode;				// Virtual file system access mode
	VirtualFileSystemType			m_type;				// Virtual file system type code
	String^							m_store;			// Path to the storage file
	List<String^>^					m_nonCacheable;		// Non-cacheable extensions

	// DEFAULT DYNAMIC CONTENT EXTENSIONS
	static initonly Generic::IList<String^>^ s_dynamicTypes = 
		Array::AsReadOnly(gcnew array<String^>{ ".asa", ".asax", ".ascx", 
		".ashx", ".asmx", ".asp", ".aspx", ".axd", ".cdx", ".cer", ".config", 
		".cs", ".csproj", ".idc", ".licx", ".rem", ".resources", ".resx", ".shtm",
		".shtml", ".soap", ".stm", ".svc", ".vb", ".vbproj", ".vsdisco", ".webinfo" });
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __VIRTUALFILESYSTEMCONFIGURATION_H_