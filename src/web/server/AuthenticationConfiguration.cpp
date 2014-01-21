//---------------------------------------------------------------------------
// AuthenticationConfiguration Implementation
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
#include "AuthenticationConfiguration.h"	// Include class declarations

#pragma warning(push, 4)					// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// AuthenticationConfiguration Constructor
//
// Arguments:
//
//	writable	- Flag if the configuration is writable or not

AuthenticationConfiguration::AuthenticationConfiguration(bool writable)
{
	// Like the parent class, we use a [NonSerialized()] boolean to prevent
	// copies of this class from being writable

	m_writable = writable;

	// Set all of the member variables to their default states.  Some
	// of this is redundant, I wanted the list to match the mvars exactly

	m_schemes = AuthenticationSchemes::None;
	m_realm = String::Empty;
	m_requireCert = false;
	m_defaultDomain = String::Empty;
	m_verifyCertDates = false;
	m_verifyCertChain = false;
	m_cacheNtlm = false;
}

//---------------------------------------------------------------------------
// AuthenticationConfiguration::CacheNtlmCredentials::set
//
// Sets the flag to cache NTLM credentials for any given TCP connection

void AuthenticationConfiguration::CacheNtlmCredentials::set(bool value)
{ 
	if(!m_writable) throw gcnew NotSupportedException();		
	m_cacheNtlm = value; 
}

//---------------------------------------------------------------------------
// AuthenticationConfiguration::DefaultDomain::set
//
// Sets the default domain to use for Basic Authentication requests

void AuthenticationConfiguration::DefaultDomain::set(String^ value)
{ 
	if(!m_writable) throw gcnew NotSupportedException();		
	m_defaultDomain = value; 
}

//---------------------------------------------------------------------------
// AuthenticationConfiguration::ReadXml (internal)
//
// Sets the state of this collection from a parent XmlNode
//
// Arguments:
//
//	parent		- Parent XmlNode passed in from WebApplicationConfiguration

