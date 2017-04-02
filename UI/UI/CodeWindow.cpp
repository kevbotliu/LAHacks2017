#include "CodeWindow.hpp"
#include "Utility.hpp"
#include "ErrorWindow.hpp"
#include "ExplainWindow.hpp"
#include <exception>
#include <map>
using namespace std;

sbide::ResourceLoader* CodeWindowLoader;
map<string, vector<string>>* fileToContentRef;

sbide::CodeWindow::CodeWindow(std::string fileName) :
	m_textWindow(CodeWindowLoader->createTextWindow()),
	m_fileName(fileName),
	m_window(nullptr)
{
    string totalCode;
    for (auto& l : fileToContentRef->at(fileName))
    {
        totalCode += l + "\n";
    }
    
	try
	{
		m_textWindow.setText(totalCode);
	}
	catch (...)
	{
		throw logic_error("CODEWINDOW_TEXTLOADING::Could not find the file's content for: " + fileName);
	}

	m_textWindow.setColors(sf::Color(239, 241, 245), sf::Color(223, 225, 232), sf::Color(79, 91, 102), sf::Color(79, 91, 102));

}

void sbide::CodeWindow::addErrorHighlight(unsigned self, unsigned row, unsigned col, std::string* messasge, std::string type, bool singleChar)
{
	sf::Color color;
	if (type == "warning")
		color = sf::Color(235, 203, 139);
	else if (type == "error")
		color = sf::Color(191, 97, 106);
	else if (type == "fatal error")
		color = sf::Color(180, 142, 173);
	else
		throw logic_error("ERROR_HIGHLIGHTING::Unrecognized error type found");

	if (singleChar)
		m_textWindow.highlightChar(row, col, color, sf::Text::Style::Regular);
	else
		m_textWindow.highlightWord(row, col, color, sf::Text::Style::Regular);

	m_helpfulMessages.push_back(make_pair(make_pair(messasge, row), self));
}

void sbide::CodeWindow::openWindow()
{
	m_window = new sf::RenderWindow(CodeWindowLoader->getVideoMode(), m_fileName);
    m_window->setFramerateLimit(60);
    sf::Vector2i oldPos = m_window->getPosition();
    oldPos += sf::Vector2i(35, 35);
    m_window->setPosition(oldPos);
}

bool sbide::CodeWindow::windowIsOpen()
{
	if (!m_window->isOpen())
	{
		for (auto& i : m_openWindows)
		{
			i->close();
			delete i;
		}

		m_openWindows.clear();
	}
	return m_window->isOpen();
}

void sbide::CodeWindow::setErrorWindow(ErrorWindow* errWin)
{
	m_errWin = errWin;
}

void sbide::CodeWindow::draw()
{
	sf::View renderView = m_window->getDefaultView();
	sf::Event event;
	int totalDelta = 0;
	while (m_window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_window->close();

		if (event.type == sf::Event::MouseWheelMoved)
			totalDelta += event.mouseWheel.delta;

		if (event.type == sf::Event::Resized)
			m_window->setView(renderView = sf::View(sf::FloatRect(0.f, 0.f,
				static_cast<float>(m_window->getSize().x),
				static_cast<float>(m_window->getSize().y))));
	}

	m_window->clear(sf::Color(239, 241, 245));
	m_textWindow.updateScroll(totalDelta);
	bool left;
	int count = m_textWindow.updateHighlight(*m_window, &left);
    
    m_textWindow.draw(*m_window);
    
    m_window->display();

	if (count >= 0)
	{
		if (left)
		{
			std::string* ptr = m_helpfulMessages[count].first.first;

			auto res = std::find_if(m_openWindows.begin(), m_openWindows.end(), [&](const ExplainWindow* window)
			{
				return window->getCode() == count;
			});
			
			if (res == m_openWindows.end())
			{
				ExplainWindow* window = new ExplainWindow();
				m_openWindows.push_back(window);
				auto& w = m_openWindows.back();
				w->setMotherWindow(this);
				w->createWindow(count, m_helpfulMessages[count].first.second, *ptr);
				w->bringFront();
			}
			else
			{
				(*res)->bringFront();
			}
		}
		else
		{
			int c = m_helpfulMessages[count].second;
			m_errWin->bringFrontError(m_helpfulMessages[count].second);
			m_errWin->bringFront();
		}
	}

	// draw the windows:
	for (auto& win : m_openWindows)
	{
		win->draw();
	}

	// go through and check if we need to remove anything
	for (auto it = m_openWindows.begin(); it != m_openWindows.end();)
	{
		if (!(*it)->windowIsOpen())
		{
			it = m_openWindows.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void sbide::addCodeWindowLoader(ResourceLoader& loader)
{
	CodeWindowLoader = &loader;
}

void sbide::setCodeWindowContentRef(map<string, vector<string>>& f)
{
	fileToContentRef = &f;
}

