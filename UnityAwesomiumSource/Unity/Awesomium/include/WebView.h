/*
	This file is a part of Awesomium, a library that makes it easy for 
	developers to embed web-content in their applications.

	Copyright (C) 2009 Khrona. All rights reserved. Awesomium is a trademark of Khrona.
*/

#ifndef __WEBVIEW_H__
#define __WEBVIEW_H__

#include "WebViewListener.h"
#include "WebKeyboardEvent.h"
#include <map>

#if defined(_WIN32)
#pragma warning( disable: 4251 )
#endif

class WebViewWaitState;
class WebViewProxy;
class JSValueFutureImpl;
class FutureValueCallback;
class CheckKeyboardFocusCallback;
class WindowOpenCallback;
namespace base { class Thread; }
class LockImpl;
namespace WebViewEvents { class InvokeCallback; }

namespace Awesomium {

class WebCore;

/**
* Mouse button enumerations, used with WebView::injectMouseDown 
* and WebView::injectMouseUp
*/
enum MouseButton {
	LEFT_MOUSE_BTN,
	MIDDLE_MOUSE_BTN,
	RIGHT_MOUSE_BTN
};

/**
* URL Filtering mode enumerations, used by WebView::setURLFilteringMode
*/
enum URLFilteringMode {
	/**
	* All resource requests and page navigations are ALLOWED 
	* except those that match the URL filters specified.
	*/
	UFM_BLACKLIST, 

	/**
	* All resource requests and page navigations are DENIED 
	* except those that match the URL filters specified.
	*/
	UFM_WHITELIST,

	// No filtering
	UFM_NONE
};

/**
* A simple rectangle class, used with WebView::render
*/
struct _OSMExport Rect {
	int x, y, width, height;

	Rect();
	Rect(int x, int y, int width, int height);
	bool isEmpty() const;
};

/**
* A map of string key/values representing an HTTP header.
*/
typedef std::map<std::string, std::string> HeaderDefinition;

/**
* A WebView is essentially a single instance of a web-browser (created via the WebCore singleton)
* that you can interact with (via input injection, javascript, etc.) and render to an off-screen buffer.
*/
class _OSMExport WebView
{
public:

	/**
	* Explicitly destroys this WebView instance. If you neglect to call this, the WebCore singleton
	* will automatically destroy all lingering WebView instances at shutdown.
	*
	* @note	This should NEVER be called directly from one of the notifications of WebViewListener.
	*/
	void destroy();

	/**
	* Registers a WebViewListener to call upon various events (such as load completions, callbacks, title receptions,
	* cursor changes, etc).
	*
	* @param	listener	The WebViewListener to register or NULL to clear any current registrations.
	*/
	void setListener(WebViewListener* listener);

	/**
	* Retrieves the current WebViewListener.
	*
	* @return	If a WebViewListener is registered, returns a pointer to the instance, otherwise returns 0.
	*/
	WebViewListener* getListener();

	/**
	* Loads a URL into the WebView asynchronously.
	*
	* @param	url	The URL to load.
	*
	* @param	frameName	Optional, the name of the frame to load the URL in; leave this blank to load in the main frame.
	*
	* @param	username	Optional, if the URL requires authentication, the username to authorize as.
	*
	* @param	password	Optional, if the URL requires authentication, the password to use.
	*/
	void loadURL(const std::string& url, const std::wstring& frameName = L"", const std::string& username = "", const std::string& password = "");

	/**
	* Loads a URL into the WebView asynchronously.
	*
	* @param	url	The URL to load.
	*
	* @param	frameName	Optional, the name of the frame to load the URL in; leave this blank to load in the main frame.
	*
	* @param	username	Optional, if the URL requires authentication, the username to authorize as.
	*
	* @param	password	Optional, if the URL requires authentication, the password to use.
	*/
	void loadURL(const std::wstring& url, const std::wstring& frameName = L"", const std::string& username = "", const std::string& password = "");

	/**
	* Loads a string of HTML into the WebView asynchronously.
	*
	* @param	html	The HTML string (ASCII) to load.
	*
	* @param	frameName	Optional, the name of the frame to load the HTML in; leave this blank to load in the main frame.
	*
	* @note	The base directory (specified via WebCore::setBaseDirectory) will be used to resolve
	*		relative URLs/resources (such as images, links, etc).
	*/
	void loadHTML(const std::string& html, const std::wstring& frameName = L"");

