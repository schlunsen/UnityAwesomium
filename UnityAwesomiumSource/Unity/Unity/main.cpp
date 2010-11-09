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

#if defined(__WIN32__) || defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include <unistd.h>
#endif

#define URL	"http://plikker.com/"
#define SLEEP_MS	250






//Delegates
//type_def void (*SetPixelsFunc);
//
//SetPixelsFunc m_setPixelsFunc;

Awesomium::WebCore* webCore;



void saveImageTGA(const std::string& filename, unsigned char* buffer, int width, int height);

class MyWebViewListener : public Awesomium::WebViewListener
{
public:
	
	MyWebViewListener(float* buf)
	{
	}
	
	void onBeginNavigation(Awesomium::WebView* caller, const std::string& url, const std::wstring& frameName)
	{
		myfile << "ONBEginNavi\n ";
		std::cout << "Navigating to URL: " << url << std::endl;
	}
	
	void onBeginLoading(Awesomium::WebView* caller, const std::string& url, const std::wstring& frameName, int statusCode, const std::wstring& mimeType)
	{
		myfile << "ONBEginLoading\n ";		
	}
	
	void onFinishLoading(Awesomium::WebView* caller)
	{		
		myfile << "OnFinished \n ";		
		unsigned char* buffer = new unsigned char[texWidth * texHeight* 4];		
				
		webView->render(buffer, texWidth * 4, 4);
		
		//saveImageTGA(".\\output\\result.tga", buffer, texWidth, texHeight);

		//convertBuffer(buffer,m_buffer);
		
		myfile << sizeof(m_buffer) << "\n";
			
			
		
		delete buffer;

		myfile << "Size of buffer after delete: " << sizeof(buffer);
		
		std::cout << "Saved a render of the page to 'result.tga'." << std::endl;		
		//system("pause");
		
	
	}
	
	void onCallback(Awesomium::WebView* caller, const std::wstring& objectName, const std::wstring& callbackName, const Awesomium::JSArguments& args)
	{

		myfile << "ONCallBack\n ";
	}
	
	void onReceiveTitle(Awesomium::WebView* caller, const std::wstring& title, const std::wstring& frameName)
	{
		myfile << "ONRecieveTitile\n ";
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



// Templated helper utility to write binary to stream
template<class T>
void writeVal(std::ofstream& destination, T value)
{
	destination.write(reinterpret_cast<char*>(&value), sizeof(T));
}

// Utility function that saves a TGA Image from a BGRA buffer
void saveImageTGA(const std::string& filename, unsigned char* buffer, int width, int height)
{
	
	std::ofstream out(filename.c_str(), std::ios_base::binary);
	
	if(out.bad())
	{
		std::cerr << "Could not save image." << std::endl;
		out.close();
		return;
	}
	
	// Write TGA Header
	writeVal<char>(out, 0);
	writeVal<char>(out, 0);
	writeVal<char>(out, 2);         // compressed RGBA
	writeVal<short>(out, 0);
	writeVal<short>(out, 0);
	writeVal<char>(out, 0);
	writeVal<short>(out, 0);        // x origin
	writeVal<short>(out, 0);        // y origin
	writeVal<short>(out, width);    // width
	writeVal<short>(out, height);   // height
	writeVal<char>(out, 32);        // 32 BPP
	writeVal<char>(out, 0);
	
	int rowSpan = width * 4;
	
	// Write Image Data
	for(int row = height - 1; row >= 0; row--){
		//myfile.write( reinterpret_cast<char*>((buffer + row * rowSpan)), rowSpan);
		out.write(reinterpret_cast<char*>(buffer + row * rowSpan), rowSpan);
	}
	
	out.close();
}

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

bool dirtyBuffer = false;
extern "C" __declspec(dllexport) bool isDirtyBuffer(){

	if (dirtyBuffer == true){
		dirtyBuffer = false;
		return true;
	}
	return dirtyBuffer;



}

extern "C" __declspec(dllexport) void closeFileStream(){
	myfile.close();	
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
	
	webView = webCore->createWebView(texWidth, texHeight);
	MyWebViewListener *myListener = new MyWebViewListener(m_buffer);	
	webView->setListener(myListener);	
	
	gotoURL(URL);
	//webView->loadURL(URL);	
}

PLUGIN_API void update(){		
	
	webCore->update();
	

	if (webView->isDirty()){		
		

		myfile << "update\n";
		
		unsigned char* buffer = new unsigned char[texWidth * texHeight* 4];		
				
		webView->render(buffer, texWidth * 4, 4);

		convertBuffer(buffer,m_buffer);
		dirtyBuffer  = true;

		delete buffer;

		// Call setpixels and apply functions

	}

}


PLUGIN_API void gotoURL(char* url){
	std::ofstream testFile("NavigationLog.log",std::ios_base::app);
	testFile << url << "\n";
	testFile.close();
	webView->loadURL(url);

}

PLUGIN_API void scrollWheel(int amount){
	webView->focus();
	webView->injectMouseWheel(amount);
	myfile << "scrolling: " << amount << " \n";
}

