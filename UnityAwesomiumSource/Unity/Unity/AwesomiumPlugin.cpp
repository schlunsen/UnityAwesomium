#include "global.h"

#if defined(__WIN32__) || defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include <unistd.h>
#endif


#define SLEEP_MS	250

static Awesomium::WebCore* webCore;

class AwesomiumWindow //: public Awesomium::WebViewListener
{
public:
	typedef void (*SetPixelsFunc)(/*int left, int top, int width, int height*/);
	typedef void (*ApplyTextureFunc)();
	
	//Delegates - callback functions
	SetPixelsFunc m_setPixelsFunc;
	ApplyTextureFunc m_applyTextureFunc;
	
	bool isEnabled;
	float* pixelBuffer;	
	int m_width;
	int m_height;	
	Awesomium::WebView* m_webView;
	bool m_isDirty;

	
	AwesomiumWindow(int uniqueID, float *buffer, int width, int height, SetPixelsFunc setPixelFunc, ApplyTextureFunc applyTextureFunc, bool transparent = false, bool enableAsyncRendering = false, int maxAsyncRenderPerSec = 70, const std::string &url = "http://google.dk") : m_width(width), m_height(height)
	{
		m_isDirty = true;
		pixelBuffer = buffer;
		isEnabled = true;
		// hookup callbacks 
		setPaintFunctions(setPixelFunc,applyTextureFunc);		
		m_webView = webCore->createWebView(m_width,m_height);				
		//m_webView->setListener(this);
		m_webView->loadURL("http://google.dk");//url);

	}

	AwesomiumWindow::~AwesomiumWindow(){
		//free(pixelBuffer);
		m_webView->destroy();		
		isEnabled = false;
	}

	void update(){
		
		if (isEnabled && m_webView->isDirty()){		
			// Create char pixel buffer 
			unsigned char* buffer = new unsigned char[m_width * m_height * 4];		
			// render to pixel buffer				
			m_webView->render(buffer, m_width * 4, 4);				

			// Convert and copy rendered Awesomium pixel buffer to our float buffer
			this->convertBuffer(buffer);
			
			// set Unity render flag. Temporarely fix
			m_isDirty = true;		

		//	 Do repaint in unity
		/*	if (m_setPixelsFunc)
				m_setPixelsFunc();

			if (m_applyTextureFunc)
				m_applyTextureFunc();	*/	

			delete buffer;		
		}

	}

	// Callbacks
	void setPaintFunctions(SetPixelsFunc setPixelsFunc, ApplyTextureFunc applyTextureFunc){
		m_setPixelsFunc = setPixelsFunc;
		m_applyTextureFunc = applyTextureFunc;
	}

	// Hack instead of callbacks. If dirty then unity should render
	bool isDirty(){
		if (m_isDirty == true)
		{			
			m_isDirty = false;
			return true;
		}
		return false;			
	}

