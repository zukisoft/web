//---------------------------------------------------------------------------
// WebApplicationConfiguration Implementation
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
#include "WebApplicationConfiguration.h"	// Include class declarations

#pragma warning(push, 4)					// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// WebApplicationConfiguration Constructor
//
// Arguments:
//
//	NONE

WebApplicationConfiguration::WebApplicationConfiguration()
{
	// Only configurations that were created by this constructor can be modified.
	// Since m_writable is [NonSerialized()], this ends up being FALSE every time
	// the class is copied across application domains, which is what we want

	m_writable = true;

	// Set all of the member variables to their default states.  Some
	// of this is redundant, I wanted the list to match the mvars exactly

	m_virtualRoot = String::Empty;
	m_physicalRoot = String::Empty;
	m_requireSSL = false;
	m_prefixes = gcnew UriPrefixCollection();
	m_defaultDocuments = gcnew List<String^>();
	m_restrictedDirs = gcnew List<String^>();
	m_authConfig = gcnew AuthenticationConfiguration(true);
	m_vwebConfig = gcnew VirtualFileSystemConfiguration(true);
	m_restart = true;
	m_logFormat = zws::LoggingFormat::None;
	m_customHeaders = gcnew Dictionary<String^, String^>();
}

//---------------------------------------------------------------------------
// WebApplicationConfiguration::AllowsAnonymous::get
//
// Determines if this application allows anonymous access or not

bool WebApplicationConfiguration::AllowsAnonymous::get(void)
{
	return (m_authConfig->Schemes & AuthenticationSchemes::Anonymous) == AuthenticationSchemes::Anonymous;
}

//---------------------------------------------------------------------------
// WebApplicationConfiguration::AutoRestart::set
//
// Sets the flag indicating if the application should automatically restart

void WebApplicationConfiguration::AutoRestart::set(bool value)
{
	if(!m_writable) throw gcnew NotSupportedException();		
	m_restart = value;
}

//---------------------------------------------------------------------------
// WebApplicationConfiguration::CustomRequestHeaders::get
//
// Accesses the dictionary of custom request headers

Generic::IDictionary<String^, String^>^ WebApplicationConfiguration::CustomRequestHeaders::get(void)
{
	// Use the silly little ReadOnlyDictionary class to protect the contents
	// of the collection if we're in read-only mode here ...

	return (m_writable) ? m_customHeaders : static_cast<Generic::IDictionary<String^, String^>^>
		(gcnew ReadOnlyDictionary<String^, String^>(m_customHeaders));
}

//---------------------------------------------------------------------------
// WebApplicationConfiguration::DefaultDocuments::get
//
// Accesses the collection of default documents for this application

Generic::IList<String^>^ WebApplicationConfiguration::DefaultDocuments::get(void)
{
	return (m_writable) ? m_defaultDocuments : 
		static_cast<Generic::IList<String^>^>(m_defaultDocuments->AsReadOnly()); 
}

//---------------------------------------------------------------------------
// WebApplicationConfiguration::FromXml (static)
//
// Generates a WebApplicationConfiguration instance from an XML file
//
// Arguments:
//
//	stream		- Open Stream instance containing the XML data

WebApplicationConfiguration^ WebApplicationConfiguration::FromXml(Stream^ stream)
{
	return FromXml(gcnew XmlTextReader(stream));	// Delegate to XmlReader version
}

//---------------------------------------------------------------------------
// WebApplicationConfiguration::FromXml (static)
//
// Generates a WebApplicationConfiguration instance from an XML file
//
// Arguments:
//
//	fileName		- Name of the source file to read the XML from

WebApplicationConfiguration^ WebApplicationConfiguration::FromXml(String^ fileName)
{
	XmlTextReader^	reader = gcnew XmlTextReader(fileName);

	try { return FromXml(reader); }			// Delegate to XmlReader version
	finally { delete reader; }				// Dispose of the XmlTextReader
}

//---------------------------------------------------------------------------
// WebApplicationConfiguration::FromXml (static)
//
// Generates a WebApplicationConfiguration instance from an XML file
//
// Arguments:
//
//	reader		- Open TextReader instance containing the XML data

WebApplicationConfiguration^ WebApplicationConfiguration::FromXml(TextReader^ reader)
{
	return FromXml(gcnew XmlTextReader(reader));	// Delegate to XmlReader version
}

