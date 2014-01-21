//---------------------------------------------------------------------------
// WebServiceReservationCollection Implementation
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

#include "stdafx.h"								// Include pre-compiled headers
#include "WebServiceReservationCollection.h"	// Include class declarations

#pragma warning(push, 4)						// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(config)

//---------------------------------------------------------------------------
// WebServiceReservationCollection Constructor
//
// Arguments:
//
//	NONE

WebServiceReservationCollection::WebServiceReservationCollection()
{
	// Construct the contained collection and perform the initial refresh

	m_col = gcnew List<WebReservation^>();
	Refresh();
}

//---------------------------------------------------------------------------
// WebServiceReservationCollection::Add
//
// Adds a new namespace reservation to the system
//
// Arguments:
//
//	reservation		- The new reservation to be added

void WebServiceReservationCollection::Add(WebReservation^ reservation)
{
	HTTP_SERVICE_CONFIG_URLACL_SET		input;		// Input data set to API call
	PinnedStringPtr						pinPrefix;	// Pinned URL prefix string
	PinnedStringPtr						pinSddl;	// Pinned SDDL string
	ULONG								ulResult;	// Result from API function call

	if(reservation == nullptr) throw gcnew ArgumentNullException();
	if(Contains(reservation)) throw gcnew ArgumentException("The specified reservation already exists in this collection");

	// Initialize the URLACL_SET structure with pointers to the prefix and SDDL data

	pinPrefix = PtrToStringChars(reservation->Prefix);
	pinSddl = PtrToStringChars(reservation->Security->Sddl);
	input.KeyDesc.pUrlPrefix = const_cast<LPWSTR>(pinPrefix);
	input.ParamDesc.pStringSecurityDescriptor = const_cast<LPWSTR>(pinSddl);
	
	// Attempt to actually add the reservation into the HTTP system first

	ulResult = HttpSetServiceConfiguration(0, HttpServiceConfigUrlAclInfo, &input,
		sizeof(HTTP_SERVICE_CONFIG_URLACL_SET), NULL);
	if(ulResult != ERROR_SUCCESS) throw gcnew Win32Exception(ulResult);

	m_col->Add(gcnew WebReservation(reservation));		// Add to collection
}

//---------------------------------------------------------------------------
// WebServiceReservationCollection::Contains
//
// Determines if a namespace reservation exists or not
//
// Arguments:
//
//	reservation		- The new reservation to be tested

bool WebServiceReservationCollection::Contains(WebReservation^ reservation)
{
	if(reservation == nullptr) throw gcnew ArgumentNullException();

	// Just iterate over the collection matching on the prefix string

	for each(WebReservation^ item in m_col) if(String::Compare(item->Prefix, 
		reservation->Prefix, StringComparison::OrdinalIgnoreCase) == 0) return true;

	return false;				// The specified reservation does not exist
}

//---------------------------------------------------------------------------
// WebServiceReservationCollection::Refresh
//
// Clears out and reloads the contents of the contained collection
//
// Arguments:
//
//	NONE

