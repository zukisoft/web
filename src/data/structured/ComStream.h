//---------------------------------------------------------------------------
// ComStream Declarations
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

#ifndef __COMSTREAM_H_
#define __COMSTREAM_H_
#pragma once

#include "IComStream.h"					// Include IComStream declarations
#include "StorageException.h"			// Include StorageException decls

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections::Generic;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(data)
BEGIN_NAMESPACE(structured)

//---------------------------------------------------------------------------
// Class ComStream (internal)
//
// ComStream implements a safe pointer class that allows a COM pointer
// to be safely shared among managed object instances, and implements thread
// safety for all member functions
//---------------------------------------------------------------------------

ref class ComStream sealed : public IComStream
{
public:

	//-----------------------------------------------------------------------
	// Constructors
	
	ComStream(IStream* pStream);

	//-----------------------------------------------------------------------
	// Member Functions

	// Commit (IComStream)
	//
	// Ensures that any changes made to a stream object are persisted
	virtual HRESULT Commit(DWORD grfCommitFlags);

	// CopyTo (IComStream)
	//
	// Copies a specified number of bytes into another stream
	virtual HRESULT CopyTo(IStream* pstm, ULARGE_INTEGER cb, ULARGE_INTEGER* pcbRead,
		ULARGE_INTEGER* pcbWritten);

	// CreateClone
	//
	// Creates a cloned instance of this ComStream
	virtual HRESULT CreateClone(ComStream^% clone);

	// IsDisposed (IComPointer)
	//
	// Exposes the object's internal disposed status
	virtual bool IsDisposed(void) { return m_disposed; }

	// LockRegion (IComStream)
	//
	// Restricts access to a specified range of bytes in the stream
	virtual HRESULT LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);

	// Read (IComStream)
	//
	// Reads a specified number of bytes from the stream 
	virtual HRESULT Read(void* pv, ULONG cb, ULONG* pcbRead);

	// Revert (IComStream)
	//
	// Discards all changes that have been made to a transacted stream 
	virtual HRESULT Revert(void);

	// Seek (IComStream)
	//
	// Changes the seek pointer to a new location
	virtual HRESULT Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER* plibNewPosition);

	// SetSize (IComStream)
	//
	// Changes the size of the stream object
	virtual HRESULT SetSize(ULARGE_INTEGER libNewSize);

	// Stat (IComStream)
	//
	// Retrieves the STATSTG structure for this stream
	virtual HRESULT Stat(::STATSTG* pstatstg, DWORD grfStatFlag);

	// UnlockRegion (IComStream)
	//
	// Removes the access restriction on a range of bytes previously restricted
	virtual HRESULT UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);

	// Write (IComStream)
	//
	// Writes a specified number of bytes into the stream object
	virtual HRESULT Write(void const* pv, ULONG cb, ULONG* pcbWritten);

private:

	// PRIVATE CONSTRUCTOR
	ComStream(IStream* pStream, ComStream^ parent);

	// DESTRUCTOR / FINALIZER
	~ComStream();
	!ComStream();

	//-----------------------------------------------------------------------
	// Private Member Functions

	// Clone (IComStream)
	//
	// Creates a new stream object with its own seek pointer
	virtual HRESULT Clone(IStream**) sealed = IComStream::Clone
		{ throw gcnew NotSupportedException(); }

	//-----------------------------------------------------------------------
	// Member Variables

	bool					m_disposed;			// Object disposal flag
	IStream*				m_pStream;			// Contained IStream
	ComStream^				m_parent;			// Cloned stream parent
	List<WeakReference^>^	m_clones;			// List of clones
};

//---------------------------------------------------------------------------

END_NAMESPACE(structured)
END_NAMESPACE(data)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __COMSTREAM_H_