//---------------------------------------------------------------------------
// UriPrefix Implementation
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
#include "UriPrefixCollection.h"		// Include UriPrefixCollection decls

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// UriPrefixCollection::Add
//
// Adds a new prefix to the collection
//
// Arguments:
//
//	uriPrefix		- New prefix to be added into the collection

void UriPrefixCollection::Add(UriPrefix^ uriPrefix)
{
	if(uriPrefix == nullptr) throw gcnew ArgumentNullException();
	if(Contains(uriPrefix)) throw gcnew PrefixExistsException(uriPrefix->ToString());

	m_col->Add(uriPrefix);			// Insert the new URI prefix
}

//---------------------------------------------------------------------------
// UriPrefixCollection::Contains
//
// Determines if the specified prefix exists in the collection.  Uses a 
// case-insensitive comparison, unlike the real collection class
//
// Arguments:
//
//	uriPrefix		- Prefix to be tested for existance

bool UriPrefixCollection::Contains(UriPrefix^ uriPrefix)
{
	if(uriPrefix == nullptr) throw gcnew ArgumentNullException();

	// Just iterate the collection and use the overloaded == operator
	// on the UriPrefixes.  I'm not overly worried about performance here,
	// I honestly doubt this will get called by many applications

	for each(UriPrefix^ prefix in m_col) if(prefix->Equals(uriPrefix)) return true;
	return false;
}

//---------------------------------------------------------------------------
// UriPrefixCollection::ReadXml (internal)
//
// Sets the state of this collection from a parent XmlNode
//
// Arguments:
//
//	parent		- Parent XmlNode passed in from WebApplicationConfiguration

void UriPrefixCollection::ReadXml(XmlNode^ parent)
{
	if(parent == nullptr) throw gcnew ArgumentNullException();

	try {

		// <prefix scheme="xxx" port="xxx">host</prefix>
		//
		for each(XmlNode^ prefix in parent->SelectNodes(XmlConstants::Elements::Prefix)) {

			this->Add(gcnew UriPrefix(
				UriPrefixScheme::Parse(prefix->Attributes[XmlConstants::Attributes::Scheme]->InnerText),
				UriPrefixHost::Parse(prefix->InnerText),
				UriPrefixPort::Parse(prefix->Attributes[XmlConstants::Attributes::Port]->InnerText)));
		}
	}

	catch(Exception^ ex) { throw gcnew XmlParseException(parent->Name, ex); }
}

//---------------------------------------------------------------------------
// UriPrefixCollection::Remove
//
// Attempts to remove the specified prefix from the collection
//
// Arguments:
//
//	uriPrefix		- Prefix to be removed from the collection

bool UriPrefixCollection::Remove(UriPrefix^ uriPrefix)
{
	if(uriPrefix == nullptr) throw gcnew ArgumentNullException();
	return m_col->Remove(uriPrefix);
}

//---------------------------------------------------------------------------
// UriPrefixCollection::ToXml (internal)
//
// Persists the state of this collection into an XML document
//
// Arguments:
//
//	writer		- Existing XmlWriter to write the configuration into

void UriPrefixCollection::ToXml(XmlWriter^ writer)
{
	// <prefixes>
	//
	for each(UriPrefix^ prefix in m_col) {

		// <prefix scheme="xxx" port="xxx">host</prefix>
		//
		writer->WriteStartElement(XmlConstants::Elements::Prefix);
		writer->WriteAttributeString(XmlConstants::Attributes::Scheme, prefix->Scheme.ToString());
		writer->WriteAttributeString(XmlConstants::Attributes::Port, prefix->Port.ToString());
		writer->WriteValue(prefix->Host.ToString());
		writer->WriteEndElement();
	}
}

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
