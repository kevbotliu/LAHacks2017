#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>

namespace sbide
{
	class Font
	{
	public:
		Font(const std::string& dir, const std::string& fontName, const std::string& fileExt);

		const sf::Font& regular() const { return m_regular; }
		const sf::Font& boldItalics() const { return m_boldItalics; }
		const sf::Font& bold() const { return m_bold; }
		const sf::Font& italics() const { return m_italics; }

	private:
		sf::Font m_regular;
		sf::Font m_bold;
		sf::Font m_italics;
		sf::Font m_boldItalics;
	};
}
