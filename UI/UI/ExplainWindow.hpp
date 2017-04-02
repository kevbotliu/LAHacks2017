#pragma once

#include "TextWindow.hpp"
#include "LoadResources.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace sbide
{
	void addExplainWindowLoader(ResourceLoader& loader);

	class CodeWindow;

	class ExplainWindow
	{
	public:
		ExplainWindow();
		~ExplainWindow()
		{
			delete m_window;
		}

		void setMotherWindow(CodeWindow* win)
		{
			m_codeWindow = win;
		}
		void createWindow(unsigned code, unsigned row, std::string msg);
		void draw();
		void bringFront()
		{
			m_window->requestFocus();
		}
		bool windowIsOpen()
		{
			return m_window->isOpen();
		}
		unsigned getCode() const { return m_code; }
		void close()
		{
			delete m_window;
			m_window = nullptr;
		}

	private:
		unsigned m_rows, m_code;
		TextWindow m_textWindow;
		sf::RenderWindow* m_window;
		CodeWindow* m_codeWindow;
	};
}
