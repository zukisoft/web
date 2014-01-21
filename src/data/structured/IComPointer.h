//---------------------------------------------------------------------------
// IComPointer Declarations
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

#ifndef __ICOMPOINTER_H_
#define __ICOMPOINTER_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(data)
BEGIN_NAMESPACE(structured)

//---------------------------------------------------------------------------
// Interface IComPointer (internal)
//
// IComPointer serves as a base interface from which all the other IComXXXX
// interfaces must derive
//---------------------------------------------------------------------------

interface class IComPointer
{
	// IsDisposed
	//
	// Exposes the object's internal disposal status
	bool IsDisposed(void);
};

//---------------------------------------------------------------------------

END_NAMESPACE(structured)
END_NAMESPACE(data)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __ICOMPOINTER_H_