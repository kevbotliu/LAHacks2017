#pragma once

#include <fstream>
#include <string>

namespace sbide
{
	inline bool readFile(const std::string& path, std::string& fileContent)
	{
		std::ifstream ifs(path);
		if (ifs.is_open())
		{
			std::string content((std::istreambuf_iterator<char>(ifs)),
				(std::istreambuf_iterator<char>()));
			fileContent = std::move(content);
			return true;
		}
		return false;
	}
}
