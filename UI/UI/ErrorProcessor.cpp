//
//  Parser.cpp
//  Test
//
//  Created by Kevin Liu on 4/1/17.
//  Copyright © 2017 Kevin Liu. All rights reserved.
//

#include <fstream>
#include <sstream>
#include "ErrorProcessor.hpp"

void ErrorProcessor::process() {
	
	
	std::ifstream file(compErrorsFileName);
	std::string line;
	
	while(std::getline(file, line)) {
		
		if(line.find("linker") != std::string::npos) {
			messages.push_back(Msg("linker", "error", "none", 1, 1));
			continue;
		}
		
		std::stringstream linestream(line);
		std::string sourceFile, type, lineNum, charNum, message;
		
		std::getline(linestream, sourceFile, ':');
		std::getline(linestream, lineNum, ':');
		std::getline(linestream, charNum, ':');
		linestream >> std::ws;
		std::getline(linestream, type, ':');
		linestream >> std::ws;
		std::getline(linestream, message);
		
		for(int i=0; i<message.size(); i++) {
			if(isalpha(message[i])) {
				message[i] = tolower(message[i]);
			}
		}
		
//		std::cout << sourceFile << lineNum << charNum << type << message << std::endl;
		
		messages.push_back(Msg(message, type, sourceFile, stoi(lineNum), stoi(charNum)));
	}
}
