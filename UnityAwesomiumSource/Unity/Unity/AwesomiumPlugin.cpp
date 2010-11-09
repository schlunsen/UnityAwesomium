/**
 * This is a simple "Hello World!" example of using Awesomium.
 *
 * It loads a page, renders it once, and saves it to a file.
 *
 * Procedure:
 * -- Create the WebCore singleton
 * -- Create a new WebView and request for it to load a URL.
 * -- Register an event listener to handle notifications from the WebView.
 * -- Continuously loop while calling WebCore::update.
 * -- Upon MyWebViewListener::onFinishLoading:
 * -- -- Render the page to a buffer.
 * -- -- Save the buffer to 'result.tga'.
 * -- -- Change the 'isRunning' flag to 'false' so that the main loop ends.
 * -- Clean up.
 */

#include "global.h"
#include "WebCore.h"
#include "MyWebViewListener.h"

#if defined(__WIN32__) || defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include <unistd.h>
#endif

//Delegates
//type_def void (*SetPixelsFunc);
//
//SetPixelsFunc m_setPixelsFunc;

Awesomium::WebCore* webCore;
static Awesomium::WebView* webView = 0;




/**
* Convert unsigned char buffer to float buffer
**/
void convertBuffer(unsigned char* charBuf,float* floatBuf){
for(int y = 0; y < texHeight; ++y)
{
	for(int x = 0; x < texWidth * 4; ++x)
    {
        //copy a pixel from a row y from the top of inData to a row y from the bottom of outData
        //x is the xth byte of the row, not the xth pixel.
		m_buffer[y * texWidth * 4 + x] = charBuf[(texHeight - 1 - y) * texWidth * 4 + x] / 255.0f;
    }
}
}


/**
* Quick method for reducing amount of render calls in unity
* Flag for wheater or not rendering should be processed in unity.
* Needs to be replaced with delegates functioning as callbacks to the C# unity code
**/
bool dirtyBuffer = false;
extern "C" __declspec(dllexport) bool isDirtyBuffer(){

	if (dirtyBuffer == true){
		dirtyBuffer = false;
		return true;
	}
	return dirtyBuffer;
}


extern "C" __declspec(dllexport) void init(float* buffer, int width, int height){

    m_buffer = buffer;	
	texWidth = width;
	texHeight = height;
	
	myfile << "Unity texture width: " << texWidth << "\n";
	myfile << "Unity texture height: " << texHeight << "\n";
	myfile << "m_buffer size (sizeof): " << sizeof(m_buffer);

	webCore = new Awesomium::WebCore(
                                 L"",
                                 L"",
                                 L"",
                                 L"",
                                 Awesomium::LOG_VERBOSE,
                                 true,
								 Awesomium::PF_RGBA,
                                 "");
	
	webView = webCore->createWebView(texWidth, texHeight,true);
	MyWebViewListener *myListener = new MyWebViewListener();	
	webView->setListener(myListener);	
	
	gotoURL(URL);	
}
PLUGIN_API void gotoURL(char* url){
	webView->loadURL(url);
}

PLUGIN_API void update(){	
	
	webCore->update();	

	if (webView->isDirty()){			
		// Create buffer for drawing
		unsigned char* buffer = new unsigned char[texWidth * texHeight* 4];		
		// Render webview to buffer				
		webView->render(buffer, texWidth * 4, 4);
		// Convert and copy rendered Awesomium pixel buffer to our float buffer
		convertBuffer(buffer,m_buffer);
		// Set flag for rerendering 
		dirtyBuffer  = true;
		delete buffer;		
	}
}




/**
* wrap mouse functions function
**/
PLUGIN_API void mouseDown(int mouseButton){
	//webView->focus();
	switch (mouseButton){	
		case 0:
		webView->injectMouseDown(Awesomium::LEFT_MOUSE_BTN);
		break;
		case 1:
		webView->injectMouseDown(Awesomium::RIGHT_MOUSE_BTN);
		break;
	}
	
}

PLUGIN_API void mouseUp(int mouseButton){
	//webView->focus();
	switch (mouseButton){	
		case 0:
		webView->injectMouseUp(Awesomium::LEFT_MOUSE_BTN);		
		break;
		case 1:
		webView->injectMouseUp(Awesomium::RIGHT_MOUSE_BTN);
		break;
	}
	
}


PLUGIN_API void mouseMove(int x,int y){
	//webView->focus();
	webView->injectMouseMove(x,y);		
}

/**
* wrap scrollwheel function
**/
PLUGIN_API void scrollWheel(int amount){	
	webView->injectMouseWheel(amount);	
}

// End mouse functions

/**
* Closing logfilestream
**/
extern "C" __declspec(dllexport) void closeFileStream(){
	myfile.close();	
}
