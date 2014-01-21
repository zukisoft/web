//---------------------------------------------------------------------------
// VirtualFileSystemConfiguration Implementation
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

#include "stdafx.h"							// Include project pre-compiled headers
#include "VirtualFileSystemConfiguration.h"	// Include class declarations

#pragma warning(push, 4)					// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// VirtualFileSystemConfiguration Constructor
//
// Arguments:
//
//	writable	- Flag if the configuration is writable or not

VirtualFileSystemConfiguration::VirtualFileSystemConfiguration(bool writable)
{
	// Like the parent class, we use a [NonSerialized()] boolean to prevent
	// copies of this class from being writable

	m_writable = writable;

	// Set all of the member variables to their default states.  Some
	// of this is redundant, I wanted the list to match the mvars exactly

	m_mode = VirtualFileSystemAccessMode::ReadOnly;
	m_type = VirtualFileSystemType::None;
	m_store = String::Empty;
	m_nonCacheable = gcnew List<String^>(s_dynamicTypes);
}

//---------------------------------------------------------------------------
// VirtualFileSystemConfiguration::AccessMode::set
//
// Sets the access mode for the virtual file system

void VirtualFileSystemConfiguration::AccessMode::set(VirtualFileSystemAccessMode value)
{ 
	if(!m_writable) throw gcnew NotSupportedException();		
	m_mode = value; 
}

//---------------------------------------------------------------------------
// VirtualFileSystemConfiguration::NonCacheableExtensions::get
//
// Accesses the collection of dynamic content file extensions

Generic::IList<String^>^ VirtualFileSystemConfiguration::NonCacheableExtensions::get(void)
{
	return (m_writable) ? m_nonCacheable : 
		static_cast<Generic::IList<String^>^>(m_nonCacheable->AsReadOnly()); 
}

//---------------------------------------------------------------------------
// VirtualFileSystemConfiguration::ReadXml (internal)
//
// Sets the state of this collection from a parent XmlNode
//
// Arguments:
//
//	parent		- Parent XmlNode passed in from WebApplicationConfiguration

void VirtualFileSystemConfiguration::ReadXml(XmlNode^ parent)
{
	XmlNode^				node = nullptr;			// A specific XML child node

	if(parent == nullptr) throw gcnew ArgumentNullException();

	try {

		// <accessmode>
		//
		node = parent->SelectSingleNode(XmlConstants::Elements::AccessMode);
		if(node != nullptr) this->AccessMode = VirtualFileSystemAccessMode::Parse(node->InnerText);

		// <noncacheableextensions>
		//
		node = parent->SelectSingleNode(XmlConstants::Elements::NonCacheableExtensions);
		if(node != nullptr) {

			this->NonCacheableExtensions->Clear();			// <-- First remove them all

			for each(XmlNode^ ext in node->SelectNodes(XmlConstants::Elements::Extension))
				this->NonCacheableExtensions->Add(ext->InnerText);
		}

		// <store>
		//
		node = parent->SelectSingleNode(XmlConstants::Elements::Store);
		if(node != nullptr) this->Store = node->InnerText;

		// <type>
		//
		node = parent->SelectSingleNode(XmlConstants::Elements::Type);
		if(node != nullptr) this->Type = VirtualFileSystemType::Parse(node->InnerText);
	}

	catch(Exception^ ex) {

		// If we know what node failed, add it into the exception.  This will end up
		// being an inner exception from the initial call to .FromXml(), so it might
		// need to be changed for a little more ease-of-use later on ...

		if(node != nullptr) throw gcnew XmlParseException(node->Name, ex);
		else throw gcnew XmlParseException(ex);
	}
}

//---------------------------------------------------------------------------
// VirtualFileSystemConfiguration::Store::set
//
// Sets the path to the virtual web storage file

void VirtualFileSystemConfiguration::Store::set(String^ value)
{ 
	if(!m_writable) throw gcnew NotSupportedException();		
	m_store = value; 
}

//---------------------------------------------------------------------------
// VirtualFileSystemConfiguration::ToXml (internal)
//
// Persists the state of this collection into an XML document
//
// Arguments:
//
//	writer		- Existing XmlWriter to write the configuration into

void VirtualFileSystemConfiguration::ToXml(XmlWriter^ writer)
{
	// <accessmode>
	//
	writer->WriteElementString(XmlConstants::Elements::AccessMode, m_mode.ToString());

	// <noncacheableextensions>
	//
	writer->WriteStartElement(XmlConstants::Elements::NonCacheableExtensions);
	for each(String^ ext in this->NonCacheableExtensions)
		writer->WriteElementString(XmlConstants::Elements::Extension, ext);
	writer->WriteEndElement();

	// <store>
	//
	writer->WriteElementString(XmlConstants::Elements::Store, m_store);

	// <type>
	//
	writer->WriteElementString(XmlConstants::Elements::Type, m_type.ToString());
}

//---------------------------------------------------------------------------
// VirtualFileSystemConfiguration::Type::set
//
// Sets the type of virtual file system to be utilized by the server

void VirtualFileSystemConfiguration::Type::set(VirtualFileSystemType value)
{ 
	if(!m_writable) throw gcnew NotSupportedException();		
	m_type = value; 
}

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
