#include "TextWindow.hpp"
#include "LoadResources.hpp"
#include "CodeWindow.hpp"
#include "ErrorWindow.hpp"
#include "ExplainWindow.hpp"

#include <stdio.h>
#include <string>
#include <iostream>
#include "Parser.hpp"

#include <iostream>
using namespace std;
using namespace sbide;

int main()
{
	// initialize the stuff:
    
    //Parser p("./Project2017/DEVLOGS/output.txt", "Project2017");
    Parser p("/Users/jjm3105/Desktop/Project2017/DEVFUNC/output.txt", "/Users/jjm3105/Desktop/Project2017");
    p.run();
    auto errors = p.getList();
    
	try
	{
        //sbide::ResourceLoader loader("./Project2017/DEVFUNC/Resources");
		sbide::ResourceLoader loader("/Users/jjm3105/Desktop/Resources");
		
		addErrorWindowLoader(loader);
		addCodeWindowLoader(loader);
		addExplainWindowLoader(loader);
        
        setCodeWindowContentRef(p.getMap());

		// load the errors into the error window:
		ErrorWindow errorWindow;

		string mess1 = "This is a test message\nBlah blah blah\nCool stuff.";
		string mess2 = "The pizza man\nHe invades us!.";
		
        for (auto& e : errors)
        {
            errorWindow.addError(e.text, e.type, e.sourceFile, e.desc, e.lineNum, e.charNum, e.isSingleChar);
        }
        
//		errorWindow.addError("This is not good formation", "warning", "Object.hpp", &mess1, 2, 2, false);
//		errorWindow.addError("This could use more work", "error", "Object.cpp", &mess2, 2, 3, true);

		vector<CodeWindow> errorPages = errorWindow.generateErrorWindows(); // errorPages stores all of the appropriate errors

		// prepare the error window:
		errorWindow.updateText();
		errorWindow.draw(errorPages);

	}
	catch (exception& e)
	{
		cout << "ERROR::" << e.what() << endl;
		cin.get();
		cin.get();
	}

	return 0;
}
