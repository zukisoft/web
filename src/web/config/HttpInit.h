//---------------------------------------------------------------------------
// HttpInit Declarations
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

#ifndef __HTTPINIT_H_
#define __HTTPINIT_H_
#pragma once

#pragma warning(push, 4)					// Enable maximum compiler warnings

using namespace System;
using namespace System::ComponentModel;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(config)

//---------------------------------------------------------------------------
// Class HttpInit (internal)
//
// Provides a simple wrapper around HttpInitialize / HttpTerminate
//---------------------------------------------------------------------------

ref class HttpInit sealed
{
public:

	//-----------------------------------------------------------------------
	// Constructor

	HttpInit(ULONG flags) : m_flags(flags)
	{
		// Initialize the HTTP API, the finalizer should clean this up by
		// calling HttpTerminate (this will work well enough for our purposes)

		HTTPAPI_VERSION ver = HTTPAPI_VERSION_1;
		ULONG ulResult = HttpInitialize(ver, m_flags, NULL);
		if(ulResult != ERROR_SUCCESS) throw gcnew Win32Exception(ulResult);
	}

private:

	// DESTRUCTOR / FINALIZER
	~HttpInit() { if(!m_disposed) { this->!HttpInit(); m_disposed = true; } }
	!HttpInit() { HttpTerminate(m_flags, NULL); }

	//-----------------------------------------------------------------------
	// Member Variables

	bool					m_disposed;			// Object disposal flag
	initonly ULONG			m_flags;			// Flags to HttpInitialize
};

//---------------------------------------------------------------------------

END_NAMESPACE(config)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __HTTPINIT_H_