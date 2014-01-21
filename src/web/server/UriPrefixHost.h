//---------------------------------------------------------------------------
// UriPrefixHost Declarations
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

#ifndef __URIPREFIXHOST_H_
#define __URIPREFIXHOST_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Net;
using namespace System::Net::Sockets;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(server)

//---------------------------------------------------------------------------
// Class UriPrefixHost
//
// Defines common prefix host names (localhost, DNS name, NetBIOS name) as
// a series of strongly typed objects instead of simple constants
//---------------------------------------------------------------------------

[Serializable()]
public value class UriPrefixHost sealed
{
private:

	//-----------------------------------------------------------------------
	// Private Type Declarations

	// _Values
	//
	// Defines an enumeration used for the underlying values for the sole purpose
	// of allowing .ToString() to provide enumeration-like output ...
	enum class _Values
	{
		All					= static_cast<int>('+'),
		AllUnhandled		= static_cast<int>('*'),
		LocalComputerName	= static_cast<int>('!'),
		LocalDnsName		= static_cast<int>('#'),
		LocalIPAddress		= static_cast<int>('$'),
		Loopback			= static_cast<int>('^'),
	};

public:

	//-----------------------------------------------------------------------
	// Overloaded Operators

	bool operator ==(UriPrefixHost rhs) { return rhs.m_host == this->m_host; }
	bool operator !=(UriPrefixHost rhs) { return rhs.m_host != this->m_host; }

	static operator String^(UriPrefixHost val) { return val.m_host; }

	//-----------------------------------------------------------------------
	// Member Functions

	// ToString (Object)
	//
	// Converts this object instance into it's string representation
	virtual String^ ToString(void) override 
	{ 
		// Use the enumeration value whenever possible for the generic ToString,
		// since that will be used for things like persistance into XML ...

		if((m_host != nullptr) && (m_host->Length == 1)) {

			String^ result = Enum::GetName(_Values::typeid, static_cast<int>(m_host[0]));
			if(result != nullptr) return result;
		}

		return m_host;					// Just return whatever has been set
	}

	//-----------------------------------------------------------------------
	// Fields

	static initonly UriPrefixHost All				= UriPrefixHost(_Values::All);
	static initonly UriPrefixHost AllUnhandled		= UriPrefixHost(_Values::AllUnhandled);
	static initonly UriPrefixHost LocalComputerName	= UriPrefixHost(_Values::LocalComputerName);
	static initonly UriPrefixHost LocalDnsName		= UriPrefixHost(_Values::LocalDnsName);
	static initonly UriPrefixHost LocalIPAddress	= UriPrefixHost(_Values::LocalIPAddress);
	static initonly UriPrefixHost Loopback			= UriPrefixHost(_Values::Loopback);

internal:

	// INTERNAL CONSTRUCTOR
	UriPrefixHost(String^ host) : m_host(host) {}

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Parse
	//
	// Attempts to convert a string into a valid UriPrefixHost value
	static UriPrefixHost Parse(String^ value)
	{
		try {

			// This process was a little too convoluted to handle in a single line,
			// since we have to cast the returned boxed enum value twice and string it

			Object^ boxedValue = Enum::Parse(_Values::typeid, value, true);
			return UriPrefixHost(static_cast<__wchar_t>(static_cast<int>(boxedValue)).ToString());
		}

		catch(Exception^) { return UriPrefixHost(value); }
	}

	// ToString
	//
	// Converts this object instance into it's string representation, optionally
	// resolving the host name if it was a special case character code
	virtual String^ ToString(bool resolve)
	{
		// If we're not supposed to resolve the host name, or there is no host name
		// that can be resolved, just return via the standard ToString() instead

		if((!resolve) || (String::IsNullOrEmpty(m_host))) return ToString();
		
		if(m_host == "+") return m_host;
		else if(m_host == "*") return m_host;
		else if(m_host == "!") return Environment::MachineName;
		else if(m_host == "#") return GetLocalDnsName();
		else if(m_host == "$") return GetLocalIPAddress();
		else if(m_host == "^") return IPAddress::Loopback->ToString();
		else return ToString();
	}

private:

	// PRIVATE CONSTRUCTOR
	UriPrefixHost(_Values host) : m_host(static_cast<const __wchar_t>(host).ToString()) {}

	//-----------------------------------------------------------------------
	// Private Member Functions

	// GetLocalDnsName
	//
	// Attempts to retrieve the local machine's DNS name
	static String^ GetLocalDnsName(void)
	{
		try { return Dns::GetHostName(); }
		catch(Exception^) { return "localhost"; }
	}

	// GetLocalIPAddress
	//
	// Attempts to retrieve the local machine's public IP address
	static String^ GetLocalIPAddress(void)
	{
		IPHostEntry^			hostEntry;		// Local IP host entry

		try {

			// Map the DNS name of the local system into a host entry, find the
			// first IPv4 entry in there, and use that.  The application can deal
			// with IPv6 or choosing a specific address from a multi-homed machine
			// on it's own if it wants to.  Most people will be happy with this

			hostEntry = Dns::GetHostEntry(Dns::GetHostName());
			for each(IPAddress^ addr in hostEntry->AddressList)
				if(addr->AddressFamily == AddressFamily::InterNetwork) return addr->ToString();

			return IPAddress::Loopback->ToString();		// No IPv4 addrs found
		}

		catch(Exception^) { return IPAddress::Loopback->ToString(); }
	}

	//-----------------------------------------------------------------------
	// Member Variables
	
	initonly String^			m_host;			// Host name string
};

//---------------------------------------------------------------------------

END_NAMESPACE(server)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif		// __URIPREFIXHOST_H_