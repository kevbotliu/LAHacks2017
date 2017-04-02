//
//  ErrorProcessor.h
//  smIDE
//
//  Created by Kevin Liu on 4/1/17.
//  Copyright © 2017 Kevin Liu. All rights reserved.
//

#ifndef ErrorProcessor_hpp
#define ErrorProcessor_hpp

#include <vector>
#include <string>
#include <iostream>

struct Msg {
	Msg(std::string name, std::string val, std::string srcFile, int num1, int num2) : text(name), type(val), sourceFile(srcFile), lineNum(num1), charNum(num2), isSingleChar(true), desc(nullptr) {}
	
	std::string text;
	std::string type;
	std::string sourceFile;
	int lineNum;
	int charNum;
	bool isSingleChar;
	std::string* desc;
};

class ErrorProcessor {
public:
	ErrorProcessor() : compErrorsFileName("output.txt") {}
	ErrorProcessor(std::string name) : compErrorsFileName(name) {}
	
	void populateDesc();
	void process();
	std::vector<Msg> getMessages() {return messages;}
	
private:
	std::string compErrorsFileName;
	std::vector<Msg> messages;
	
};

#endif /* ErrorProcessor_hpp */
