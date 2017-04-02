//
//  Parser.cpp
//  smIDE
//
//  Created by Kevin Liu on 4/1/17.
//  Copyright © 2017 Kevin Liu. All rights reserved.
//

#include "Parser.hpp"
#include "Utility2.hpp"
#include <cassert>

void Parser::run() {
	ep.process();
	createDesc();
	sbide::loadFiles(dir, code);
	
	simplifyMessages();
	
	sortByLineNum();

//	std::cout << code.size() << std::endl;
//	std::cout << code["test.cpp"].size() << std::endl;
//	std::cout << code["test.cpp"][2].size() << std::endl;
}

void Parser::print() {
	for(int i=0; i<list.size(); i++) {
		std::cout << list[i].text << std::endl;
		std::cout << *(list[i].desc) << std::endl;
	}
}

void Parser::simplifyMessages() {
	list = ep.getMessages();
	
	for(auto it = list.begin(); it!=list.end();) {
		
		if((*it).text == "linker") {
			(*it).text = "(Linker error) The cause of linker errors varies. Check that you have include guards for all header files! Make sure that you did not comment out the main function and that you did not define two main functions! Make sure you defined every function that is declared!";
			(*it).desc = &descriptions[28];
			it++;
			continue;
		}
		
		if((*it).type == "note" ||
		   (*it).text.find("wsign-compare") != std::string::npos ||
		   (*it).text.find("wc++11-extensions") != std::string::npos) {
			it = list.erase(it);
		}
		else {
			std::string s = "";
			if((*it).type == "warning") {
				if((*it).text.find("wtautological-compare") != std::string::npos) {
					(*it).text = "(Strange conditional comparison) Self-comparisons always evaluate to true or false! Double check the condition in the parentheses!";
					(*it).desc = &descriptions[0];
				}
				else if((*it).text.find("warray-bounds") != std::string::npos) {
					(*it).text = "(Array index out-of-bounds) You are trying to access an array index that is beyond the size of the array! We don't know what lies in this uncharted region. Check the array size and the desired array index.";
					(*it).desc = &descriptions[1];
				}
				else if((*it).text.find("wbool-compare") != std::string::npos) {
					(*it).text = "(Weird bool comparison) You are comparing a boolean value with an integer value. Is this what you really want to do?";
					(*it).desc = &descriptions[2];
				}
				else if((*it).text.find("wcomment") != std::string::npos) {
					(*it).text = "(wcomment) You wrote another block comment ' /* ' inside a block comment! Did you mean to do this? Check that you commented out the correct lines!";
					(*it).desc = &descriptions[3];
				}
				else if((*it).text.find("wuninitialized") != std::string::npos) {
					(*it).text = "(Uninitialized variable) The variable was not given a value before it is used! This may lead to undefined behavior! Initialize the variable before using it!";
					(*it).desc = &descriptions[4];
					(*it).isSingleChar = false;
				}
				else if((*it).text.find("wlogical-not-parentheses") != std::string::npos) {
					(*it).text = "(Logical not parentheses) The '!' operator is only applied to one side of the comparison. This may lead to unintended results. Did you mean to put parentheses around the comparison or put the '!' operator outside the parentheses?";
					(*it).desc = &descriptions[5];
				}
				else if((*it).text.find("wmaybe-uninitialized") != std::string::npos) {
					(*it).text = "(Possibly uninitialized variable) The variable may not be initialized when the program reaches this point. This may lead to undefined behavior. Make sure all edge cases are covered!";
					(*it).desc = &descriptions[6];
					(*it).isSingleChar = false;
				}
				else if((*it).text.find("wmisleading-indentation") != std::string::npos) {
					(*it).text = "(Misleading indentation) You indented a statement that, by its placement, may be confusing and misleading. The statement may be executed when it should not be, or vice versa. Check that you did not forget parentheses on your if-statement!";
					(*it).desc = &descriptions[7];
				}
				else if((*it).text.find("wparentheses") != std::string::npos) {
					(*it).text = "(Parenthetical ommission) You omitted parentheses here! This may cause problems because of precedence! Did you mean to do this?";
					(*it).desc = &descriptions[8];
				}
				else if((*it).text.find("wreturn-type") != std::string::npos) {
					(*it).text = "(Missing or incorrect return value) The function has an incorrect or missing return value! Change the return type or change the return value to fix.";
					(*it).desc = &descriptions[9];
				}
				else if((*it).text.find("wswitch") != std::string::npos) {
					(*it).text = "(Incomplete case handling) You did not write a case for that enumeration type! Therefore, if the switch statement encounters that enum value, it may cause problems or skip the switch statement entirely!";
					(*it).desc = &descriptions[11];
				}
				else if((*it).text.find("wunused") != std::string::npos) {
					(*it).text = "(Unused object) You did not use a variable or expression that you declared! Therefore, it is pointless. Did you mean to do this?";
					(*it).desc = &descriptions[12];
					(*it).isSingleChar = false;
				}
				else if((*it).text.find("wempty-body") != std::string::npos) {
					(*it).text = "(Empty body) The body inside the if, else, or do-while statement is empty! Therefore, the statement does nothing. Did you mean to do this?";
					(*it).desc = &descriptions[13];
				}
				else if((*it).text.find("missing terminating") != std::string::npos) {
					(*it).text = "(Missing closing quotation mark or apostrophe) You must write the respective closing quotation marks or apostrophe! Close the quote!";
					(*it).desc = &descriptions[14];
				}
				else if((*it).text.find("wmultichar") != std::string::npos) {
					(*it).text = "(Multichar char variable) You assigned more than 1 character to a char variable! This may be a problem. Did you mean to do this?";
					(*it).desc = &descriptions[15];
				}
				else {
					(*it).desc = &descriptions[23];
				}
			}
			else if((*it).type == "error") {
				if((*it).text.find("use of undeclared identifier 'cout'; did you mean 'std::cout'?") != std::string::npos) {
					(*it).text = "(Missing namespace) You must either use \"std::cout\" or write \"using namespace std\" before using cout in order to use cout!";
					(*it).desc = &descriptions[26];
					(*it).isSingleChar = false;
				}
				else if((*it).text.find("use of undeclared identifier 'cin'; did you mean 'std::cin'?") != std::string::npos) {
					(*it).text = "(Missing namespace) You must either use \"std::cin\" or write \"using namespace std\" before using cin in order to use cin!";
					(*it).desc = &descriptions[26];
					(*it).isSingleChar = false;
				}
				else if((*it).text.find("use of undeclared identifier 'endl'; did you mean 'std::endl'?") != std::string::npos) {
					(*it).text = "(Missing namespace) You must either use \"std::endl\" or write \"using namespace std\" before using endl in order to use endl!";
					(*it).desc = &descriptions[26];
					(*it).isSingleChar = false;
				}
				else if((*it).text.find("use of undeclared identifier 'cout'") != std::string::npos) {
					(*it).text = "(Missing #include statement : iostream) You need to include the <iostream> library in order to use cout!";
					(*it).desc = &descriptions[25];
					(*it).isSingleChar = false;
				}
				else if((*it).text.find("use of undeclared identifier 'cin'") != std::string::npos) {
					(*it).text = "(Missing #include statement : iostream) You need to include the <iostream> library in order to use cin!";
					(*it).desc = &descriptions[25];
					(*it).isSingleChar = false;
				}
				else if((*it).text.find("use of undeclared identifier 'endl'") != std::string::npos) {
					(*it).text = "(Missing #include statement : iostream) You need to include the <iostream> library in order to use endl!";
					(*it).desc = &descriptions[27];
					(*it).isSingleChar = false;
				}
				else if((*it).text.find("undeclared identifier") != std::string::npos) {
					(*it).text = "(Undeclared identifier) The compiler doesn't know what you are trying to use!";
					(*it).desc = &descriptions[16];
					(*it).isSingleChar = false;
				}
				
				
				else if((*it).text.find("expected ';'") != std::string::npos) {
					(*it).text = "(Semicolon expected) There is a semicolon missing here!";
					(*it).desc = &descriptions[17];
				}
				else if((*it).text.find("expected") != std::string::npos) {
					(*it).text = "(Syntax error) There is some syntax or expression issue here! As a result, the program cannot continue!";
					(*it).desc = &descriptions[17];
				}
				
				
				else if((*it).text.find("'main' must return 'int'") != std::string::npos) {
					(*it).text = "('main' must return 'int') The main function must return an int type, not a void type! Replace void with int!";
					(*it).desc = &descriptions[18];

				}
				
				
				else if((*it).text.find("segmentation fault") != std::string::npos) {
					(*it).text = "(Segfault) You're attempting to illegally read or write memory! Make sure you're not dereferencing an uninitialized pointer, or pointing to values out of bounds of an array.";
					(*it).desc = &descriptions[22];
				}
				
				
				else if((*it).text.find("const-qualified type") != std::string::npos) {
					(*it).text = "(Modifying a const variable) You are trying to change the value of a const variable. If a variable is declared const, you cannot modify it after it is created!";
					(*it).desc = &descriptions[19];
					(*it).isSingleChar = false;
				}
				
				
				else if((*it).text.find("redefinition") != std::string::npos) {
					(*it).text = "(Redefinition) You must not define a function more than once! Make sure that you only have one definition per function!";
					(*it).desc = &descriptions[20];
					(*it).isSingleChar = false;
				}
				
				
				else if((*it).text.find("explicit size or an initializer") != std::string::npos) {
					(*it).text = "(Explicit size) You must insert a size into the bracket that comes directly after the array declaration. The array size must be determined at compile-time!";
					(*it).desc = &descriptions[29];
				}
				
				
				else if((*it).text.find("type specifier") != std::string::npos) {
					(*it).text = "(Type required) All declarations require a specified type! Make sure none of your declared variables lack a specified type.";
					(*it).desc = &descriptions[30];
				}
				
				
				else if((*it).text.find("wreturn-type") != std::string::npos) {
					(*it).text = "(Illegal return value) You must not return a value inside a void function! Remove the return statement, or make it return nothing by writing \"return;\" (without the parentheses).";
					(*it).desc = &descriptions[10];
				}
				
				
				else if((*it).text.find("missing terminator") != std::string::npos) {
					(*it).text = "(Missing closing quotation or apostrophe) There is a missing closing quotation or apostrophe! Without that terminating character, the entire rest of your code will be considered as between the pair of characters!";
					(*it).desc = &descriptions[14];
				}
				
				
				else {
					(*it).desc = &descriptions[23];
				}
			}
			else {
				(*it).text = "(Uh-oh) Something bad happened!";
				(*it).desc = &descriptions[21];
			}
			
			it++;
		}
	}
	
}

