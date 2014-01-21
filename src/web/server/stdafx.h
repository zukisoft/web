//---------------------------------------------------------------------------
// stdafx.h
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

#ifndef __STDAFX_H_
#define __STDAFX_H_
#pragma once

#if !defined(UNICODE) || !defined(_UNICODE)
#error This module must be built using the Unicode API set
#endif

//---------------------------------------------------------------------------
// BEGIN_NAMESPACE / END_NAMESPACE
//
// Used to define namespaces, since C++/CLI still doesn't let you combine them
// into one statement

#define BEGIN_NAMESPACE(__x) namespace __x {
#define END_NAMESPACE(__x) }

//---------------------------------------------------------------------------
// CHECK_DISPOSED
//
// Used throughout to make object disposed exceptions easier to read and not
// require hard-coding a class name into the statement.  This will throw the
// function name, but that's actually better in my opinion

#define CHECK_DISPOSED(__flag) \
	if(__flag) throw gcnew ObjectDisposedException(gcnew String(__FUNCTION__));

//---------------------------------------------------------------------------
// Win32 / CRT Declarations

#define WINVER			0x0501			// Windows XP (Service Pack 2)
#define _WIN32_WINNT	0x0501			// Windows XP (Service Pack 2)

// NOTE: Including <windows.h> before <http.h> will cause a series
// of redefinition errors in <winsock2.h> when compiling the project.

#include <http.h>					// Include HTTP.SYS declaration
#include <windows.h>				// Include main Win32 declarations
#include <malloc.h>					// Include CRT memory allocation declarations
#include <wincrypt.h>				// Include CryptoAPI declarations
#include <iphlpapi.h>				// Include IP Helper declarations
#include <vcclr.h>					// Include VC CLR declarations

#pragma warning(disable:4091)
#include <msclr\lock.h>				// Include lock class declarations
#pragma warning(default:4091)

// Remove some macros that conflict with CLR methods/properties

#undef GetEnvironmentVariable

//---------------------------------------------------------------------------
// Pinned Pointer Types

typedef pin_ptr<const wchar_t>		PinnedStringPtr;
typedef pin_ptr<unsigned __int8>	PinnedBytePtr;

//---------------------------------------------------------------------------
// Embedded Structured Storage

// STRUCTURED_STORAGE_PUBLIC
//
// Defined to be nothing so that the Structured Storage library becomes
// internal to this assembly and cannot be accessed
#define STRUCTURED_STORAGE_PUBLIC

#include "..\..\data\structured\StructuredStorage.h"

//---------------------------------------------------------------------------

#endif	// __STDAFX_H_

