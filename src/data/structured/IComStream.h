//---------------------------------------------------------------------------
// IComStream Declarations
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

#ifndef __ICOMSTREAM_H_
#define __ICOMSTREAM_H_
#pragma once

#include "IComPointer.h"				// Include IComPointer declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(data)
BEGIN_NAMESPACE(structured)

//---------------------------------------------------------------------------
// Interface IComStream (internal)
//
// IComStream is an identical interface to the COM IStream interface.  This
// is used to allow multiple COM interfaces to be exposed from a managed class
//---------------------------------------------------------------------------

interface class IComStream : public IComPointer
{
	//-----------------------------------------------------------------------
	// Methods

	// Clone
	//
	// Creates a new stream object with its own seek pointer
	HRESULT Clone(IStream** ppStm);

	// Commit
	//
	// Ensures that any changes made to a stream object are persisted
	HRESULT Commit(DWORD grfCommitFlags);

	// CopyTo
	//
	// Copies a specified number of bytes into another stream
	HRESULT CopyTo(IStream* pstm, ULARGE_INTEGER cb, ULARGE_INTEGER* pcbRead,
		ULARGE_INTEGER* pcbWritten);

	// LockRegion
	//
	// Restricts access to a specified range of bytes in the stream
	HRESULT LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);

	// Read
	//
	// Reads a specified number of bytes from the stream 
	HRESULT Read(void* pv, ULONG cb, ULONG* pcbRead);

	// Revert
	//
	// Discards all changes that have been made to a transacted stream 
	HRESULT Revert(void);

	// Seek
	//
	// Changes the seek pointer to a new location
	HRESULT Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER* plibNewPosition);

	// SetSize
	//
	// Changes the size of the stream object
	HRESULT SetSize(ULARGE_INTEGER libNewSize);

	// Stat
	//
	// Retrieves the STATSTG structure for this stream
	HRESULT Stat(::STATSTG* pstatstg, DWORD grfStatFlag);

	// UnlockRegion
	//
	// Removes the access restriction on a range of bytes previously restricted
	HRESULT UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);

	// Write
	//
	// Writes a specified number of bytes into the stream object
	HRESULT Write(void const* pv, ULONG cb, ULONG* pcbWritten);
};

//---------------------------------------------------------------------------

END_NAMESPACE(structured)
END_NAMESPACE(data)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __ICOMSTREAM_H_