	/**
	* Loads a string of HTML into the WebView asynchronously.
	*
	* @param	html	The HTML string (wide) to load.
	*
	* @param	frameName	Optional, the name of the frame to load the HTML in; leave this blank to load in the main frame.
	*
	* @note	The base directory (specified via WebCore::setBaseDirectory) will be used to resolve
	*		relative URLs/resources (such as images, links, etc).
	*/
	void loadHTML(const std::wstring& html, const std::wstring& frameName = L"");

	/**
	* Loads a local file into the WebView asynchronously.
	*
	* @param	file	The file to load.
	*
	* @param	frameName	Optional, the name of the frame to load the file in; leave this blank to load in the main frame.
	*
	* @note	The file should exist within the base directory (specified via WebCore::setBaseDirectory).
	*/
	void loadFile(const std::string& file, const std::wstring& frameName = L"");

	/**
	* Navigates back/forward in history via a relative offset.
	*
	* @note
	*	For example, to go back one page:
	*		myView->goToHistoryOffset(-1);
	*	Or, to go forward one page:
	*		myView->goToHistoryOffset(1);
	*
	* @param	offset	The relative offset in history to navigate to.
	*/
	void goToHistoryOffset(int offset);

	/**
	* Executes a string of Javascript in the context of the current page asynchronously.
	*
	* @param	javascript	The ASCII string of Javascript to execute.
	*
	* @param	frameName	Optional, the name of the frame to execute in; leave this blank to execute in the main frame.
	*/
	void executeJavascript(const std::string& javascript, const std::wstring& frameName = L"");

	/**
	* Executes a string of Javascript in the context of the current page asynchronously.
	*
	* @param	javascript	The wide string of Javascript to execute.
	*
	* @param	frameName	Optional, the name of the frame to execute in; leave this blank to execute in the main frame.
	*/
	void executeJavascript(const std::wstring& javascript, const std::wstring& frameName = L"");

	/**
	* Executes a string of Javascript in the context of the current page asynchronously with a result.
	*
	* @param	javascript	The ASCII string of Javascript to execute.
	*
	* @param	frameName	Optional, the name of the frame to execute in; leave this blank to execute in the main frame.
	*
	* @return	Returns a 'FutureJSValue' which is basically an 'IOU' for the future JSValue result.
	*			You can obtain the actual result via FutureJSValue::get later.
	*/
	Awesomium::FutureJSValue executeJavascriptWithResult(const std::string& javascript, const std::wstring& frameName = L"");

	/**
	* Executes a string of Javascript in the context of the current page asynchronously with a result.
	*
	* @param	javascript	The wide string of Javascript to execute.
	*
	* @param	frameName	Optional, the name of the frame to execute in; leave this blank to execute in the main frame.
	*
	* @return	Returns a 'FutureJSValue' which is basically an 'IOU' for the future JSValue result.
	*			You can obtain the actual result via FutureJSValue::get later.
	*/
	Awesomium::FutureJSValue executeJavascriptWithResult(const std::wstring& javascript, const std::wstring& frameName = L"");

	/**
	* Call a certain function defined in Javascript directly.
	*
	* @param	object	The name of the object that contains the function, pass an empty string if the function
	*					is defined in the global scope.
	*
	* @param	function	The name of the function.
	*
	* @param	args	The arguments to pass to the function.
	*
	* @param	frameName	Optional, the name of the frame to execute in; leave this blank to execute in the main frame.
	*/
	void callJavascriptFunction(const std::wstring& object, const std::wstring& function, const JSArguments& args, const std::wstring& frameName = L"");

	/**
	* Creates a new global Javascript object that will persist throughout the lifetime of this WebView. This object is
	* managed directly by Awesomium and so you can modify its properties and bind callback functions via WebView::setObjectProperty
	* and WebView::setObjectCallback, respectively.
	*
	* @param	objectName	The name of the object.
	*/
	void createObject(const std::wstring& objectName);

	/**
	* Destroys a Javascript object previously created by WebView::createObject.
	*
	* @param	objectName	The name of the object to destroy.
	*/
	void destroyObject(const std::wstring& objectName);

