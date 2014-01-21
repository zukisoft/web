//---------------------------------------------------------------------------
// StorageException Implementation
//
// ZukiSoft Structured Storage
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

#include "stdafx.h"					// Include project pre-compiled headers
#include "StorageException.h"		// Include StorageException declarations

#pragma warning(push, 4)			// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(data)
BEGIN_NAMESPACE(structured)

//---------------------------------------------------------------------------
// StorageException::GenerateMessage (private)
//
// Used to create the exception message based on nothing but the HRESULT.
// Ordinarily, we could use the Win32Exception class, but many of the STG
// error messages expect a string argument for formatting (%1).  Granted,
// this can ONLY handle the one insert string, but it's better than nothing
//
// Arguments :
//
//	hr			- HRESULT code to be looked up
//	insert		- Insert string to replace %1 in the error message

String^ StorageException::GenerateMessage(HRESULT hr, String^ insert)
{
	IErrorInfo*				pErrorInfo;			// In case COM is nice to us
	BSTR					bstr;				// Generic BSTR value
	LPWSTR					pwszMessage;		// Looked up message
	StringBuilder^			sb;					// String Builder class

	sb = gcnew StringBuilder();

	// There is always a chance that COM was pleasant enough to throw some
	// error information onto the current thread for us.  In the case of
	// Structured Storage, this never seems to actually happen, however.

	if(GetErrorInfo(NULL, &pErrorInfo) == S_OK) {

		// Format the string as "[source]: description"

		pErrorInfo->GetSource(&bstr);
		sb->AppendFormat("[{0}]: ", Marshal::PtrToStringBSTR(IntPtr(bstr)));
		SysFreeString(bstr);

		pErrorInfo->GetDescription(&bstr);
		sb->Append(Marshal::PtrToStringBSTR(IntPtr(bstr)));
		SysFreeString(bstr);

		pErrorInfo->Release();				// Finished with the error info
		return sb->ToString();				// Return the COM error info
	}

	// As expected, there isn't any COM error information to grab, so the
	// next best thing we can do is ask the system to look it up for us

	if(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS |
		FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPWSTR>(&pwszMessage), 0, NULL)) 
	{ 

		// Cheesy way of replacing the %1 in the message here, but significantly
		// easier (and probably faster) than going through the hassle of converting
		// the string, passing it into FormatMessage, blah blah blah

		String^ msg = gcnew String(pwszMessage);
		if(insert) msg = msg->Replace("%1", insert);

		sb->Append(msg);				// Tack on the error message
		LocalFree(pwszMessage);			// Release the system allocated buffer
	}

	// And in the event that even Windows itself cannot find the error message
	// for us, just peuk with a default "I have no clue what's wrong" message

	else sb->Append("Unknown or unspecified error");

	sb->AppendFormat(" (0x{0:X})", hr);			// Append the HRESULT
	return sb->ToString();						// Return the message
}

//---------------------------------------------------------------------------

END_NAMESPACE(structured)
END_NAMESPACE(data)
END_NAMESPACE(zuki)

#pragma warning(pop)