void WebServiceReservationCollection::Refresh(void)
{
	HTTP_SERVICE_CONFIG_URLACL_QUERY	query;		// Input data to function call
	PHTTP_SERVICE_CONFIG_URLACL_SET		pOutput;	// Output data from function call
	ULONG								cbOutput;	// Length of the output data
	ULONG								ulResult;	// Result from function call

	m_col->Clear();						// Remove all existing items

	// Set up the query information. For the reservation list, we repeatedly call
	// into HttpQueryServiceConfiguration with an incrementing "dwToken" until
	// we have retrieved all of the information available to be retrieved from HTTP.SYS

	ZeroMemory(&query, sizeof(HTTP_SERVICE_CONFIG_URLACL_QUERY));
	query.QueryDesc = HttpServiceConfigQueryNext;

	do {

		cbOutput = 0;					// Reset buffer length back to zero
		pOutput = NULL;					// Reset pointer back to NULL

		// For each call into HttpQueryServiceConfiguration, we start with a NULL buffer
		// and let us know exactly how much space we need to allocate to hold the result

		ulResult = HttpQueryServiceConfiguration(0, HttpServiceConfigUrlAclInfo, &query, 
			sizeof(HTTP_SERVICE_CONFIG_URLACL_QUERY), NULL, cbOutput, &cbOutput, NULL);
		if(ulResult == ERROR_INSUFFICIENT_BUFFER) {

			// Now that we know how much memory we need, go ahead and try to allocate it from the heap

			pOutput = reinterpret_cast<PHTTP_SERVICE_CONFIG_URLACL_SET>(Marshal::AllocHGlobal(cbOutput).ToPointer());
			if(!pOutput) throw gcnew InsufficientMemoryException();

			try {

				// Invoke the API again, this time passing in the buffer that we allocated for data.
				// This needs to return ERROR_SUCCESS or ERROR_NO_MORE_ITEMS to be successful

				ulResult = HttpQueryServiceConfiguration(0, HttpServiceConfigUrlAclInfo, &query, 
					sizeof(HTTP_SERVICE_CONFIG_URLACL_QUERY), pOutput, cbOutput, &cbOutput, NULL);

				if(ulResult == ERROR_SUCCESS) 
					m_col->Add(gcnew WebReservation(gcnew String(pOutput->KeyDesc.pUrlPrefix), 
						gcnew String(pOutput->ParamDesc.pStringSecurityDescriptor)));
			}

			finally { Marshal::FreeHGlobal(IntPtr(pOutput)); }		// Always release allocated memory
		
		} // (if ulResult == ERROR_INSUFFICIENT_BUFFER...

		query.dwToken++;		// <-- Increment the "token" value to get the next entry

	} while(ulResult == ERROR_SUCCESS);

	// The only error code that should have legitimately broken the loop
	// is ERROR_NO_MORE_ITEMS, which simply means we hit the end of the list

	if(ulResult != ERROR_NO_MORE_ITEMS) throw gcnew Win32Exception(ulResult);
}

//---------------------------------------------------------------------------
// WebServiceReservationCollection::Remove
//
// Removes a namespace reservation from the system
//
// Arguments:
//
//	reservation		- The reservation to be removed

bool WebServiceReservationCollection::Remove(WebReservation^ reservation)
{
	PinnedStringPtr						pinPrefix;	// Pinned prefix string
	HTTP_SERVICE_CONFIG_URLACL_SET		input;		// Input data to function call
	ULONG								ulResult;	// Result from function call

	if(reservation == nullptr) throw gcnew ArgumentNullException();
	if(!Contains(reservation)) return false;

	// Attempt to physically remove the reservation from the system first

	pinPrefix = PtrToStringChars(reservation->Prefix);
	ZeroMemory(&input, sizeof(HTTP_SERVICE_CONFIG_URLACL_SET));
	input.KeyDesc.pUrlPrefix = const_cast<LPWSTR>(pinPrefix);

	// Note that we allow ERROR_FILE_NOT_FOUND to go without exception, it just means
	// that the URL didn't exist on the system, which means the collection was out
	// of date, so we still remove it from the collection and return TRUE

	ulResult = HttpDeleteServiceConfiguration(0, HttpServiceConfigUrlAclInfo, &input, 
		sizeof(HTTP_SERVICE_CONFIG_URLACL_SET), NULL);
	if((ulResult != ERROR_SUCCESS) && (ulResult != ERROR_FILE_NOT_FOUND)) 
		throw gcnew Win32Exception(ulResult);

	// Now remove the item from the collection without using an enumerator.
	// There should only be one removed, but iterate the whole collection just in case
	// (that should lead to a nice unforseen consequence to wrestle with later)

	int index = 0;
	while(index < m_col->Count) {

		if(String::Compare(m_col[index]->Prefix, reservation->Prefix, StringComparison::OrdinalIgnoreCase) == 0)
			m_col->RemoveAt(index);
		else index++;
	}

	return true;
}

//---------------------------------------------------------------------------

END_NAMESPACE(config)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)
