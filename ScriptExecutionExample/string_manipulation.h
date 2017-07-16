#pragma once
#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>

namespace InputScripts {
class StringManipulation
{
public:
	StringManipulation();

	std::string getParenthesis(std::string, int);
	std::string numToString(double);
	bool isLetter(char);
	bool isNumber(char);
	std::string GetWord(std::string, int, bool);
	// Actually removes spaces, tabs, and endlines.
	std::string RemoveSpaces(std::string);
	std::string Capitalize(std::string);
	std::string SearchAndReplace(std::string&, const std::string&, const std::string&);
	std::string LoadTextFromFile(const std::string&);
};
}