	/**
* Convert unsigned char buffer to float buffer
**/
void convertBuffer(unsigned char* charBuf){	

for(int y = 0; y < m_height; ++y)
{
	for(int x = 0; x < m_width * 4; ++x)
    {	
        //copy a pixel from a row y from the top of inData to a row y from the bottom of outData
        //x is the xth byte of the row, not the xth pixel.
		pixelBuffer[y * m_width * 4 + x] = charBuf[(m_height - 1 - y) * m_width * 4 + x] / 255.0f;	
    }
}
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
};



#include <map>
// A map that holds the created windows
typedef std::map<int, AwesomiumWindow *> WindowMap;
WindowMap awesomiumWindows;

AwesomiumWindow *getWindow(int id)
{		
	WindowMap::const_iterator it = awesomiumWindows.find(id);
	if(it == awesomiumWindows.end())
	{		
		return 0;
	}	

	return it->second;
}


/**
* Quick method for reducing amount of render calls in unity
* Flag for wheater or not rendering should be processed in unity.
* Needs to be replaced with delegates functioning as callbacks to the C# unity code
**/

extern "C" __declspec(dllexport) bool isDirtyBuffer(int uniqueId){
	AwesomiumWindow* pWindow = getWindow(uniqueId);
	if (pWindow)
		return pWindow->isDirty();

	return false;
}


/**
*
**/

extern "C" __declspec(dllexport) void Init(){
	webCore = new Awesomium::WebCore(Awesomium::LOG_VERBOSE,true, Awesomium::PF_RGBA);
    /*webCore = new Awesomium::WebCore(
                                 L"",
                                 L"",
                                 L"",
                                 L"",
								 Awesomium::LOG_VERBOSE,
                                 true,
								 Awesomium::PF_RGBA,
                                 "");		*/
	
}

PLUGIN_API bool CreateAwesomiumWebView(int uniqueID, float *buffer, int width, int height, AwesomiumWindow::SetPixelsFunc setPixelsFunc, AwesomiumWindow::ApplyTextureFunc applyTextureFunc, bool transparent = false, bool enableAsyncRendering = false, int maxAsyncRenderPerSec = 70, const std::string &url = "http://google.dk"){
	
	if(awesomiumWindows.find(uniqueID) != awesomiumWindows.end())
	{		
		return false;
	}	

	AwesomiumWindow *pWindow = new AwesomiumWindow(uniqueID, buffer, width, height, setPixelsFunc, applyTextureFunc, transparent, enableAsyncRendering,maxAsyncRenderPerSec,url);
	awesomiumWindows[uniqueID] = pWindow;		
	return true;
}

PLUGIN_API void DestroyAwesomiumWebView(int uniqueID){
	AwesomiumWindow* pWindow = getWindow(uniqueID);
	if(pWindow)
		delete pWindow;
}

PLUGIN_API bool isDirty(int uniqueID){
	AwesomiumWindow* pWindow = getWindow(uniqueID);
	if(pWindow)
		return pWindow->isDirty();

	return false;
}

PLUGIN_API void Destroy(){	
	for (std::map<int, AwesomiumWindow *>::iterator it = awesomiumWindows.begin(); it != awesomiumWindows.end(); it++) {
		it->second->~AwesomiumWindow();		
	}			
	
	delete webCore;
}


PLUGIN_API void Update(){	
	
	webCore->update();		
	
	//traverse windows
	// update webviews
	for (std::map<int, AwesomiumWindow *>::iterator it = awesomiumWindows.begin(); it != awesomiumWindows.end(); it++) {									
		it->second->update();		
	}	
}

PLUGIN_API void LoadURL(int uniqueId, char* url){
	AwesomiumWindow* pWindow = getWindow(uniqueId);
	if (pWindow)
		pWindow->m_webView->loadURL(url);
	
}

PLUGIN_API void LoadFile(int uniqueId, char* url){	
	
	AwesomiumWindow* pWindow = getWindow(uniqueId);	
	if (pWindow)
	{
			pWindow->m_webView->loadFile(url);
	}
	
}



/**
* Keyboard wrapping
**/
//PLUGIN_API void InjectKeyboard(int uniqueId, int msg, int wParam, long lParam){
//	AwesomiumWindow* pWindow = getWindow(uniqueId);
//	if (pWindow)		
//		pWindow->m_webView->get
//		pWindow->m_webView->injectKeyboardEvent(msg,wParam,lParam);
//	
//}

/**
* wrap mouse functions function
**/
PLUGIN_API void MouseDown(int uniqueId, int mouseButton){
	AwesomiumWindow* pWindow = getWindow(uniqueId);
	if (pWindow){		
		switch (mouseButton){	
			case 0:
				pWindow->m_webView->injectMouseDown(Awesomium::LEFT_MOUSE_BTN);
			break;
			case 1:
			pWindow->m_webView->injectMouseDown(Awesomium::RIGHT_MOUSE_BTN);
			break;
		}
	}
	
}

PLUGIN_API void MouseUp(int uniqueId, int mouseButton){
	AwesomiumWindow* pWindow = getWindow(uniqueId);
	if (pWindow){	
		switch (mouseButton){	
			case 0:
				pWindow->m_webView->injectMouseUp(Awesomium::LEFT_MOUSE_BTN);		
			break;
			case 1:
			pWindow->m_webView->injectMouseUp(Awesomium::RIGHT_MOUSE_BTN);
			break;
		}
	}
	
}


PLUGIN_API void MouseMove(int uniqueId,int x,int y){	
	AwesomiumWindow* pWindow = getWindow(uniqueId);
	if (pWindow){	
		pWindow->m_webView->injectMouseMove(x,y);		
	
	}
}

/**
* wrap scrollwheel function
**/
PLUGIN_API void ScrollWheel(int uniqueId,int amount){	
	AwesomiumWindow* pWindow = getWindow(uniqueId);
	if (pWindow)
		pWindow->m_webView->injectMouseWheel(amount);	
}

// End mouse functions

/**
* Enabled/Disable window
**/
PLUGIN_API void EnabledWindow(int uniqueId,bool isEnabled){	
	AwesomiumWindow* pWindow = getWindow(uniqueId);
	if (pWindow)
		pWindow->isEnabled = isEnabled;	
}


/**
* Closing logfilestream
**/

PLUGIN_API void CloseFileStream(){
//	myfile.close();	
	
}
