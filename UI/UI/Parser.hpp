//
//  Parser.hpp
//  smIDE
//
//  Created by Kevin Liu on 4/1/17.
//  Copyright © 2017 Kevin Liu. All rights reserved.
//

#ifndef Parser_hpp
#define Parser_hpp

#include "ErrorProcessor.hpp"
#include <map>


class Parser {
public:
	Parser(std::string direc) : dir(direc) {}
	Parser(std::string fileName, std::string direc) : ep(fileName), dir(direc) {}
	
	void run();
	std::vector<Msg> getList() {return list;}
    
    std::map<std::string, std::vector<std::string> > &getMap() { return code; }

private:
	void simplifyMessages();
	void sortByLineNum();
	void createDesc();
	
	void print();
	
	ErrorProcessor ep;
	std::string dir;
	std::vector<Msg> list;
	std::vector<std::string> descriptions;
	std::map<std::string, std::vector<std::string> > code;
};

#endif /* Parser_hpp */
