#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "RichText.hpp"
#include "Font.hpp"

namespace sbide
{
	const float HIGHIGHT_FRAC = 0.2f;
	const float NO_CLICK_OPACITY_FRAC = 0.5f;

	enum HighlightCode {NONE, BEGWORD, ENDWORD, CHAR, LINE};

	class TextWindow
	{
	public:
		TextWindow(const sf::Font& font, unsigned size, unsigned margin, unsigned scrolling);

		void setColors(sf::Color background, sf::Color highlight, sf::Color highlightFont, sf::Color font)
		{
			m_background = background;
			m_highlight = highlight;
			m_highlightFont = highlightFont;
			m_font = font;
		}

		void directSetText(sf::Color color, sf::Text::Style style, std::string msg);
		void setText(const std::string& text);
		unsigned highlightChar(unsigned row, unsigned col, sf::Color color, sf::Text::Style style); 
		unsigned highlightWord(unsigned row, unsigned col, sf::Color color, sf::Text::Style style);
		void generateText(bool setSame = false);

		void setAtRow(unsigned row);
		void generateRowHighlights();

		void draw(sf::RenderWindow& m_window);
		void updateScroll(int update);
		int updateHighlight(sf::RenderWindow& m_window, bool* left = nullptr, bool lines = true, bool highlights = true, bool checkClick = false);

	private:
		struct StyleChar
		{
			char m_char;
			sf::Color m_color;
			sf::Text::Style m_style;
			unsigned m_count = -1;
			HighlightCode m_code = NONE;
		};

		struct HighLight
		{
			sf::RectangleShape m_rect;
			unsigned m_count;
		};

		sf::Color m_background;
		sf::Color m_highlight;
		sf::Color m_highlightFont;
		sf::Color m_font;
		
		sfe::RichText m_renderText;
		std::vector<HighLight> m_highLights;
		std::vector<std::vector<StyleChar>> m_rawText;
		std::vector<sf::RectangleShape> m_rowHighlights;

		int m_counter;
		unsigned m_margin;
		unsigned m_scrollingRate;
	};
}
