#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>
using namespace std;

bool fileLoad(string file)
{
	ifstream instream(file);
	if (!instream)
	{
		cerr << "Error! Can't open the input file.\n";
		return false;
	}

	ofstream outfile;
	string line;
	outfile.open("output.txt");
	while (getline(instream, line))
	{
		bool isImportantLine = false;

		//first, check if the line has a ' . ' or a ' : '
		//if not, ignore the line -- it is not important
		for (int i = 0; i != line.size(); i++)
		{
			if (line[i] == '.')
			{
				string check;
				string test1 = "cpp";
				string test2 = "hpp";

				for (int j = i + 1; j < line.size() && line[j] != ':'; j++)
				{
					check += line[j];
				}
				if (check == test1 || check == test2)
				{
					isImportantLine = true;
					break;
				}
			}
			if (line[i] == ':')
			{
				string check;
				string test1 = "linker";
				for (int j = i + 2; j < line.size() && line[j] != ' '; j++)
				{
					check += line[j];
				}
				if (check == test1)
				{
					isImportantLine = true;
					break;
				}
			}
		}

		if (isImportantLine)
		{
			outfile << line << '\n';
		}
	}
	outfile.close();

	return true;
}

int main()
{
	fileLoad("./TEMP/errCopy.txt");
}
