#include "ExplainWindow.hpp"
#include "CodeWindow.hpp"
using namespace std;

sbide::ResourceLoader* ExplainWindowLoader;

sbide::ExplainWindow::ExplainWindow() :
	m_textWindow(ExplainWindowLoader->createTextWindow()),
	m_window(nullptr),
	m_codeWindow(nullptr)
{
	m_textWindow.setColors(sf::Color(239, 241, 245), sf::Color(223, 225, 232), sf::Color(79, 91, 102), sf::Color(79, 91, 102));
}

void sbide::ExplainWindow::createWindow(unsigned code, unsigned row, std::string msg)
{
	m_window = new sf::RenderWindow(ExplainWindowLoader->getVideoMode(), "Insight");
    m_window->setFramerateLimit(60);
	string message = "Return\n" + msg;

	m_textWindow.setText(message);
	m_textWindow.highlightWord(1, 1, sf::Color(150, 181, 180), sf::Text::Style::Regular);
	m_textWindow.generateText();

	m_rows = row;
	m_code = code;
}

void sbide::ExplainWindow::draw()
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
	int count = m_textWindow.updateHighlight(*m_window, &left, false);

	if (count >= 0)
	{
		if (left)
		{
			m_codeWindow->goToRow(m_rows);
			m_codeWindow->bringFront();
		}
	}

	m_textWindow.draw(*m_window);
	m_window->display();
}

void sbide::addExplainWindowLoader(ResourceLoader& loader)
{
	ExplainWindowLoader = &loader;
}
