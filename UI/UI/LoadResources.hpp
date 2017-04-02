#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include "Utility.hpp"
#include "TextWindow.hpp"

namespace sbide
{
	class ResourceLoader
	{
	public:
		ResourceLoader(const std::string& path);

		const sf::Font& getFont() { return m_font; }
		TextWindow createTextWindow() { return TextWindow(m_font, m_fontSize, m_marginSize, m_scrollingRate); }
		sf::VideoMode getVideoMode() { return sf::VideoMode(m_windowWidth, m_windowHeight); }
		float getWindowFraction() const { return m_sideWindowFraction; }

	private:
		sf::Font m_font;

		// window properties:
		const unsigned NUM_PROP = 6;

		unsigned m_fontSize;
		unsigned m_marginSize;
		unsigned m_windowWidth;
		unsigned m_windowHeight;
		unsigned m_scrollingRate;
		float m_sideWindowFraction;
	};
}
