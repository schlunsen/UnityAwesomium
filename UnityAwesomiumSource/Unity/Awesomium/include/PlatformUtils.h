/*
	This file is a part of Awesomium, a library that makes it easy for 
	developers to embed web-content in their applications.

	Copyright (C) 2009 Khrona. All rights reserved. Awesomium is a trademark of Khrona.
*/

#ifndef __PLATFORM_UTILS_H__
#define __PLATFORM_UTILS_H__

#if defined(__WIN32__) || defined(_WIN32)
#	if defined(OSM_NONCLIENT_BUILD)
#		define _OSMExport __declspec( dllexport )
#	else
#		define _OSMExport __declspec( dllimport )
#	endif
#elif defined(__APPLE__)
#	define _OSMExport __attribute__((visibility("default")))
#else
#	define _OSMExport
#endif

namespace Awesomium {
	_OSMExport const std::wstring& convertAsciiStringToWide(const std::string& asciiString);
	
	_OSMExport const std::string& convertWideStringToAscii(const std::wstring& wideString);

	_OSMExport const std::wstring& getCurrentWorkingDirectory();
};

namespace Impl {
void initCommandLine();
void initWebCorePlatform();
}

#endif