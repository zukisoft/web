//---------------------------------------------------------------------------
// StorageOpenMode Declarations
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

#ifndef __STORAGEOPENMODE_H_
#define __STORAGEOPENMODE_H_
#pragma once

#pragma warning(push, 4)					// Enable maximum compiler warnings

using namespace System::IO;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(data)
BEGIN_NAMESPACE(structured)

//---------------------------------------------------------------------------
// StorageOpenMode Enumeration
//
// The StorageOpenMode enumeration defines a subset of the System::IO::FileMode
// enumeration as they pertain to this library
//---------------------------------------------------------------------------

STRUCTURED_STORAGE_PUBLIC enum struct StorageOpenMode
{
	Create			= FileMode::Create,			// Create w/overwrite
	CreateNew		= FileMode::CreateNew,		// Create fail if exists
	Open			= FileMode::Open,			// Open existing
	OpenOrCreate	= FileMode::OpenOrCreate,	// Open or create w/overwrite
};

//---------------------------------------------------------------------------

END_NAMESPACE(structured)
END_NAMESPACE(data)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif		// __STORAGEOPENMODE_H_