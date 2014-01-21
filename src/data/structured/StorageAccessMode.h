//---------------------------------------------------------------------------
// StorageAccessMode Declarations
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

#ifndef __STORAGEACCESSMODE_H_
#define __STORAGEACCESSMODE_H_
#pragma once

#pragma warning(push, 4)					// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(data)
BEGIN_NAMESPACE(structured)

//---------------------------------------------------------------------------
// StorageAccessMode Enumeration
//
// The StorageAccessMode enumeration defines the various options for access
// and sharing when opening a StructuredStorage file.
//
// For now, I'm only supporting DIRECT mode and very limited sharing.  At 
// some point in the future, this may change to allow transactional and/or
// DIRECT_SWMR support (after many changes to the code)
//---------------------------------------------------------------------------

STRUCTURED_STORAGE_PUBLIC enum struct StorageAccessMode
{
	Exclusive			= STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
	ReadOnlyExclusive	= STGM_DIRECT | STGM_READ | STGM_SHARE_EXCLUSIVE,
	ReadOnlyShared		= STGM_DIRECT | STGM_READ | STGM_SHARE_DENY_WRITE,
};

//---------------------------------------------------------------------------

END_NAMESPACE(structured)
END_NAMESPACE(data)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif		// __STORAGEACCESSMODE_H_