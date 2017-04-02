#include "Font.hpp"
#include <exception>

using namespace std;

sbide::Font::Font(const string& dir, const string& fontName, const std::string& fileExt)
{
	string fileName = fontName + "-";

	if (!m_regular.loadFromFile(dir + "/" + fileName + "Regular" + fileExt))
		throw runtime_error("FONT_LOADING::Could not load regular font");
	if (!m_regular.loadFromFile(dir + "/" + fileName + "Bold" + fileExt))
		throw runtime_error("FONT_LOADING::Could not load bold font");
	if (!m_regular.loadFromFile(dir + "/" + fileName + "Italic" + fileExt))
		throw runtime_error("FONT_LOADING::Could not load italic font");
	if (!m_regular.loadFromFile(dir + "/" + fileName + "BoldItalic" + fileExt))
		throw runtime_error("FONT_LOADING::Could not load bold-italic font");
}
