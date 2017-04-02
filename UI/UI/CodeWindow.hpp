#pragma once

#include <string>
#include <vector>
#include <list>
#include <map>
#include "TextWindow.hpp"
#include "LoadResources.hpp"

namespace sbide
{
	void addCodeWindowLoader(ResourceLoader& loader);
    void setCodeWindowContentRef(std::map<std::string, std::vector<std::string>>& fileToContentRef);

	class ErrorWindow;
	class ExplainWindow;

	class CodeWindow
	{	
	public:
		CodeWindow(std::string fileName);
		~CodeWindow()
		{
			delete m_window;
			for (auto& i : m_openWindows)
			{
				delete i;
			}
			m_openWindows.clear();
		}

		void addErrorHighlight(unsigned self, unsigned row, unsigned col, std::string* messasge, std::string type, bool singleChar);
		void updateText()
		{
			m_textWindow.generateText();
		}
		const std::string& getFileName() const
		{
			return m_fileName;
		}

		void goToRow(unsigned row)
		{
			m_textWindow.setAtRow(row);
		}
		void openWindow();
		bool windowIsOpen();
		void bringFront()
		{
			m_window->requestFocus();
		}

		void setErrorWindow(ErrorWindow* errWin);

		void draw();

	private:
		std::list<ExplainWindow*> m_openWindows;
		std::string m_fileName;
		sf::RenderWindow* m_window;
		TextWindow m_textWindow;
		std::vector<std::pair<std::pair<std::string*, unsigned>, unsigned>> m_helpfulMessages;
		ErrorWindow* m_errWin;
	};
}
