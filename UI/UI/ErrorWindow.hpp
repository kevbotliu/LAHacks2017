#pragma once

#include <vector>
#include <SFML/Window.hpp>
#include "TextWindow.hpp"
#include "CodeWindow.hpp"
#include "LoadResources.hpp"

namespace sbide
{
	void addErrorWindowLoader(ResourceLoader& loader);

	class ErrorWindow
	{
	public:
		ErrorWindow();

		void updateText();
		void draw(std::vector<CodeWindow>& codeWindows);
		std::vector<sbide::CodeWindow> generateErrorWindows();
		void addError(std::string msg, std::string type, std::string filename, std::string* helpful, unsigned row, unsigned col, bool singleChar);
		void bringFront()
		{
			m_window.requestFocus();
		}
		void bringFrontError(unsigned row)
		{
			m_textWindow.setAtRow(m_errors[row].m_row);
		}

	private:
		struct Error
		{
			unsigned m_col;
			unsigned m_row;
			std::string m_filename;
			std::string m_message;
			std::string m_type;
			bool m_singleChar;
			std::string* m_helpful;
		};

		sf::RenderWindow m_window;
		TextWindow m_textWindow;
		std::string m_rawText;
		std::vector<Error> m_errors;
	};
}
