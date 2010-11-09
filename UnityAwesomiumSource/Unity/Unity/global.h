#define PLUGIN_API extern "C" __declspec(dllexport)


#include <iostream>
#include <fstream>
#include <time.h>

#define URL	"http://version2.dk/"

// unity float buffer ~ m_pixels in unity (Pointer to Color[] ??)
float* m_buffer;

//

//Unity prop
int texWidth;
int texHeight;
std::ofstream myfile(".\\output\\testDBG.txt");

//PLUGIN_API void init(float* buffer, int width, int height);


std::string WStringToString(const std::wstring& s)
{
	std::string temp(s.length(), ' ');
	std::copy(s.begin(), s.end(), temp.begin());
	return temp; 
}




PLUGIN_API void gotoURL(char* url);