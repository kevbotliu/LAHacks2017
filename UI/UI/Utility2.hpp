 #pragma once

#include <dirent.h>
#include <sys/stat.h>
#include <map>
#include <fstream>
#include <string>

namespace sbide
{
	bool readFile(const std::string& path, std::string& fileContent)
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
	
	bool loadFiles(std::string dir, std::map<std::string, std::vector<std::string> >& fileToContent)
	{
		
		std::ifstream fin;
		std::string filepath;
		std::string fileContent;
		DIR *dp;
		struct dirent *dirp;
		struct stat filestat;
		
		dp = opendir(dir.c_str());
		if (dp == NULL)
		{
			std::cout << "Error(" << errno << ") opening " << dir << std::endl;
			int window; std::cin >> window;
			return false;
		}
		std::string check1 = "cpp";
		std::string check2 = "hpp";
		std::string check3 = "h";
		while ((dirp = readdir(dp)))
		{
			filepath = dir + "/" + dirp->d_name;
			
			// If the file is a directory (or is in some way invalid) we'll skip it
			if (stat(filepath.c_str(), &filestat)) continue;
			if (S_ISDIR(filestat.st_mode))         continue;
			
			// Endeavor to read a single number from the file and display its
			
			bool shouldReadFromFile = false;
			for (int i = 0; i < filepath.size(); i++)
			{
				if (filepath[i] == '.')
				{
					std::string check;
					
					for (int j = i + 1; j < filepath.size(); j++)
					{
						check += filepath[j];
					}
					if (check == check1 || check == check2  || check == check3)
					{
						shouldReadFromFile = true;
						break;
					}
				}
			}
			
			if (shouldReadFromFile)
			{
				std::string line;
				fin.open(filepath.c_str());
				std::string tempname = filepath;
				filepath = "";
				for (int i = tempname.size() - 1.0; i >= 0; i--)
				{
					if (tempname[i] == '/')
					{
						for (int j = i + 1; j < tempname.size(); j++)
							filepath += tempname[j];
						break;
					}
				}
				std::vector<std::string> specificFileVector;
				while (getline(fin, line))
				{
					fileToContent[filepath].push_back(line);
					
				}
				
				fin.close();
			}
		}
		
		closedir(dp);
		
		/*
		 for (std::map<std::string, std::vector<std::string>>::iterator p = fileToContent.begin(); p != fileToContent.end(); p++)
		 {
		 std::cout << p->first << std::endl;
		 for (std::vector<std::string>::iterator k = p->second.begin(); k != p->second.end(); k++)
		 {
		 
			std::cout << *k << std::endl;
		 }
		 }
		 int a; std::cin >> a;
		 */
		
		
		return true;
	}
	
}

