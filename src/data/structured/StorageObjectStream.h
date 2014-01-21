//---------------------------------------------------------------------------
// StorageObjectStream Declarations
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

#ifndef __STORAGEOBJECTSTREAM_H_
#define __STORAGEOBJECTSTREAM_H_
#pragma once

#include "ComStream.h"					// Include ComStream declarations
#include "StorageObjectStreamMode.h"	// Include stream mode enumeration
#include "StorageException.h"			// Include StorageException declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::IO;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(data)
BEGIN_NAMESPACE(structured)

//---------------------------------------------------------------------------
// Class StorageObjectStream
//
// StorageObjectStream wraps an existing IStream pointer from a storage object
//---------------------------------------------------------------------------

STRUCTURED_STORAGE_PUBLIC ref class StorageObjectStream abstract : public Stream
{
public:

	//-----------------------------------------------------------------------
	// Stream Overrides

	//
	// NOTE: Do not override .Close() ... just implement IDisposable property
	//

	virtual void	Flush() override;
	virtual	int		Read(array<Byte>^ buffer, int offset, int count) override;
	virtual __int64	Seek(__int64 offset, SeekOrigin origin) override;
	virtual void	SetLength(__int64 value) override;
	virtual void	Write(array<Byte>^ buffer, int offset, int count) override;

	virtual property bool		CanRead { bool get(void) override; }
	virtual property bool		CanSeek { bool get(void) override; }
	virtual property bool		CanWrite { bool get(void) override; }
	virtual property __int64	Length { __int64 get(void) override; }

	virtual property __int64 Position 
	{ 
		__int64 get(void) override { return Seek(0, SeekOrigin::Current); } 
		void set(__int64 pos) override { Seek(pos, SeekOrigin::Begin); }
	}

internal:

	// INTERNAL CONSTRUCTOR
	StorageObjectStream(ComStream^ stream, StorageObjectStreamMode mode);

	//-----------------------------------------------------------------------
	// Internal Properties

	property bool IsDisposed { bool get(void) { return m_disposed; } } 

private:

	// DESTRUCTOR / FINALIZER
	~StorageObjectStream() { m_stream = nullptr; m_disposed = true; }
	//!StorageObjectStream();

	//-----------------------------------------------------------------------
	// Member Variables

	bool							m_disposed;		// Object disposal flag
	StorageObjectStreamMode			m_mode;			// Object stream mode
	ComStream^						m_stream;		// Parent stream instance
	//IStream*						m_pStream;		// Contained COM stream
};

//---------------------------------------------------------------------------

END_NAMESPACE(structured)
END_NAMESPACE(data)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __STORAGEOBJECTSTREAM_H_
