//---------------------------------------------------------------------------
// StorageSummaryInformation Declarations
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

#ifndef __STORAGESUMMARYINFORMATION_H_
#define __STORAGESUMMARYINFORMATION_H_
#pragma once

#include "IComPropertySetStorage.h"			// Include IComPropSetStorage decls
#include "StorageException.h"				// Include StorageException decls
#include "StorageUtil.h"					// Include StorageUtil declarations

#pragma warning(push, 4)					// Enable maximum compiler warnings
#pragma warning(disable:4461)				// "Finalizer without destructor"

using namespace System;
using namespace System::ComponentModel;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(data)
BEGIN_NAMESPACE(structured)

//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------

// SUMMARYINFO_DATETIME_PROPERTY
//
// Defines a standard read/write integer property for this class
#define SUMMARYINFO_DATETIME_PROPERTY(__name, __code) \
	property DateTime __name { \
	DateTime get(void) { return GetFileTimeValue(__code); } \
	void set(DateTime value) { SetFileTimeValue(__code, value); } }

// SUMMARYINFO_INTEGER_PROPERTY
//
// Defines a standard read/write integer property for this class
#define SUMMARYINFO_INTEGER_PROPERTY(__name, __code) \
	property int __name { \
	int get(void) { return GetIntegerValue(__code); } \
	void set(int value) { SetIntegerValue(__code, value); } }

// SUMMARYINFO_STRING_PROPERTY
//
// Defines a standard read/write string property for this class
#define SUMMARYINFO_STRING_PROPERTY(__name, __code) \
	property String^ __name { \
	String^ get(void) { return GetStringValue(__code); } \
	void set(String^ value) { SetStringValue(__code, value); } }

// SUMMARYINFO_TIMESPAN_PROPERTY
//
// Defines a standard read/write integer property for this class
#define SUMMARYINFO_TIMESPAN_PROPERTY(__name, __code) \
	property TimeSpan __name { \
	TimeSpan get(void) { return GetTimeSpanValue(__code); } \
	void set(TimeSpan value) { SetTimeSpanValue(__code, value); } }

//---------------------------------------------------------------------------
// Class StorageSummaryInformation
//
// StorageSummaryInformation provides a wrapper around the FMTID_StorageSummaryInfo
// property set attached to the root storage of the compound file
//---------------------------------------------------------------------------

STRUCTURED_STORAGE_PUBLIC ref class StorageSummaryInformation sealed
{
public:

	//-----------------------------------------------------------------------
	// Properties

	property bool IsReadOnly { bool get(void) { return m_readOnly; } }

	SUMMARYINFO_STRING_PROPERTY		(ApplicationName,	PIDSI_APPNAME);
	SUMMARYINFO_STRING_PROPERTY		(Author,			PIDSI_AUTHOR);
	SUMMARYINFO_INTEGER_PROPERTY	(Characters,		PIDSI_CHARCOUNT);
	SUMMARYINFO_STRING_PROPERTY		(Comments,			PIDSI_COMMENTS);
	SUMMARYINFO_DATETIME_PROPERTY	(Created,			PIDSI_CREATE_DTM);
	SUMMARYINFO_STRING_PROPERTY		(Keywords,			PIDSI_KEYWORDS);
	SUMMARYINFO_DATETIME_PROPERTY	(LastPrinted,		PIDSI_LASTPRINTED);
	SUMMARYINFO_DATETIME_PROPERTY	(LastSaved,			PIDSI_LASTSAVE_DTM);
	SUMMARYINFO_STRING_PROPERTY		(LastSavedBy,		PIDSI_LASTAUTHOR);
	SUMMARYINFO_INTEGER_PROPERTY	(Pages,				PIDSI_PAGECOUNT);
	SUMMARYINFO_STRING_PROPERTY		(RevisionNumber,	PIDSI_REVNUMBER);
	SUMMARYINFO_INTEGER_PROPERTY	(Security,			PIDSI_DOC_SECURITY);
	SUMMARYINFO_STRING_PROPERTY		(Subject,			PIDSI_SUBJECT);
	SUMMARYINFO_STRING_PROPERTY		(Template,			PIDSI_TEMPLATE);
	SUMMARYINFO_TIMESPAN_PROPERTY	(TimeSpentEditing,	PIDSI_EDITTIME);
	SUMMARYINFO_STRING_PROPERTY		(Title,				PIDSI_TITLE);
	SUMMARYINFO_INTEGER_PROPERTY	(Words,				PIDSI_WORDCOUNT);

internal:

	// INTERNAL CONSTRUCTOR
	StorageSummaryInformation(IComPropertySetStorage^ storage);

	void InternalDispose(void);

private:

	// FINALIZER
	!StorageSummaryInformation();

	//-----------------------------------------------------------------------
	// Private Member Functions

	DateTime	GetFileTimeValue(PROPID propid);
	void		SetFileTimeValue(PROPID propid, DateTime value);

	int			GetIntegerValue(PROPID propid);
	void		SetIntegerValue(PROPID propid, int value);

	TimeSpan	GetTimeSpanValue(PROPID propid);
	void		SetTimeSpanValue(PROPID propid, TimeSpan value);

	String^		GetStringValue(PROPID propid);
	void		SetStringValue(PROPID propid, String^ value);

	//-----------------------------------------------------------------------
	// Member Variables

	bool					m_disposed;				// Object disposal
	bool					m_readOnly;				// Read-Only flag
	IPropertyStorage*		m_pPropertyStorage;		// Contained IPropertyStg
};

//---------------------------------------------------------------------------

END_NAMESPACE(structured)
END_NAMESPACE(data)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __STORAGESUMMARYINFORMATION_H_
