/** Trying to extract weblistener into seperate class **/
#include "MyWebViewListener.h"
#include "WebCore.h"

	MyWebViewListener::MyWebViewListener()
	{
	}
	
	void onBeginNavigation(Awesomium::WebView* caller, const std::string& url, const std::wstring& frameName)
	{		
	}
	
	void onBeginLoading(Awesomium::WebView* caller, const std::string& url, const std::wstring& frameName, int statusCode, const std::wstring& mimeType)
	{	
	}
	
	void onFinishLoading(Awesomium::WebView* caller)
	{		
	}
	
	void onCallback(Awesomium::WebView* caller, const std::wstring& objectName, const std::wstring& callbackName, const Awesomium::JSArguments& args)
	{		
	}
	
	void onReceiveTitle(Awesomium::WebView* caller, const std::wstring& title, const std::wstring& frameName)
	{	
	}
	
	void onChangeTooltip(Awesomium::WebView* caller, const std::wstring& tooltip)
	{	
	}
	
#if defined(_WIN32)
	void onChangeCursor(Awesomium::WebView* caller, const HCURSOR& cursor)
	{	
	}
#endif
	
	void onChangeKeyboardFocus(Awesomium::WebView* caller, bool isFocused)
	{	
	}
	
	void onChangeTargetURL(Awesomium::WebView* caller, const std::string& url)
	{	
	}

	void onOpenExternalLink(Awesomium::WebView* caller, const std::string& url, const std::wstring& source)
	{	
	}
