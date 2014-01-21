//---------------------------------------------------------------------------
// stdafx.h
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

#ifndef __STDAFX_H_
#define __STDAFX_H_
#pragma once

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
// Win32 Declarations

#define	_WIN32_WINNT		0x0500			// Windows 2000
#define	_WIN32_IE			0x0500			// IE 5.0 / ShellAPI 5.0

#include <windows.h>				// Include main Windows declarations
#include <vcclr.h>					// Include VC CLR extensions

#pragma warning(disable:4091)
#include <msclr\lock.h>				// Include lock class declarations
#pragma warning(default:4091)

//---------------------------------------------------------------------------
// Pinned Pointer Types

typedef pin_ptr<const wchar_t>		PinnedStringPtr;
typedef pin_ptr<unsigned __int8>	PinnedBytePtr;

//---------------------------------------------------------------------------
// STRUCTURED_STORAGE_PUBLIC
//
// Used in all public class declarations to allow other projects to
// embed the code without making it public

#define STRUCTURED_STORAGE_PUBLIC	public

//---------------------------------------------------------------------------

#endif	// __STDAFX_H_
