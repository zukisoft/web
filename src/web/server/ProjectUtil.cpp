//---------------------------------------------------------------------------
// ProjectUtil Implementation
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

#include "stdafx.h"						// Include project pre-compiled headers
#include "ProjectUtil.h"				// Include ProjectUtil declarations
#include "Host.h"						// Include Host declarations

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// ProjectUtil::FastPtrToStringAnsi (static)
//
// Significanltly faster version of Marshal::PtrToStringAnsi.  In testing,
// this routine performs over twice as fast.  Should not be used for very
// long strings, since it allocates memory on the stack
//
// Arguments:
//
//	psz			- Pointer to the ANSI string to be converted

String^ ProjectUtil::FastPtrToStringAnsi(const char *psz)
{
	return (psz) ? FastPtrToStringAnsi(psz, strlen(psz)) : nullptr;
}

//---------------------------------------------------------------------------
// ProjectUtil::FastPtrToStringAnsi (static)
//
// Significanltly faster version of Marshal::PtrToStringAnsi.  In testing,
// this routine performs over twice as fast.  Should not be used for very
// long strings, since it allocates memory on the stack
//
// Arguments:
//
//	psz			- Pointer to the ANSI string to be converted
//	cch			- Length of the input string, in characters

String^ ProjectUtil::FastPtrToStringAnsi(const char *psz, size_t cch)
{
	if(!psz) return nullptr;			// NULL in, NULL out
	if(cch <= 0) return String::Empty;	// No need to convert zero-length

	// Use _alloca to quickly reserve some stack space for the conversion, and
	// invoke a standard MultiByteToWideChar to perform the Unicode conversion.
	
	wchar_t* rgwsz = reinterpret_cast<wchar_t*>(_alloca(cch * sizeof(wchar_t)));

	if(MultiByteToWideChar(CP_ACP, 0, psz, static_cast<int>(cch), rgwsz, 
		static_cast<int>(cch)) == 0) throw gcnew Win32Exception();
	else return gcnew String(rgwsz, 0, static_cast<int>(cch));
}

//---------------------------------------------------------------------------
// ProjectUtil::FindOpenTcpPort
//
// Locates an available TCP port that can be used for dynamic configurations
//
// Arguments:
//
//	NONE

int ProjectUtil::FindOpenTcpPort(void)
{
	Random^					random;				// Random number generator
	int						basePort;			// Base port number to use
	MIB_TCPTABLE*			pTcpTable = NULL;	// Pointer to the TCP table
	DWORD					cbTable = 0;		// Start with an 8KB buffer
	bool					inuse;				// Flag if port is in use already
	DWORD					dwResult;			// Result from function call

	// To avoid getting the same port each and every time, select something
	// between 32768 and 49152 as the starting point for the search

	random = gcnew Random(static_cast<int>(DateTime::Now.Ticks));
	basePort = 32768 + random->Next(16384);

	try {

		// Use the old GetTcpTable instead of GetExtendedTcpTable to not require
		// an updated Platform SDK over what VS2005 provides out of the box ...

		dwResult = GetTcpTable(NULL, &cbTable, FALSE);
		
		do {

			pTcpTable = reinterpret_cast<MIB_TCPTABLE*>(realloc(pTcpTable, cbTable));
			if(!pTcpTable) throw gcnew OutOfMemoryException();

			dwResult = GetTcpTable(pTcpTable, &cbTable, FALSE);

		} while(dwResult == ERROR_INSUFFICIENT_BUFFER);

		if(dwResult != NO_ERROR) throw gcnew Win32Exception(dwResult);

		// Starting from the random base port, scan to find one that's not in use
		// by some local endpoint on this system already

		for(int port = basePort; port <= 65535; port++) {

			inuse = false;				// Assume the port is not in use anywhere

			for(DWORD tableIndex = 0; tableIndex < pTcpTable->dwNumEntries; tableIndex++) {

				// FIX: The ports are stored in network byte order, so we need to ntohs them
				// before we can actually know what they are.  Also of note is that they are
				// 16-bit values stored in a 32-bit variable, so we have to be sure to cast them
				// back into unsigned shorts BEFORE we convert the endian-ness.

				unsigned short localPort = static_cast<unsigned short>(pTcpTable->table[tableIndex].dwLocalPort);
				localPort = ntohs(localPort);

				if(port == localPort) { inuse = true; break; }
			}

			if(!inuse) return port;		// <-- Found an unused port; return it
		}
	}

	finally { if(pTcpTable) free(pTcpTable); }

	return -1;							// <-- Could not locate a unique port value
}

//---------------------------------------------------------------------------
// ProjectUtil::TryGetHostWithWait
//
// Attempts to access a named Host instance from an ApplicationManager.
// If the host isn't located, it could be in the process of restarting so
// wait a little bit and try again before failing outright
//
// Arguments:
//
//	manager		- ApplicationManager instance
//	appid		- Host application ID to locate
//	host		- On success, contains the located Host instance

bool ProjectUtil::TryGetHostWithWait(ApplicationManager^ manager, String^ appid, Host^% host)
{
	int					attempts = 0;		// Number of attempted GetObjects()

	host = nullptr;					// Initialize [out] reference to NULL

	if(manager == nullptr) throw gcnew ArgumentNullException();
	if(String::IsNullOrEmpty(appid)) throw gcnew ArgumentNullException();

	//
	// NOTE: This model is a little hokey, it can be better.  It will do
	// for an initial release. Ideally there should be an event or something
	// to wait on that can be passed from Host->Host as they are restarted
	//

	do {

		attempts++;
		host = dynamic_cast<Host^>(manager->GetObject(appid, Host::typeid));
		
		if(host != nullptr) return true;		// Found it
		else Thread::Sleep(250);				// Wait a smidge longer

	} while(attempts < 5);			// Do this up to 5 times (for now)

	return false;					// Could not locate the host instance
}

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