void AuthenticationConfiguration::ReadXml(XmlNode^ parent)
{
	XmlNode^				node = nullptr;			// A specific XML child node

	if(parent == nullptr) throw gcnew ArgumentNullException();

	try {

		// <cachentlmcredentials>
		//
		node = parent->SelectSingleNode(XmlConstants::Elements::CacheNtlmCredentials);
		if(node != nullptr) this->CacheNtlmCredentials = Convert::ToBoolean(node->InnerText);

		// <defaultdomain>
		//
		node = parent->SelectSingleNode(XmlConstants::Elements::DefaultDomain);
		if(node != nullptr) this->DefaultDomain = Convert::ToString(node->InnerText);

		// <realm>
		//
		node = parent->SelectSingleNode(XmlConstants::Elements::Realm);
		if(node != nullptr) this->Realm = Convert::ToString(node->InnerText);

		// <requireclientcertificate>
		//
		node = parent->SelectSingleNode(XmlConstants::Elements::RequireClientCertificate);
		if(node != nullptr) this->RequireClientCertificate = Convert::ToBoolean(node->InnerText);

		// <schemes>
		//
		// The schemes are persisted as individual AuthenticationSchemes enumeration
		// members, and need to be ORed back together again one at a time ...

		node = parent->SelectSingleNode(XmlConstants::Elements::Schemes);
		if(node != nullptr) {

			for each(XmlNode^ scheme in node->SelectNodes(XmlConstants::Elements::Scheme))
				this->Schemes |= static_cast<AuthenticationSchemes>(Enum::Parse(AuthenticationSchemes::typeid, scheme->InnerText, true));
		}

		// <verifyclientcertificatechain>
		//
		node = parent->SelectSingleNode(XmlConstants::Elements::VerifyClientCertificateChain);
		if(node != nullptr) this->VerifyClientCertificateChain = Convert::ToBoolean(node->InnerText);

		// <verifyclientcertificateexpiration>
		//
		node = parent->SelectSingleNode(XmlConstants::Elements::VerifyClientCertificateExpiration);
		if(node != nullptr) this->VerifyClientCertificateExpiration = Convert::ToBoolean(node->InnerText);
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
// AuthenticationConfiguration::Realm::set
//
// Sets the realm for Basic Authentication requests

void AuthenticationConfiguration::Realm::set(String^ value)
{ 
	if(!m_writable) throw gcnew NotSupportedException();		
	m_realm = value; 
}

//---------------------------------------------------------------------------
// AuthenticationConfiguration::RequireClientCertificate::set
//
// Sets a flag indicating that an SSL client certificate is required

void AuthenticationConfiguration::RequireClientCertificate::set(bool value)
{ 
	if(!m_writable) throw gcnew NotSupportedException();		
	m_requireCert = value; 
}

//---------------------------------------------------------------------------
// AuthenticationConfiguration::Schemes::set
//
// Defines the HTTP authentication schemes that the application will use

void AuthenticationConfiguration::Schemes::set(AuthenticationSchemes value)
{ 
	if(!m_writable) throw gcnew NotSupportedException();		
	m_schemes = value; 
}

//---------------------------------------------------------------------------
// AuthenticationInformation::ToXml (internal)
//
// Persists the state of this collection into an XML document
//
// Arguments:
//
//	writer		- Existing XmlWriter to write the configuration into

void AuthenticationConfiguration::ToXml(XmlWriter^ writer)
{
	// <cachentlmcredentials>
	//
	writer->WriteElementString(XmlConstants::Elements::CacheNtlmCredentials, m_cacheNtlm.ToString());

	// <defaultdomain>
	//
	writer->WriteElementString(XmlConstants::Elements::DefaultDomain, m_defaultDomain);

	// <realm>
	//
	writer->WriteElementString(XmlConstants::Elements::Realm, m_realm);

	// <requireclientcertificate>
	//
	writer->WriteElementString(XmlConstants::Elements::RequireClientCertificate, m_requireCert.ToString());

	// <schemes>
	//
	// <schemes> is a little complicated since I want each possible value to show
	// up as it's own entry in the XML file rather than an unintelligible number

	writer->WriteStartElement(XmlConstants::Elements::Schemes);

	for each(Object^ code in Enum::GetValues(AuthenticationSchemes::typeid)) {

		int value = Convert::ToInt32(code);			// Get the numeric value as well

		// Note: only use the powers of 2 here during the enumeration, since there are
		// combined ones like "IntegratedWindowsAuthentication" that might be confusing
		// to see in the resultant XML.  This also skips zero (AuthenticationSchemes::None)

		if((value <= 0) || ((value & (value - 1)) != 0)) continue;
		if((static_cast<int>(m_schemes) & value) == value)
			writer->WriteElementString(XmlConstants::Elements::Scheme, code->ToString());
	}

	writer->WriteEndElement();						// </schemes>

	// <verifyclientcertificatechain>
	//
	writer->WriteElementString(XmlConstants::Elements::VerifyClientCertificateChain, m_verifyCertChain.ToString());

	// <verifyclientcertificateexpiration>
	//
	writer->WriteElementString(XmlConstants::Elements::VerifyClientCertificateExpiration, m_verifyCertDates.ToString());
}

//---------------------------------------------------------------------------
// AuthenticationConfiguration::VerifyClientCertificateChain::set
//
// Indicates that any client certificates should have their chain checked
// before allowing access to the web application

void AuthenticationConfiguration::VerifyClientCertificateChain::set(bool value)
{ 
	if(!m_writable) throw gcnew NotSupportedException();		
	m_verifyCertChain = value; 
}

//---------------------------------------------------------------------------
// AuthenticationConfiguration::VerifyClientCertificateExpiration::set
//
// Indicates that any client certificates should have their dates checked
// before allowing access to the web application

void AuthenticationConfiguration::VerifyClientCertificateExpiration::set(bool value)
{ 
	if(!m_writable) throw gcnew NotSupportedException();		
	m_verifyCertDates = value; 
}

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