	/**
	* Sets a property of a Javascript object previously created by WebView::createObject.
	*
	* @param	objectName	The name of the Javascript object.
	*
	* @param	propName	The name of the property.
	*
	* @param	value	The javascript-value of the property.
	*/
	void setObjectProperty(const std::wstring& objectName, const std::wstring& propName, const JSValue& value);

	/**
	* Sets a callback function of a Javascript object previously created by WebView::createObject. This is very useful
	* for passing events from Javascript to C++. To receive notification of the callback, a WebViewListener should 
	* be registered (see WebView::setListener and WebViewListener::onCallback).
	*
	* @param	objectName	The name of the Javascript object.
	*
	* @param	callbackName	The name of the callback function.
	*/
	void setObjectCallback(const std::wstring& objectName, const std::wstring& callbackName);

	/**
	* Returns whether or not the WebView is dirty and needs to be re-rendered via WebView::render.
	*
	* @return	If the WebView is dirty, returns true, otherwise returns false.
	*/
	bool isDirty();
	
	/**
	* Renders the WebView to an off-screen buffer.
	*
	* @param	destination	The buffer to render to, its width and height should match the WebView's.
	*
	* @param	destRowSpan	The row-span of the destination buffer (number of bytes per row).
	*
	* @param	destDepth	The depth (bytes per pixel) of the destination buffer. Valid options
	*						include 3 (BGR/RGB) or 4 (BGRA/RGBA).
	*
	* @param	renderedRect	Optional (pass 0 to ignore); if asynchronous rendering is not enabled,
	*							you can provide a pointer to a Rect to store the dimensions of the 
	*							rendered area, or rather, the dimensions of the area that actually
	*							changed since the last render.
	*/
	void render(unsigned char* destination, int destRowSpan, int destDepth, Awesomium::Rect* renderedRect = 0);

	/**
	* Injects a mouse-move event in local coordinates.
	*
	* @param	x	The absolute x-coordinate of the mouse (localized to the WebView).
	* @param	y	The absolute y-coordinate of the mouse (localized to the WebView).
	*/
	void injectMouseMove(int x, int y);

	/**
	* Injects a mouse-down event.
	*
	* @param	button	The button that was pressed.
	*/
	void injectMouseDown(Awesomium::MouseButton button);

	/**
	* Injects a mouse-up event.
	*
	* @param	button	The button that was released.
	*/
	void injectMouseUp(Awesomium::MouseButton button);

	/**
	* Injects a mouse-wheel event.
	*
	* @param	scrollAmount	The relative amount of pixels to scroll by.
	*/
	void injectMouseWheel(int scrollAmount);

	/**
	* Injects a keyboard event. See WebKeyboardEvent.h for more information.
	*
	* @param	keyboardEvent	The keyboard event to inject.
	*/
	void injectKeyboardEvent(const WebKeyboardEvent& keyboardEvent);

	/**
	* Invokes a 'cut' action using the system clipboard.
	*/
	void cut();

	/**
	* Invokes a 'copy' action using the system clipboard.
	*/
	void copy();

	/**
	* Invokes a 'paste' action using the system clipboard.
	*/
	void paste();

	/**
	* Selects all items on the current page.
	*/
	void selectAll();

	/**
	* De-selects all items on the current page.
	*/
	void deselectAll();

	/**
	* Retrieves the content of the current page as plain text.
	*
	* @param	result	The wide string to store the retrieved text in.
	*
	* @param	maxChars	The maximum number of characters to retrieve.
	*
	* @note	Warning: The performance of this function depends upon the number of retrieved characters
	*		and the complexity of the page.
	*/
	void getContentAsText(std::wstring& result, int maxChars);

	/**
	* Zooms into the page, enlarging by 20%.
	*/
	void zoomIn();

	/**
	* Zooms out of the page, reducing by 20%.
	*/
	void zoomOut();

	/**
	* Resets the zoom level.
	*/
	void resetZoom();

	/**
	* Resizes this WebView to certain dimensions.
	*
	* @param	width	The width to resize to.
	* @param	height	The height to resize to.
	*/
	void resize(int width, int height);

	/**
	* Notifies the current page that it has lost focus.
	*/
	void unfocus();

	/**
	* Notifies the current page that is has gained focus.
	*/
	void focus();