//---------------------------------------------------------------------------
// WebApplicationConfiguration::FromXml (static)
//
// Generates a WebApplicationConfiguration instance from an XML file
//
// Arguments:
//
//	reader		- Open XmlReader instance containing the XML data

WebApplicationConfiguration^ WebApplicationConfiguration::FromXml(XmlReader^ reader)
{
	WebApplicationConfiguration^	config;				// The new configuration object
	XmlDocument^					doc;				// XmlDocument instance

	// I'm not really a big XML person (in case that's not painfully obvious by now)
	// so I'm going to use the XmlDocument class instead of the XmlReader since it
	// allows the XML to be accessed more randomly. (Note: The author *HATES* XML)

	config = gcnew WebApplicationConfiguration();	// Construct the config instance
	doc = gcnew XmlDocument();						// Construct the document instance
	doc->Load(reader);								// Load from the provided XmlReader

	// If there is no document element, just throw some lame exception.  This isn't
	// even worth making a custom one (did I mention I *HATE* XML yet? Twenty times?)

	if(doc->DocumentElement == nullptr) throw gcnew XmlException("Missing document element");

	config->ReadXml(doc->DocumentElement);			// Generate from the document element
	return config;									// Return generated configuration
}

//---------------------------------------------------------------------------
// WebApplicationConfiguration::IsComplete::get
//
// Determines if there is enough information to actually launch an app

bool WebApplicationConfiguration::IsComplete::get(void)
{
	// This doesn't check to see if the configuration is VALID, it just checks
	// to see if enough stuff has been set to try and launch an application. The
	// intention for using this method is for utility applications that need to
	// enable/disable UI elements based on the current configuration ...

	// AUTHENTICATION SCHEME
	if(Authentication->Schemes == AuthenticationSchemes::None) return false;

	// PHYSICAL ROOT
	if(String::IsNullOrEmpty(PhysicalRoot)) return false;

	// PREFIX
	if(Prefixes->Count == 0) return false;

	// VIRTUAL ROOT
	if(String::IsNullOrEmpty(VirtualRoot)) return false;

	return true;
}

//---------------------------------------------------------------------------
// WebApplicationConfiguration::LoggingFormat::set
//
// Sets the activity logging format for this application

void WebApplicationConfiguration::LoggingFormat::set(zws::LoggingFormat value)
{ 
	if(!m_writable) throw gcnew NotSupportedException();		
	m_logFormat = value; 
}

//---------------------------------------------------------------------------
// WebApplicationConfiguration::PhysicalRoot::set
//
// Sets the physical root path for this web application

void WebApplicationConfiguration::PhysicalRoot::set(String^ value)
{ 
	if(!m_writable) throw gcnew NotSupportedException();		
	m_physicalRoot = value; 
}

//---------------------------------------------------------------------------
// WebApplicationConfiguration::PhysicalRootFull::get
//
// Gets the fully qualified physical root, or the current path if not valid

String^ WebApplicationConfiguration::PhysicalRootFull::get(void)
{ 
	try { return Path::GetFullPath(Environment::ExpandEnvironmentVariables(m_physicalRoot)); }
	catch(Exception^) { return Environment::CurrentDirectory; }
}

//---------------------------------------------------------------------------
// WebApplicationConfiguration::ReadXml (private)
//
// Reads the contents of this collection from an XML parent node
//
// Arguments:
//
//	parent		- Parent XmlNode to read the configuration from

