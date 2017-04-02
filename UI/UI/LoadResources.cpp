#include "LoadResources.hpp"

#include <exception>
#include <sstream>
using namespace std;

sbide::ResourceLoader::ResourceLoader(const std::string & path) :
	m_fontSize(0),
	m_windowHeight(0),
	m_windowWidth(0),
	m_marginSize(0),
	m_sideWindowFraction(-1.f)
{
	if (!m_font.loadFromFile(path + "/font/main_font.ttf"))
	{
		throw runtime_error("FONT_LOADING::Could not load the text font");
	}

	string settings;
	if (!readFile(path + "/settings", settings))
	{
		throw runtime_error("SETTINGS_LOADING::Could not load the settings file");
	}

	// parse the settings file:
	unsigned counter = 0;
	istringstream iss(settings);
	for (string line; getline(iss, line); )
	{
		counter++;
		size_t pos = line.find('=');
		string type = line.substr(0, pos);
		if (type == "fontSize")
		{
			if (m_fontSize != 0)
			{
				throw runtime_error("SETTINGS_LOADING::Settings file redefined fontSize");
			}

			pos++;
			try
			{
				m_fontSize = stoi(line.substr(pos));
			}
			catch (...)
			{
				throw runtime_error("SETTINGS_LOADING::Settings file fontSize value invalid");
			}

			m_fontSize *= (72.f / 96);
		}
		else if (type == "windowWidth")
		{
			if (m_windowWidth != 0)
			{
				throw runtime_error("SETTINGS_LOADING::Settings file redefined windowWidth");
			}

			pos++;
			try
			{
				m_windowWidth = stoi(line.substr(pos));
			}
			catch (...)
			{
				throw runtime_error("SETTINGS_LOADING::Settings file windowWidth value invalid");
			}
		}
		else if (type == "windowHeight")
		{
			if (m_windowHeight != 0)
			{
				throw runtime_error("SETTINGS_LOADING::Settings file redefined windowHeight");
			}

			pos++;
			try
			{
				m_windowHeight = stoi(line.substr(pos));
			}
			catch (...)
			{
				throw runtime_error("SETTINGS_LOADING::Settings file windowHeight value invalid");
			}
		}
		else if (type == "marginSize")
		{
			if (m_marginSize != 0)
			{
				throw runtime_error("SETTINGS_LOADING::Settings file redefined marginSize");
			}

			pos++;
			try
			{
				m_marginSize = stoi(line.substr(pos));
				m_marginSize *= (72.f / 96);
			}
			catch (...)
			{
				throw runtime_error("SETTINGS_LOADING::Settings file marginSize value invalid");
			}
		}
		else if (type == "scrollingRate")
		{
			if (m_scrollingRate != 0)
			{
				throw runtime_error("SETTINGS_LOADING::Settings file redefined scrollingRate");
			}

			pos++;
			try
			{
				m_scrollingRate = stoi(line.substr(pos));
				m_scrollingRate *= (72.f / 96);
			}
			catch (...)
			{
				throw runtime_error("SETTINGS_LOADING::Settings file scrollingRate value invalid");
			}
		}
		else if (type == "sideWindowFraction")
		{
			if (m_scrollingRate < 0)
			{
				throw runtime_error("SETTINGS_LOADING::Settings file redefined sideWindowFraction");
			}

			pos++;
			try
			{
				m_sideWindowFraction = stof(line.substr(pos));
			}
			catch (...)
			{
				throw runtime_error("SETTINGS_LOADING::Settings file sideWindowFraction value invalid");
			}
		}
		else
		{
			throw runtime_error("SETTINGS_LOADING::Settings file unknown setting property");
		}
	}

	if (counter < NUM_PROP)
	{
		throw runtime_error("SETTINGS_LOADING::Missing some properties");
	}
}
