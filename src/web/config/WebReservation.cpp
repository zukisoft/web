//---------------------------------------------------------------------------
// WebReservation Implementation
//
// Zucchini Service Configuration Library
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

#include "stdafx.h"							// Include pre-compiled headers
#include "WebReservation.h"					// Include class declarations

#pragma warning(push, 4)					// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(config)

//---------------------------------------------------------------------------
// WebReservation::CheckPrefix (private, static)
//
// Checks a UriPrefix string for API compatibility
//
// Arguments:
//
//	prefix		- UriPrefix string to be checked

String^ WebReservation::CheckPrefix(String^ prefix)
{
	int				index;					// Generic character index value
	bool			inBracket = false;		// Flag if parsing a bracket
	int				offset;					// Offset into the prefix string

	if(String::IsNullOrEmpty(prefix)) throw gcnew ArgumentNullException();

	// I stole this entire mess from the .NET Framework code (thanks to both
	// Microsoft and Lutz Roeder). It is horrible code and really should be
	// replaced with a nice regular expression. [System.Net.HttpListener.AddPrefix()]

	// Check the scheme and set the index to the position to the last character of it
	if(String::Compare(prefix, 0, "http://", 0, 7, StringComparison::OrdinalIgnoreCase) == 0) index = 7;
	else if(String::Compare(prefix, 0, "https://", 0, 8, StringComparison::OrdinalIgnoreCase) == 0) index = 8;
	else throw gcnew ArgumentException("Specified prefix does not start with 'http://' or 'https://'.");

	// Check to ensure there is a hostname, allowing for bracketed addresses
	offset = index;
	while(((offset < prefix->Length) && (prefix[offset] != '/')) && ((prefix[offset] != ':') || inBracket)) {

		if(prefix[offset] == '[') {

			if(inBracket) { offset = index; break; }
			inBracket = true;
		}

		if(inBracket && (prefix[offset] == ']')) inBracket = false;
		offset++;
	}

	// Invalid or missing hostname
    if(index == offset) 
		throw gcnew ArgumentException("Specified prefix does not include a valid hostname.");

	// Missing trailing slash character
	if (prefix[prefix->Length - 1] != '/')
        throw gcnew ArgumentException("Specified prefix does not end with a trailing '/'.");

	// Apply a default port value as necessary and return the prefix string back to the caller
	return (prefix[offset] == ':') ? String::Copy(prefix) : 
		(prefix->Substring(0, offset) + ((index == 7) ? ":80" : ":443") + prefix->Substring(offset));
}

//---------------------------------------------------------------------------

END_NAMESPACE(config)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