void Parser::createDesc() {
	descriptions.push_back("Programmers often compare two different variables. This warning indicates that you \nare comparing one variable with itself, which is a rare \nsituation that probably doesn't do what you intend.");
	descriptions.push_back("Accessing an array index beyond the size of the array can lead to undefined behavior, \nwhich means that the results are unpredictable. \nIt's possible that memory allocated for something other than the array is unintentionally overwritten, \nwhich could lead to a multitude of issues, including an abortion of the program.");
	descriptions.push_back("Programmers often compare two variables of the same type. \nThis warning indicates that you are comparing a boolean value with a non-boolean value, \nwhich may not be what you intended.");
	descriptions.push_back("Comments inside another comment may indicate that the programmer commented out lines \nthat he or she wished to leave in, which could lead to errors.");
	descriptions.push_back("Use of an uninitialized variable could lead to undefined behavior during runtime, \nwhich could cause the program to crash or perform unintended actions. \nIt is good practice to assign a value to a variable once it is declared \nto prevent problems during runtime.");
	descriptions.push_back("Beginner programmers often leave out parentheses when using the logical-not operator, \nwhich leads to incorrect behavior. This warning warns the user that the omission of \nparentheses causes different results.");
	descriptions.push_back("Beginner programmers may write code that only assigns a value to a variable given certain conditions. \nThis means that if the conditions are not met, then the variable may be uninitialized, \nwhich could be a problem if the variable ends up being used by another statement of function.");
	descriptions.push_back("Beginner programmers may write two statements following an if-condition without \ngrouping these two statements in a block. They may incorrectly think that \nindenting the second statement connects it to the if-condition. \nThis warning warns against this common mistake.");
	descriptions.push_back("The omission of parentheses could change the value of a variable \nbecause of precedence, especially in arithmetic evaluation.");
	descriptions.push_back("Every non-void function should return a value of the specific type. \nBeginner programmers may forget to return a value.");
	descriptions.push_back("Void functions are functions that do not return a value when it is called. \nNon-void functions return values that may be used to modify or \nassign another variable inside the statement in which it is called.");
	descriptions.push_back("Beginner programmers may either forget to write a case for a specific case \nin a switch statement or forget to write a default case.");
	descriptions.push_back("Beginner programmers may accidentally initialize a variable and not even use it. \nWhen optimizing, those unused variables take up valuable memory and can be \ndebilitating when at large scale. When programming, we encourage you to avoid clutter.");
	descriptions.push_back("Beginner programmers may forget to attach statements to an if, else, or \ndo-while statement, which makes that statement useless.");
	descriptions.push_back("Upon encountering the opening double-quotation marks or left apostrophe, \nthe compiler expects to find the closing double-quotation marks or right \napostrophe that indicates the termination of a string or char literal. \nHowever, if the compiler finds a semicolon that marks the end of statement before finding \nthe closing quotation marks, it produces an error and does not compile.");
	descriptions.push_back("Beginner programmers may accidentally use a char variable when they should \nuse a string variable instead. This warning checks this situation when a char variable \nis assigned more than one character, which is usually not what beginners intend.");
	descriptions.push_back("You probably either: \nused a library function and failed to include the correct header; \nfailed to use the correct namespace; \nor failed to declare a variable *in the scope* prior to trying to use it! \nWithout the correct header or namespace, the compiler will not recognize \nthe piece of code that came from that library. \nThus, the compiler will not know how to implement that piece of code, and will be unable to compile. \nIf a variable is not yet declared, the compiler does not recognize it \nwhen it tries to execute a statement containing that variable. \nYour desired variable does not yet exist, so to speak. Even if the variable has been \ndeclared elsewhere in the program, if it is not in the local scope, \nthe compiler still will not know the variable you are trying to use. \nTherefore, the program cannot compile. \n\nNOTE: this can happen if you just misspell the variable you are using. Capitalization matters! \nNOTE2: If you did not declare the type of your variable, it might register as an undeclared variable. Solving this error might be as simple as declaring the type of your variable.");
	descriptions.push_back("Go through the area and try to remember all those C++ syntax rules that you learned. \nIt's probably those pesky semicolons. \nMore significantly, you might have made a mistake that causes \nthe compiler to *think* that there's a syntax error, when in reality \nit's a totally different reason, like forgetting to type the types of variables \nor defining a function within a function \n(the latter is not allowed - the compiler thinks that you are trying to create two independent function, \nand that you merely forgot a semicolon somewhere to differentiate between the two).");
	descriptions.push_back("First of all, the C++ Standard explicitly prohibits main from being a void function. \nIn addition, the return value of main indicates how the program was exited. \nA return value of 0 indicates successful execution of the program, \nwhereas a non-zero return value indicates an abnormal exit. \nOther programs may use the return value of main to determine \nwhether the application was executed successfully. \nYou do not have to explicitly write a return statement inside main.");
	descriptions.push_back("The keyword \"const\" in front of a variable tells the compiler \nto prevent modification of that variable. \nIf, however, the compiler encounters a statement that attempts to \nmodify that variable, the code will not compile.");
	descriptions.push_back("Functions may be declared multiple times, but they may only be defined once. \nThus, the compiler knows what specific statements to execute \nupon reaching a function call, and there is no ambiguity between multiple definitions. \nYou may, however, overload a function if the arguments differ in each overload.");
	descriptions.push_back("A fatal error has occurred. Check if any files are missing, \nyour syntax is correct, and that your machine is not on fire.");
	descriptions.push_back("When your program runs, it has access to certain portions of memory. \nYour program is only allowed to touch memory that belongs to it -- the memory previously mentioned. \nAny access outside that area will cause a segmentation fault. \nThere are four common mistakes that lead to segmentation faults: \ndereferencing nullptr, \ndereferencing an uninitialized pointer, \ndereferencing a pointer that has been freed, deleted or that has gone out of scope, \nand writing off the end of an array.");
	descriptions.push_back("No description");
	descriptions.push_back("cin isn't defined automatically in C++! The C++ Standard Library contains \na predefined header file named iostream.h. Within this header file \nare implementations of functions used for input and output, like cin and cout. \nIn order to use these functions, you must include the header file \nwith #include <iostream>.");
	descriptions.push_back("cout isn't defined automatically in C++! The C++ Standard Library contains \na predefined header file named iostream.h. Within this header file \nare implementations of functions used for input and output, like cin and cout. \nIn order to use these functions, you must include the header file \nwith #include <iostream>.");
	descriptions.push_back("In order to use any C++ Standard Library functions, you need to use \nthe correct namespace (std), so that the compiler can recognize you are \nusing a function from the Standard Library.");
	descriptions.push_back("endl isn't defined automatically in C++! The C++ Standard Library contains \na predefined header file named iostream.h. Within this header file \nare implementations of functions used for input and output, like endl. \nIn order to use these functions, you must include the header file \nwith #include <iostream>.");
	descriptions.push_back("Linker errors can be caused by many different things, including: \nmore than one main function in the program, \nno main function in the program, \ncircular dependency with header files, \nreimplementation of classes, etc.");
	descriptions.push_back("Unlike vectors, arrays in C++ must have a size before it is compiled. \nVectors are dynamic and can increase/decrease in size, but arrays are different. \nIf the compiler does not know the size of an array, \nit will not compile the program.");
	descriptions.push_back("In C++, an object oriented programming language, \nvariables & functions need type (array didn't work)");
	
	
	
	
}

void Parser::sortByLineNum() {
	for(int i=0; i<list.size(); i++) {
		int min = i;
		for(int j=i; j<list.size(); j++) {
			if(list[j].lineNum < list[min].lineNum) {
				min = j;
			}
			else if(list[j].lineNum == list[min].lineNum) {
				if(list[j].charNum < list[min].charNum) {
					min = j;
				}
			}
		}
		std::swap(list[i], list[min]);
	}
}



