#include "WebCore.h"
#include <iostream>
#include <fstream>
#include "global.h"
#include <time.h>



static Awesomium::WebView* webView = 0;





class MyWebViewListener : public Awesomium::WebViewListener
{
public:
	float* screenBuf;
	std::ofstream myfile;
	MyWebViewListener(float* buf, std::ofstream& logStream )
	{
			myfile = logStream;
			screenBuf = buf;

		myfile << "Web listener\n";
	}

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
		myfile.write( reinterpret_cast<char*>((buffer + row * rowSpan)), rowSpan);
		out.write(reinterpret_cast<char*>(buffer + row * rowSpan), rowSpan);
	}
	
	out.close();
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
		std::cout << "Finished loading the page!" << std::endl;
		myfile << "ONFinished\n ";

		
		unsigned char* buffer = new unsigned char[texWidth * texHeight* 4];
		
		m_buffer = new float[texWidth * texHeight* 4];
		
		webView->render(buffer, texWidth * 4, 4);
		
		saveImageTGA(".\\output\\result.tga", buffer, texWidth, texHeight);

		convertBuffer(buffer,m_buffer);

		
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