void WebApplicationConfiguration::ReadXml(XmlNode^ parent)
{
	XmlNode^				node = nullptr;		// Current XmlNode being processed

	if(parent == nullptr) throw gcnew ArgumentNullException();

	try {

		// <authentication>
		//
		node = parent->SelectSingleNode(XmlConstants::Elements::Authentication);
		if(node != nullptr) this->Authentication->ReadXml(node);

		// <autorestart>
		//
		node = parent->SelectSingleNode(XmlConstants::Elements::AutoRestart);
		if(node != nullptr) this->AutoRestart = Convert::ToBoolean(node->InnerText);
		
		// <customrequestheaders>
		//
		node = parent->SelectSingleNode(XmlConstants::Elements::CustomRequestHeaders);
		if(node != nullptr) {

			// <header name="xxx" value="xxx"/>
			//
			for each(XmlNode^ header in node->SelectNodes(XmlConstants::Elements::Header)) {

				XmlAttribute^ name = header->Attributes[XmlConstants::Attributes::Name];
				XmlAttribute^ value = header->Attributes[XmlConstants::Attributes::Value];
				if((name != nullptr) && (value != nullptr)) this->CustomRequestHeaders->Add(name->InnerText, value->InnerText);
			}
		}

		// <defaultdocuments>
		//
		node = parent->SelectSingleNode(XmlConstants::Elements::DefaultDocuments);
		if(node != nullptr) {

			for each(XmlNode^ doc in node->SelectNodes(XmlConstants::Elements::Document))
				this->DefaultDocuments->Add(doc->InnerText);
		}

		// <loggingformat>
		//
		node = parent->SelectSingleNode(XmlConstants::Elements::LoggingFormat);
		if(node != nullptr) this->LoggingFormat = zws::LoggingFormat::Parse(node->InnerText);

		// <physicalroot>
		//
		node = parent->SelectSingleNode(XmlConstants::Elements::PhysicalRoot);
		if(node != nullptr) this->PhysicalRoot = Convert::ToString(node->InnerText);

		// <prefixes>
		//
		node = parent->SelectSingleNode(XmlConstants::Elements::Prefixes);
		if(node != nullptr) this->Prefixes->ReadXml(node);

		// <requiresecurechannel>
		//
		node = parent->SelectSingleNode(XmlConstants::Elements::RequireSecureChannel);
		if(node != nullptr) this->RequireSecureChannel = Convert::ToBoolean(node->InnerText);

		// <restricteddirectories>
		//
		node = parent->SelectSingleNode(XmlConstants::Elements::RestrictedDirectories);
		if(node != nullptr) {

			for each(XmlNode^ dir in node->SelectNodes(XmlConstants::Elements::Directory))
				this->RestrictedDirectories->Add(dir->InnerText);
		}

		// <virtualroot>
		//
		node = parent->SelectSingleNode(XmlConstants::Elements::VirtualRoot);
		if(node != nullptr) this->VirtualRoot = Convert::ToString(node->InnerText);

		// <virtualfilesystem>
		//
		node = parent->SelectSingleNode(XmlConstants::Elements::VirtualFileSystem);
		if(node != nullptr) this->VirtualFileSystem->ReadXml(node);
	}

	catch(Exception^ ex) {

		// If we had a referenced node that failed, throw it into the error message.  This
		// is hokey, but it does help a little bit and gets me out of XML land more quickly

		if(node != nullptr) throw gcnew XmlParseException(node->Name, ex);
		else throw gcnew XmlParseException(ex);
	}
}

//---------------------------------------------------------------------------
// WebApplicationConfiguration::RequireSecureChannel::set
//
// Sets the flag indicating if the application will require an SSL channel

void WebApplicationConfiguration::RequireSecureChannel::set(bool value)
{ 
	if(!m_writable) throw gcnew NotSupportedException();		
	m_requireSSL = value; 
}

//---------------------------------------------------------------------------
// WebApplicationConfiguration::RestrictedDirectories::get
//
// Accesses the collection of additional 'restricted' directories for the app

Generic::IList<String^>^ WebApplicationConfiguration::RestrictedDirectories::get(void)
{ 
	return (m_writable) ? m_restrictedDirs : 
		static_cast<Generic::IList<String^>^>(m_restrictedDirs->AsReadOnly()); 
}

//---------------------------------------------------------------------------
// WebApplicationConfiguration::ToXml
//
// Converts the current configuration into an XML document
//
// Arguments:
//
//	stream		- Open stream to write the XML data into

void WebApplicationConfiguration::ToXml(Stream^ stream)
{
	XmlTextWriter^ writer = gcnew XmlTextWriter(stream, nullptr);
	writer->Formatting = Formatting::Indented;

	return ToXml(writer);				// Delegate to XmlWriter version
}

//---------------------------------------------------------------------------
// WebApplicationConfiguration::ToXml
//
// Converts the current configuration into an XML document
//
// Arguments:
//
//	fileName	- Name of the file to write the XML configuration into

void WebApplicationConfiguration::ToXml(String^ fileName)
{
	XmlTextWriter^ writer = gcnew XmlTextWriter(fileName, nullptr);

	try {

		writer->Formatting = Formatting::Indented;
		
		writer->WriteStartDocument(true);	// Start the document header
		ToXml(writer);						// Delegate to XmlWriter version
		writer->WriteEndDocument();			// Close out the document
	}

	finally { delete writer; }			// Dispose of writer when finished
}

