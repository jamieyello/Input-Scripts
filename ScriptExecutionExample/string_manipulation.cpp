#pragma once
#include "stdafx.h"
#include "string_manipulation.h"
#include <iostream>


namespace InputScripts {
StringManipulation::StringManipulation()
{
}

//This function returns the contents of the first detected parenthesis.
std::string StringManipulation::getParenthesis(std::string str, int i = 0) {
	int firstParenthesis = -1;
	int closingParenthesis = -1;
	int closingParenthesisNeeded = 0;

	for (i = i; i < str.size(); i++) {
		if (static_cast<int>(str[i]) == 40) { firstParenthesis = i; closingParenthesisNeeded = 1; break; }
	}

	while (closingParenthesisNeeded) {
		i++;
		if (str[i] == '(') { closingParenthesisNeeded++; }
		if (str[i] == ')') { closingParenthesisNeeded--; }

	}

	if (firstParenthesis > -1) {
		closingParenthesis = i;

		if (firstParenthesis != -1) {
			std::string str2 = str.replace(closingParenthesis, str.size(), "");
			std::string str1 = str.replace(0, firstParenthesis + 1, "");
		}
	}

	return str;
}

std::string StringManipulation::numToString(double Number) {
	std::string Result;
	std::ostringstream convert;
	convert << Number;
	return convert.str();
}

bool StringManipulation::isLetter(char i) {
	if ((static_cast<int>(i) > 96 && static_cast<int>(i) < 123) || (static_cast<int>(i) > 64 && static_cast<int>(i) < 91)) {
		return true;
	}
	else {
		return false;
	}
}

bool StringManipulation::isNumber(char i) {
	if (static_cast<int>(i) > 47 && static_cast<int>(i) < 58) {
		return true;
	}
	else {
		return false;
	}
}

// This will return a word in a string given the starting position.
std::string StringManipulation::GetWord(std::string str, int position, bool count_period = false)
{
	std::string result = "";

	for (int i = 0; i < str.length(); i++) {
		if (count_period) {
			if ((isLetter(str[position + i])) || (str[position + i] == '.')) {
				result += str[position + i];
			}
			else {
				return result;
			}
		}
		else {
			if (isLetter(str[position + i])) {
				result += str[position + i];
			}
			else {
				return result;
			}
		}
	}

	return result;
}

std::string StringManipulation::RemoveSpaces(std::string str)
{
	str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
	str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
	str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	return str;
}

std::string StringManipulation::Capitalize(std::string us) {
	//97-122 lowercase
	//65-90 uppercase
	std::string usr = "";
	for (int u = 0; u < us.size(); u++) {
		if (static_cast<int>(us[u]) > 96 && static_cast<int>(us[u]) < 123) { us[u] -= 32; }
	}
	return us;
}

std::string StringManipulation::SearchAndReplace(std::string &str, const std::string &from, const std::string &to)
{
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
	return str;
}

std::string StringManipulation::LoadTextFromFile(const std::string& file_path)
{
	std::string script;

	// Load test script
	
	std::ifstream in_file;
	in_file.open(file_path);

	std::string temp = "";
	while (!in_file.eof())
	{
		in_file >> temp;
		script += temp;
	}

	in_file.close();

	return script;
}
}