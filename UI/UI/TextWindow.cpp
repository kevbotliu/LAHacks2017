#include "TextWindow.hpp"
#include <algorithm>
#include <cctype>

sbide::TextWindow::TextWindow(const sf::Font& font, unsigned size, unsigned margin, unsigned scrolling) :
	m_counter(0),
	m_scrollingRate(scrolling),
	m_margin(margin)
{
	m_renderText.setFont(font);
	m_renderText.setCharacterSize(size);
	m_renderText.setPosition(margin, margin);
}

void sbide::TextWindow::directSetText(sf::Color color, sf::Text::Style style, std::string msg)
{
	m_renderText << color << style << msg;
}

void sbide::TextWindow::setText(const std::string& text)
{
	m_rawText.push_back(std::vector<StyleChar>());
	std::vector<StyleChar>* curr = &m_rawText.back();

	for (char c : text)
	{
		if (c == '\n')
		{
			m_rawText.push_back(std::vector<StyleChar>());
			curr = &m_rawText.back();
		}
		else
		{
			StyleChar schar;
			schar.m_char = c;
			schar.m_style = sf::Text::Style::Regular;
			schar.m_color = m_font;

			curr->push_back(schar);
		}
	}
}

unsigned sbide::TextWindow::highlightChar(unsigned row, unsigned col, sf::Color color, sf::Text::Style style)
{
	row--;
	col--;

	m_rawText[row][col].m_color = color;
	m_rawText[row][col].m_style = style;
	m_rawText[row][col].m_count = m_counter;
	m_rawText[row][col].m_code = CHAR;

	m_counter++;
	return m_counter - 1;
}

bool isCharOrNum(char c)
{
	return isalpha(c) || isdigit(c);
}

unsigned sbide::TextWindow::highlightWord(unsigned row, unsigned col, sf::Color color, sf::Text::Style style)
{
	row--;
	col--;

	int COL = col;

	// find the beginning:
	// All the formating information is stored in the first char only
	for (COL--; COL >= 0 && isCharOrNum(m_rawText[row][COL].m_char); COL--);
	COL++;
	m_rawText[row][COL].m_code = BEGWORD;
	m_rawText[row][COL].m_color = color;
	m_rawText[row][COL].m_style = style;
	m_rawText[row][COL].m_count = m_counter;
	m_counter++;

	// find the end:
	for (; COL < m_rawText[row].size() && isCharOrNum(m_rawText[row][COL].m_char); COL++);
	COL--;
	m_rawText[row][COL].m_code = ENDWORD;
	m_rawText[row][COL].m_color = color;

	return m_counter - 1;
}

void sbide::TextWindow::generateText(bool setSame)
{
	m_renderText.clear();

	for (const auto& arr : m_rawText)
	{
		for (size_t i = 0; i < arr.size(); i++)
		{
			StyleChar schar = arr[i];

			if (schar.m_code == NONE)
				m_renderText << m_font << schar.m_style << schar.m_char;
			else if (schar.m_code == CHAR || schar.m_code == BEGWORD)
			{
				if (setSame)
					m_renderText << schar.m_color << schar.m_style << schar.m_char;
				else
					m_renderText << m_highlightFont << schar.m_style << schar.m_char;
			}

			if (schar.m_code == CHAR)
			{
				HighLight highlight;
				highlight.m_count = schar.m_count;
				
				sf::FloatRect rect = m_renderText.getLines().back().getTexts().back().getGlobalBounds();
				rect = m_renderText.getLines().back().getTransform().transformRect(rect);

				// update the height information of the highlight
				unsigned textHeight = m_renderText.getLines().back().getLocalBounds().height;
				textHeight -= m_renderText.getLines().back().getTexts().back().getCharacterSize() * HIGHIGHT_FRAC;

				// apply the rectangle of the height
				highlight.m_rect.setSize(sf::Vector2f(rect.width, textHeight));
				highlight.m_rect.setOrigin(highlight.m_rect.getOrigin() + sf::Vector2f(0, textHeight - rect.height));
				highlight.m_rect.setPosition(sf::Vector2f(rect.left, rect.top));

				// set the color of the highlight
				sf::Color lightColor = schar.m_color;
				lightColor.a *= NO_CLICK_OPACITY_FRAC;
				highlight.m_rect.setFillColor(lightColor);

				m_highLights.push_back(highlight);
			}
			else if (schar.m_code == BEGWORD)
			{
				HighLight highlight;
				highlight.m_count = schar.m_count;

				unsigned textHeight = m_renderText.getLines().back().getLocalBounds().height;
				textHeight -= m_renderText.getLines().back().getTexts().back().getCharacterSize() * HIGHIGHT_FRAC;

				sf::FloatRect begRect = m_renderText.getLines().back().getTexts().back().getGlobalBounds();
				begRect = m_renderText.getLines().back().getTransform().transformRect(begRect);

				if (setSame)
				{
					for (i++; arr[i].m_code != ENDWORD; i++)
						m_renderText << arr[i].m_color << arr[i].m_style << arr[i].m_char;
					m_renderText << arr[i].m_color << arr[i].m_style << arr[i].m_char;
				}
				else
				{
					for (i++; arr[i].m_code != ENDWORD; i++)
						m_renderText << m_highlightFont << arr[i].m_style << arr[i].m_char;
					m_renderText << m_highlightFont << arr[i].m_style << arr[i].m_char;
				}

				sf::FloatRect endRect = m_renderText.getLines().back().getTexts().back().getGlobalBounds();
				endRect = m_renderText.getLines().back().getTransform().transformRect(endRect);

				// combine the end and begining rectangles:
				sf::FloatRect largeRect;
				largeRect = begRect;
				largeRect.width = (endRect.left + endRect.width) - begRect.left;
				
				highlight.m_rect.setSize(sf::Vector2f(largeRect.width, textHeight));
				highlight.m_rect.setOrigin(highlight.m_rect.getOrigin() + sf::Vector2f(0, textHeight - largeRect.height));
				highlight.m_rect.setPosition(sf::Vector2f(largeRect.left, largeRect.top));

				// set the color of the highlight
				sf::Color lightColor = schar.m_color;
				lightColor.a *= NO_CLICK_OPACITY_FRAC;
				highlight.m_rect.setFillColor(lightColor);

				m_highLights.push_back(highlight);
			}
		}

		m_renderText << "\n";
	}

	generateRowHighlights();
}