//---------------------------------------------------------------------------
// WebApplicationConfiguration::ToXml
//
// Converts the current configuration into an XML document
//
// Arguments:
//
//	writer		- Open TextWriter to write the XML data into

void WebApplicationConfiguration::ToXml(TextWriter^ writer)
{
	XmlTextWriter^ xwriter = gcnew XmlTextWriter(writer);
	xwriter->Formatting = Formatting::Indented;

	return ToXml(xwriter);				// Delegate to XmlWriter version
}

//---------------------------------------------------------------------------
// WebApplicationConfiguration::ToXml
//
// Converts the current configuration into an XML document
//
// Arguments:
//
//	writer		- Open XmlWriter to write the XML data into

void WebApplicationConfiguration::ToXml(XmlWriter^ writer)
{
	// <webapplicationconfiguration>
	//
	writer->WriteStartElement(XmlConstants::DocumentElements::WebApplicationConfiguration);

	// <authentication>
	//
	writer->WriteStartElement(XmlConstants::Elements::Authentication);
	m_authConfig->ToXml(writer);
	writer->WriteEndElement();

	// <autorestart>
	//
	writer->WriteElementString(XmlConstants::Elements::AutoRestart, m_restart.ToString());

	// <customrequestheaders>
	//
	writer->WriteStartElement(XmlConstants::Elements::CustomRequestHeaders);
	for each(KeyValuePair<String^, String^> header in this->CustomRequestHeaders) {

		// <header name="xxx" value="xxx"/>
		writer->WriteStartElement(XmlConstants::Elements::Header);
		writer->WriteAttributeString(XmlConstants::Attributes::Name, header.Key);
		writer->WriteAttributeString(XmlConstants::Attributes::Value, header.Value);
		writer->WriteEndElement();
	}
	writer->WriteEndElement();

	// <defaultdocuments>
	//
	writer->WriteStartElement(XmlConstants::Elements::DefaultDocuments);
	for each(String^ doc in this->DefaultDocuments)
		writer->WriteElementString(XmlConstants::Elements::Document, doc);
	writer->WriteEndElement();

	// <loggingformat>
	//
	writer->WriteElementString(XmlConstants::Elements::LoggingFormat, m_logFormat.ToString());

	// <physicalroot>
	//
	writer->WriteElementString(XmlConstants::Elements::PhysicalRoot, m_physicalRoot);

	// <prefixes>
	//
	writer->WriteStartElement(XmlConstants::Elements::Prefixes);
	m_prefixes->ToXml(writer);
	writer->WriteEndElement();

	// <requiresecurechannel>
	writer->WriteElementString(XmlConstants::Elements::RequireSecureChannel, m_requireSSL.ToString());

	// <restricteddirectories>
	//
	writer->WriteStartElement(XmlConstants::Elements::RestrictedDirectories);
	for each(String^ dir in this->RestrictedDirectories)
		writer->WriteElementString(XmlConstants::Elements::Directory, dir);
	writer->WriteEndElement();

	// <virtualroot>
	//
	writer->WriteElementString(XmlConstants::Elements::VirtualRoot, m_virtualRoot);

	// <virtualfilesystem>
	//
	writer->WriteStartElement(XmlConstants::Elements::VirtualFileSystem);
	m_vwebConfig->ToXml(writer);
	writer->WriteEndElement();

	writer->WriteEndElement();			// </webapplicationconfiguration>
}

//---------------------------------------------------------------------------
// WebApplicationConfiguration::Validate (internal, static)
//
// Validates the contents of a WebApplicationConfiguration instance and
// throws an exception appropriate to the problem if one is located
//
// Arguments:
//
//	config		- Configuration instance to be validated

