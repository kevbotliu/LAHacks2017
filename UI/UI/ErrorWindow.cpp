#include "ErrorWindow.hpp"
#include <algorithm>
#include <exception>
#include <list>
using namespace std;

sbide::ResourceLoader* ErrorWindowLoader;

sbide::ErrorWindow::ErrorWindow() :
	m_textWindow(ErrorWindowLoader->createTextWindow()),
	m_window(ErrorWindowLoader->getVideoMode(), "Error Window")
{
	m_textWindow.setColors(sf::Color(43, 48, 59), sf::Color(79, 91, 102), sf::Color(101, 115, 126), sf::Color(223, 225, 232));
    m_window.setFramerateLimit(60);
}

void sbide::ErrorWindow::addError(std::string msg, std::string type, std::string filename, std::string* helpful, unsigned row, unsigned col, bool singleChar)
{
	Error error;
	error.m_col = col;
	error.m_row = row;
	error.m_filename = filename;
	error.m_message = msg;
	error.m_type = type;
	error.m_singleChar = singleChar;
	error.m_helpful = helpful;

	string errorMessage = "ee  " + msg + "\n";
	m_rawText += errorMessage;

	m_errors.push_back(error);
}

std::vector<sbide::CodeWindow> sbide::ErrorWindow::generateErrorWindows()
{
	std::vector<CodeWindow> errors;
	int counter = 0;
	for (Error& err : m_errors)
	{
		auto it = find_if(errors.begin(), errors.end(), [&](const CodeWindow& file) { return file.getFileName() == err.m_filename; });
		if (it == errors.end())
		{
			CodeWindow newWindow(err.m_filename);
			newWindow.addErrorHighlight(counter, err.m_row, err.m_col, err.m_helpful, err.m_type, err.m_singleChar);
			newWindow.setErrorWindow(this);
			errors.push_back(newWindow);
		}
		else
		{
			it->addErrorHighlight(counter, err.m_row, err.m_col, err.m_helpful, err.m_type, err.m_singleChar);
		}

		counter++;
	}

	return errors;
}

void sbide::ErrorWindow::draw(vector<CodeWindow>& codeWindows)
{
	std::list<CodeWindow*> openWindows;

	while (m_window.isOpen())
	{
		sf::View renderView = m_window.getDefaultView();
		sf::Event event;
		int totalDelta = 0;
		while (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_window.close();

			if (event.type == sf::Event::MouseWheelMoved)
				totalDelta += event.mouseWheel.delta;

			if (event.type == sf::Event::Resized)
				m_window.setView(renderView = sf::View(sf::FloatRect(0.f, 0.f,
					static_cast<float>(m_window.getSize().x),
					static_cast<float>(m_window.getSize().y))));
		}

		m_window.clear(sf::Color(43, 48, 59));
		m_textWindow.updateScroll(totalDelta);
		int count = m_textWindow.updateHighlight(m_window, nullptr, true, false, true);
		m_textWindow.draw(m_window);

		m_window.display();

		// get the error from which it is called:
		if (count >= 0)
		{
			auto& it = m_errors[count];
			auto winInMast = find_if(codeWindows.begin(), codeWindows.end(), [&](const CodeWindow& file) { return file.getFileName() == it.m_filename; });
			auto winInOpen = find_if(openWindows.begin(), openWindows.end(), [&](const CodeWindow* file) { return file->getFileName() == it.m_filename; });

			if (winInMast == codeWindows.end())
				throw std::logic_error("ERROR_WINDOW_FIND_ERROR::Can't find the error location");

			if (winInOpen == openWindows.end()) // does not exist yet, then open it
			{
				winInMast->openWindow();
				winInMast->updateText();
				openWindows.push_back(&(*winInMast));
			}

			winInMast->goToRow(it.m_row);
			winInMast->bringFront();
		}

		// draw the windows:
		for (auto win : openWindows)
		{
			win->draw();
		}

		// go through and check if we need to remove anything
		for (auto it = openWindows.begin(); it != openWindows.end();)
		{
			if (!(*it)->windowIsOpen())
			{
				it = openWindows.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
}

void sbide::ErrorWindow::updateText()
{
	m_textWindow.setText(m_rawText);
	for (size_t i = 0; i < m_errors.size(); i++)
	{
		sf::Color color;
		if (m_errors[i].m_type == "warning")
			color = sf::Color(235, 203, 139);
		else if (m_errors[i].m_type == "error")
			color = sf::Color(191, 97, 106);
		else if (m_errors[i].m_type == "fatal error")
			color = sf::Color(180, 142, 173);
		else
			throw logic_error("ERROR_HIGHLIGHTING::Unrecognized error type found");

		m_textWindow.highlightWord(i + 1, 1, color, sf::Text::Regular); // should be position of text
	}
	m_textWindow.generateText(true);
}

void sbide::addErrorWindowLoader(ResourceLoader& loader)
{
	ErrorWindowLoader = &loader;
}

