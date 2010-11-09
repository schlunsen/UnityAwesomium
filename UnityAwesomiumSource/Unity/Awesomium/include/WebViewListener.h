/*
	This file is a part of Awesomium, a library that makes it easy for 
	developers to embed web-content in their applications.

	Copyright (C) 2009 Khrona. All rights reserved. Awesomium is a trademark of Khrona.
*/

#ifndef __WEBVIEWLISTENER_H__
#define __WEBVIEWLISTENER_H__

#include <string>
#include "JSValue.h"

#if defined(_WIN32)
#include <windows.h>
#endif

namespace Awesomium {

class WebView;

/**
* WebViewListener is a virtual interface that you can use to receive notifications
* from a certain WebView. Simply make a class that inherits from WebViewListener
* and register it via WebView::setListener.
*/
class _OSMExport WebViewListener
{
public:
	/**
	* This event is fired when a WebView begins navigating to a new URL.
	*
	* @param	caller	The WebView that fired the event.
	*
	* @param	url		The URL that is being navigated to.
	*
	* @param	frameName	The name of the frame that this event originated from.
	*/
	virtual void onBeginNavigation(Awesomium::WebView* caller, const std::string& url, const std::wstring& frameName) = 0;

	/**
	* This event is fired when a WebView begins to actually receive data from a server.
	*
	* @param	caller	The WebView that fired the event.
	*
	* @param	url		The URL of the frame that is being loaded.
	*
	* @param	frameName	The name of the frame that this event originated from.
	*
	* @param	statusCode	The HTTP status code returned by the server.
	*
	* @param	mimeType	The mime-type of the content that is being loaded.
	*/
	virtual void onBeginLoading(Awesomium::WebView* caller, const std::string& url, const std::wstring& frameName, int statusCode, const std::wstring& mimeType) = 0;

	/**
	* This event is fired when all loads have finished for a WebView.
	*
	* @param	caller	The WebView that fired the event.
	*/
	virtual void onFinishLoading(Awesomium::WebView* caller) = 0;

	/**
	* This event is fired when a Client callback has been invoked via Javascript from a page.
	*
	* @param	caller	The WebView that fired the event.
	*
	* @param	objectName	The name of the Javascript Object that contains the invoked callback.
	*
	* @param	callbackName	The name of the callback that was invoked (must have been previously bound via WebView::setObjectCallback).
	*
	* @param	args	The arguments passed to the callback.
	*/
	virtual void onCallback(Awesomium::WebView* caller, const std::wstring& objectName, const std::wstring& callbackName, const Awesomium::JSArguments& args) = 0;
	
	/**
	* This event is fired when a page title is received.
	*
	* @param	caller	The WebView that fired the event.
	*
	* @param	title	The page title.
	*
	* @param	frameName	The name of the frame that this event originated from.
	*/
	virtual void onReceiveTitle(Awesomium::WebView* caller, const std::wstring& title, const std::wstring& frameName) = 0;

	/**
	* This event is fired when a tooltip has changed state.
	*
	* @param	caller	The WebView that fired the event.
	*
	* @param	tooltip		The tooltip text (or, is an empty string when the tooltip should disappear).
	*/
	virtual void onChangeTooltip(Awesomium::WebView* caller, const std::wstring& tooltip) = 0;

#if defined(_WIN32)
	/**
	* This event is fired when a cursor has changed state. [Windows-only]
	*
	* @param	caller	The WebView that fired the event.
	*
	* @param	cursor	The cursor handle/type.
	*/
	virtual void onChangeCursor(Awesomium::WebView* caller, const HCURSOR& cursor) = 0;
#endif

	/**
	* This event is fired when keyboard focus has changed.
	*
	* @param	caller	The WebView that fired the event.
	*
	* @param	isFocused	Whether or not the keyboard is currently focused.
	*/
	virtual void onChangeKeyboardFocus(Awesomium::WebView* caller, bool isFocused) = 0;

	/**
	* This event is fired when the target URL has changed. This is usually the result of 
	* hovering over a link on the page.
	*
	* @param	caller	The WebView that fired the event.
	*
	* @param	url	The updated target URL (or empty if the target URL is cleared).
	*/
	virtual void onChangeTargetURL(Awesomium::WebView* caller, const std::string& url) = 0;

	/**
	* This event is fired when an external link is attempted to be opened. An external link
	* is any link that normally opens in a new window in a standard browser (for example, links
	* with target="_blank", calls to window.open(url), and URL open events from Flash plugins).
	* External links may or may not be automatically opened by a WebView (please see
	* WebView::setOpensExternalLinksInCallingFrame).
	*
	* @param	caller	The WebView that fired the event.
	*
	* @param	url		The URL of the external link.
	*
	* @param	source	If the external link originated from a call to window.open(), this value
	*					is "window.open". Else, if the link originated from a Flash plugin, this
	*					value is "flash". Otherwise, this value is the name of the frame that
	*					contains the link.
	*/
	virtual void onOpenExternalLink(Awesomium::WebView* caller, const std::string& url, const std::wstring& source) = 0;
};

}

#endif