	/**
	* Sets whether or not pages should be rendered with a transparent background-color.
	*
	* @param	isTransparent	Whether or not to force the background-color as transparent.
	*/
	void setTransparent(bool isTransparent);

	/**
	* Sets the current URL Filtering Mode (default is UFM_NONE).
	* See URLFilteringMode for more information on the modes.
	*
	* @param	mode	The URL filtering mode to use.
	*/
	void setURLFilteringMode(URLFilteringMode mode);

	/**
	* Adds a new URL Filter rule.
	*
	* @param	filter	A string with optional wildcards that describes a certain URL.
	*
	* @note		For example, to match all URLs from the domain "google.com", your filter string 
	*			might be: http://google.com/*
	*
	* @note		You may also use the "local://" scheme prefix to describe the URL to the base
	*			directory (set via WebCore::setBaseDirectory).
	*/
	void addURLFilter(const std::wstring& filter);

	/**
	* Clears all URL Filter rules.
	*/
	void clearAllURLFilters();
	
	/**
	* Defines a new Header Definition or updates it if it already exists.
	*
	* @param	name	The unique name of the Header Definition; this is used to refer to it later 
	*					in WebView::addHeaderRewriteRule and WebView::removeHeaderRewriteRulesByDefinitionName.
	*
	* @param	definition	The header definition, a map of key/values representing an HTTP header.
	*/
	void setHeaderDefinition(const std::string& name, const Awesomium::HeaderDefinition& definition);

	/**
	* Adds a new a header re-write rule. All requests whose URL matches the specified rule will have its 
	* HTTP headers re-written with the specified header definition before sending it to the server.
	*
	* @param	rule	A wide string with optional wildcards (*, ?) that matches the URL(s) that will
	*					have its headers re-written with the specified header definition.
	*
	* @param	name	The name of the header definition (specified in WebView::setHeaderDefinition).
	*
	* @note		The case where a URL is matched by multiple rules is unsupported, only the first match will be used.
	*/
	void addHeaderRewriteRule(const std::wstring& rule, const std::string& name);

	/**
	* Removes a header re-write rule from this WebView.
	*
	* @param	rule	The rule to remove (should match the string specified in WebView::addHeaderRewriteRule exactly).
	*/
	void removeHeaderRewriteRule(const std::wstring& rule);

	/**
	* Removes all header re-write rules that are using a certain header definition.
	*
	* @param	name	The name of the header definition (specified in WebView::setHeaderDefinition). If you
	*					specify an empty string, this will remove ALL header re-write rules.
	*/
	void removeHeaderRewriteRulesByDefinitionName(const std::string& name);

	/**
	* Sets whether or not external links (links that normally would open in a new window) should
	* be automatically be opened in the same frame. The default behavior is enabled. You can handle
	* external links yourself via WebViewListener::onOpenExternalLink.
	*
	* @param	isEnabled	Whether or not external links will be automatically opened in the calling frame.
	*/
	void setOpensExternalLinksInCallingFrame(bool isEnabled);

protected:
	WebView(int width, int height, bool isTransparent, bool enableAsyncRendering, int maxAsyncRenderPerSec, base::Thread* coreThread);
	~WebView();

	void startup();
	void setDirty(bool val = true);
	void setAsyncDirty(bool val = true);
	void setFinishRender();
	void setFinishShutdown();
	void setFinishGetContentText();
	void setFinishResize();

	void resolveJSValueFuture(int requestID, Awesomium::JSValue* result);
	void handleFutureJSValueCallback(const Awesomium::JSArguments& args);
	void nullifyFutureJSValueCallbacks();
	void handleCheckKeyboardFocus(bool isFocused);
	void openExternalLink(const std::string& url, const std::wstring& source);

	base::Thread* coreThread;
	WebViewProxy* viewProxy;
	WebViewWaitState* waitState;
	WebViewListener* listener;
	LockImpl* dirtinessLock;
	bool dirtiness, isKeyboardFocused;
	LockImpl* jsValueFutureMapLock;
	std::map<int, JSValueFutureImpl*> jsValueFutureMap;

	const bool enableAsyncRendering;

	friend class WebCore;
	friend class ::WebViewProxy;
	friend class ::FutureValueCallback;
	friend class ::CheckKeyboardFocusCallback;
	friend class ::WindowOpenCallback;
};

}

#endif