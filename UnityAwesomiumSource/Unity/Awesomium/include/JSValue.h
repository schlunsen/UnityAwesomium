/*
	This file is a part of Awesomium, a library that makes it easy for 
	developers to embed web-content in their applications.

	Copyright (C) 2009 Khrona. All rights reserved. Awesomium is a trademark of Khrona.
*/

#ifndef __JSVALUE_H__
#define __JSVALUE_H__

#include <string>
#include <vector>
#include <map>
#include "PlatformUtils.h"

namespace Impl { struct VariantValue; }

namespace Awesomium 
{

class WebView;

class _OSMExport JSIdentifier
{
public:
	JSIdentifier(const char* value);
	JSIdentifier(const std::string& value);
	JSIdentifier(const wchar_t* value);
	JSIdentifier(const std::wstring& value);
	JSIdentifier(int value);
	JSIdentifier(const JSIdentifier& original);
	~JSIdentifier();
	bool operator<(const JSIdentifier& rhs) const;

	bool isString() const;

	const std::wstring& getString() const;
	int getInteger() const;

protected:
	union { int intValue; std::wstring* strValue; } data;
	bool isStringData;
};

/**
* JSValue is a class that represents a Javascript value. It can be initialized from
* and converted to several types: boolean, integer, double, string
*/
class _OSMExport JSValue
{
	Impl::VariantValue* varValue;
public:

	typedef std::map<std::wstring, JSValue> Object;
	typedef std::vector<JSValue> Array;

	/// Creates a null JSValue.
	JSValue();

	/// Creates a JSValue initialized with a boolean.
	JSValue(bool value);

	/// Creates a JSValue initialized with an integer.
	JSValue(int value);

	/// Creates a JSValue initialized with a double.
	JSValue(double value);

	/// Creates a JSValue initialized with an ASCII string.
	JSValue(const char* value);

	/// Creates a JSValue initialized with an ASCII string.
	JSValue(const std::string& value);

	/// Creates a JSValue initialized with a wide string.
	JSValue(const wchar_t* value);

	/// Creates a JSValue initialized with a wide string.
	JSValue(const std::wstring& value);

	/// Creates a JSValue initialized with an object.
	JSValue(const Object& value);

	/// Creates a JSValue initialized with an array.
	JSValue(const Array& value);

	JSValue(const JSValue& original);

	~JSValue();

	JSValue& operator=(const JSValue& rhs);

	/// Returns whether or not this JSValue is a boolean.
	bool isBoolean() const;

	/// Returns whether or not this JSValue is an integer.
	bool isInteger() const;

	/// Returns whether or not this JSValue is a double.
	bool isDouble() const;

	/// Returns whether or not this JSValue is a number (integer or double).
	bool isNumber() const;

	/// Returns whether or not this JSValue is a string.
	bool isString() const;

	/// Returns whether or not this JSValue is an array.
	bool isArray() const;

	/// Returns whether or not this JSValue is an object.
	bool isObject() const;

	/// Returns whether or not this JSValue is null.
	bool isNull() const;

	/// Returns this JSValue as a wide string (converting if necessary).
	const std::wstring& toString() const;

	/// Returns this JSValue as an integer (converting if necessary).
	int toInteger() const;

	/// Returns this JSValue as a double (converting if necessary).
	double toDouble() const;

	/// Returns this JSValue as a boolean (converting if necessary).
	bool toBoolean() const;

	/// Gets a reference to this JSValue's array value (will assert if not an array type)
	Array& getArray();

	/// Gets a constant reference to this JSValue's array value (will assert if not an array type)
	const Array& getArray() const;

	/// Gets a reference to this JSValue's object value (will assert if not an object type)
	Object& getObject();

	/// Gets a constant reference to this JSValue's object value (will assert if not an object type)
	const Object& getObject() const;
};

typedef std::vector<JSValue> JSArguments;

/**
* FutureJSValue is a special wrapper around a JSValue that allows
* asynchronous retrieval of the actual value at a later time.
*
* If you are unfamiliar with the concept of a 'Future', please see:
* <http://en.wikipedia.org/wiki/Futures_and_promises>
*/
class _OSMExport FutureJSValue
{
public:
	FutureJSValue();
	~FutureJSValue();

	/**
	* If the internal JSValue has been computed, immediately returns
	* the value, else, blocks the calling thread until it has.
	*/
	const JSValue& get();

protected:
	void init(WebView* source, int requestID);

	JSValue value;
	WebView* source;
	int requestID;

	friend class WebView;
};

}

#endif