void sbide::TextWindow::setAtRow(unsigned row)
{
	int height = m_renderText.getLines().back().getGlobalBounds().height;
	int top = m_renderText.getGlobalBounds().top + height * (row - 1) - m_margin;

	m_renderText.move(sf::Vector2f(0, -top));

	// move each of the row hightlights
	for (auto& it : m_rowHighlights)
	{
		it.move(sf::Vector2f(0, -top));
	}
}

void sbide::TextWindow::generateRowHighlights()
{
	unsigned maxWidth = sf::VideoMode::getFullscreenModes().front().width;
	unsigned height = m_renderText.getLines().back().getGlobalBounds().height;

	unsigned numRows = m_renderText.getLines().size();
	sf::Vector2f pos = m_renderText.getPosition();

	for (size_t i = 0; i < numRows; i++)
	{
		sf::RectangleShape shape;
		shape.setPosition(pos);
		shape.setSize(sf::Vector2f(maxWidth, height));
		shape.setFillColor(m_background);

		m_rowHighlights.push_back(shape);

		pos += sf::Vector2f(0, height);
	}
}

void sbide::TextWindow::draw(sf::RenderWindow& m_window)
{
	for (auto& it : m_rowHighlights)
	{
		m_window.draw(it);// , m_renderText.getTransform());
	}

	for (auto& it : m_highLights)
	{
		m_window.draw(it.m_rect, m_renderText.getTransform());
	}

	m_window.draw(m_renderText);
}

bool isEmptyLine(sfe::RichText::Line& l)
{
	auto& texts = l.getTexts();
	for (const sf::Text& t : texts)
	{
		std::string s = t.getString();
		for (char c : s)
		{
			if (isCharOrNum(c))
				return false;
		}
	}

	return true;
}

void sbide::TextWindow::updateScroll(int updates)
{
	int displacement = updates * m_scrollingRate;

	std::vector<sf::Vector2f> oldPositions;
	sf::Vector2f oldPos = m_renderText.getPosition();
	m_renderText.move(sf::Vector2f(0, displacement));

	for (auto& it : m_rowHighlights)
	{
		oldPositions.push_back(it.getPosition());
		it.move(sf::Vector2f(0, displacement));
	}

	int empty = 0;
	auto lines = m_renderText.getLines();
	for (int i = lines.size() - 1; i >= 0; i--)
	{
		if (isEmptyLine(lines[i]))
			empty++;
		else
			break;
	}

	int bottom = m_renderText.getGlobalBounds().top + m_renderText.getLocalBounds().height -
		(empty + 1) * m_renderText.getLines().back().getGlobalBounds().height;

	if (m_renderText.getGlobalBounds().top > m_margin)
	{
		m_renderText.setPosition(oldPos); // resest the location
		for (size_t i = 0; i < m_rowHighlights.size(); i++)
		{
			m_rowHighlights[i].setPosition(oldPositions[i]);
		}
	}
	if (bottom < m_margin)
	{
		m_renderText.setPosition(oldPos);
		for (size_t i = 0; i < m_rowHighlights.size(); i++)
		{
			m_rowHighlights[i].setPosition(oldPositions[i]);
		}
	}
}

int sbide::TextWindow::updateHighlight(sf::RenderWindow& m_window, bool* left, bool line, bool highlights, bool checkClick)
{
	sf::Vector2i pos = sf::Mouse::getPosition(m_window);

	if (line)
	{
		for (auto& it : m_rowHighlights)
		{
			sf::FloatRect tempRect = it.getGlobalBounds();

			if (tempRect.contains(sf::Vector2f(pos)))
			{
				if (m_window.hasFocus())
				{
					it.setFillColor(m_highlight);

					if (checkClick)
						if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
						{
							for (auto& jt : m_highLights)
							{
								sf::FloatRect tempRect1 = jt.m_rect.getGlobalBounds();
								tempRect1 = m_renderText.getTransform().transformRect(tempRect1);

								if (tempRect1.intersects(tempRect))
									return jt.m_count;
							}
						}
				}
			}
			else
			{
				it.setFillColor(m_background);
			}
		}
	}

	for (auto& it : m_highLights)
	{
		sf::FloatRect tempRect = it.m_rect.getGlobalBounds();
		tempRect = m_renderText.getTransform().transformRect(tempRect);

		if (tempRect.contains(sf::Vector2f(pos)))
		{
			if (m_window.hasFocus())
			{
				sf::Color color = it.m_rect.getFillColor();
				color.a = 255;

				it.m_rect.setFillColor(color);

				if (left != nullptr)
				{
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						*left = true;
						return it.m_count;
					}
					if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
					{
						*left = false;
						return it.m_count;
					}
				}
				else
				{
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						return it.m_count;
					}
				}
			}
		}
		else
		{
			sf::Color color = it.m_rect.getFillColor();

			if (highlights)
				color.a = 255 * NO_CLICK_OPACITY_FRAC;
			else
				color.a = 255;

			it.m_rect.setFillColor(color);
		}
	}

	return -1;
}
