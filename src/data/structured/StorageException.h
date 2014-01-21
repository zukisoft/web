//---------------------------------------------------------------------------
// StorageException Declarations
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

#ifndef __STORAGEEXCEPTION_H_
#define __STORAGEEXCEPTION_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Text;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(data)
BEGIN_NAMESPACE(structured)

//---------------------------------------------------------------------------
// Class StorageException
//
// The generic exception thrown by the StructuredStorage objects.  Wraps up
// the underlying COM error into something slightly more useful than just
// an HRESULT error code
//---------------------------------------------------------------------------

STRUCTURED_STORAGE_PUBLIC ref class StorageException sealed : public Exception
{
internal:

	//-----------------------------------------------------------------------
	// Constructor
	//
	// Arguments:
	//
	//	hr			- HRESULT error code from Structured Storage API

	StorageException(HRESULT hr) : Exception(GenerateMessage(hr, nullptr))
	{
		Exception::HResult = hr;			// Set the HRESULT value
	}

	//-----------------------------------------------------------------------
	// Constructor
	//
	// Arguments:
	//
	//	hr			- HRESULT error code from Structured Storage API
	//	insert		- Single insert string for the error message

	StorageException(HRESULT hr, String^ insert) : 
		Exception(GenerateMessage(hr, insert))
	{
		Exception::HResult = hr;			// Set the HRESULT value
	}

private:
	
	//-----------------------------------------------------------------------
	// Private Member Functions

	static String^ GenerateMessage(HRESULT hr, String^ insert);
};

//---------------------------------------------------------------------------

END_NAMESPACE(structured)
END_NAMESPACE(data)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __STORAGEEXCEPTION_H_
