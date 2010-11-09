#define PLUGIN_API extern "C" __declspec(dllexport)

#include "WebCore.h"
#include <iostream>
#include <fstream>
#include <time.h>

#define URL	"http://google.dk/"

// unity float buffer ~ m_pixels in unity (Pointer to Color[] ??)
float* m_buffer;

static Awesomium::WebView* webView = 0;

//Unity prop
int texWidth;
int texHeight;
std::ofstream myfile(".\\output\\debugger.log");