void WebApplicationConfiguration::Validate(WebApplicationConfiguration^ config)
{
	StorageConnectionStringBuilder^	csb;	// Storage connection string builder
	String^							value;	// Yanked out reference for brevity

	// DEFAULT DOCUMENTS
	//
	// Default documents should not include any path information or
	// any invalid characters
	
	for each(String^ doc in config->DefaultDocuments) {

		// Path::GetInvalidFileNameChars includes directory separators,
		// so this test should be quite sufficient for validation

		if(doc->IndexOfAny(Path::GetInvalidFileNameChars()) != -1)
			throw gcnew InvalidDefaultDocumentException(doc);
	}

	// PHYSICAL ROOT
	//
	// The physical root must exist and be accessible from this process.
	// It also must be rooted, don't allow relative paths due to the various
	// application domains at play and the chance of different working folders

	value = config->PhysicalRootFull;

	if((!Directory::Exists(value)) || (!Path::IsPathRooted(value)))
		throw gcnew InvalidPhysicalRootException(value);

	try { (gcnew FileIOPermission(FileIOPermissionAccess::Read | FileIOPermissionAccess::PathDiscovery,
		config->PhysicalRootFull))->Demand(); }
	catch(Exception^) { throw gcnew InvalidPhysicalRootException(value); }

	// PREFIXES
	//
	// The prefixes are fairly self-validating but we should make sure
	// that there is at least ONE prefix for listening here

	if(config->Prefixes->Count == 0) throw gcnew MissingPrefixesException();

	// RESTRICTED DIRECTORIES
	//
	// Restricted directories must start with a leading slash and not
	// end with a trailing slash

	for each(String^ dir in config->RestrictedDirectories) {

		// NOTE: Path::IsPathRooted also checks for invalid characters

		if((String::IsNullOrEmpty(dir)) || (!Path::IsPathRooted(dir)))
			throw gcnew InvalidRestrictedDirectoryException(dir);

		if(!dir->StartsWith("/")) throw gcnew InvalidRestrictedDirectoryException(dir);
		if((dir->Length > 1) && (dir->EndsWith("/") || dir->EndsWith("\\")))
			throw gcnew InvalidRestrictedDirectoryException(dir);
	}

	// VIRTUAL ROOT
	//
	// The virtual root should start with a slash but not end with one.
	
	value = config->VirtualRoot;

	if(String::IsNullOrEmpty(value) || (!Path::IsPathRooted(value)))
		throw gcnew InvalidVirtualRootException(value);

	if(!value->StartsWith("/")) throw gcnew InvalidVirtualRootException(value);
	if((value->Length > 1) && (value->EndsWith("/") || value->EndsWith("\\")))
		throw gcnew InvalidVirtualRootException(value);

	// VIRTUAL FILE SYSTEM
	//
	// The virtual file system file itself must exist and should be a Structured
	// Storage file.  This is a little slow to check here, but it keeps everything
	// in the same place and will throw a configuration style exception instead
	// of a lower level Structured Storage exception if it fails to open.

	if(config->VirtualFileSystem->Type != VirtualFileSystemType::None) {

		csb = gcnew StorageConnectionStringBuilder(config->VirtualFileSystem->Store);
		if(!File::Exists(csb->DataSource)) throw gcnew InvalidStorageFileException(csb->DataSource);

		// STRUCTURED STORAGE
		if(config->VirtualFileSystem->Type == VirtualFileSystemType::StructuredStorage) {
		
			StructuredStorage^ storage = nullptr;
			try { storage = StructuredStorage::Open(csb->DataSource, StorageOpenMode::Open, StorageAccessMode::ReadOnlyShared); }
			catch(Exception^) { throw gcnew InvalidStorageFileException(csb->DataSource); }
			finally { if(storage != nullptr) delete storage; }
		}
	}

	// SECURE CHANNEL
	//
	// The only real thing to check for here is to ensure that an SSL prefix has
	// been added to the collection somewhere before allowing the application to
	// require a secure channel for communication

	if(config->RequireSecureChannel) {

		bool hasHTTPS = false;
		
		for each(UriPrefix^ prefix in config->Prefixes)
			if(prefix->Scheme == UriPrefixScheme::Https) hasHTTPS = true;
		
		if(!hasHTTPS) throw gcnew MissingSecurePrefixException();
	}
}

//---------------------------------------------------------------------------
// WebApplicationConfiguration::VirtualRoot::get
//
// Gets the specified virtual root for the application

String^ WebApplicationConfiguration::VirtualRoot::get(void)
{
	return String::IsNullOrEmpty(m_virtualRoot) ? "/" : m_virtualRoot;
}

//---------------------------------------------------------------------------
// WebApplicationConfiguration::VirtualRoot::set
//
// Sets the specified virtual root for the application

void WebApplicationConfiguration::VirtualRoot::set(String^ value)
{
	if(!m_writable) throw gcnew NotSupportedException();

	if(value == nullptr) throw gcnew ArgumentNullException();
	m_virtualRoot = (value != "/") ? value->TrimEnd(gcnew array<__wchar_t>{'/'}) : value;
}

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
