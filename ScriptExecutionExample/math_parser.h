#pragma once
#include <string>
#include <iostream>
#include "hash_tables.h"
#include "string_manipulation.h"
#include <vector>
#include <tuple>

// This parser takes in a math string and outputs the result as a double.
// It stores variables in local hashtables.

namespace InputScripts {
class MathParser
{
public:
	MathParser();
	void SetVariableValue(std::string, double);
	double GetVariableValue(std::string);

	// This takes in a string of math and returns the calculated value.
	double Solve(std::string);

	// Add variable that will be referenced with read/write access by the parser. (format value like this - "pad.field")
	// Example: AddExternalValue("pad.a", &value)
	bool AddExternalValue(std::string, double*);

	bool ifResult = false;
private:
	bool HasExternalFlag(std::string);
	vector<std::string> external_flag;
	vector<tuple<std::string, double*>> external_values;
	int GetExternalValueIndex(std::string);

	double Solve2(std::string);
	bool isLetter(char);
	bool isNumber(char);
	bool isSystemVar(std::string);
	int getCommadContentEnd = 0;
	double getValue(std::string &, int &);
	double getValueNumber(std::string, int &);
	double getValueWord(std::string, int &);
	double getSystemVar(std::string var, std::string sentParenthesis);
	std::string getCommadContent(std::string str, int pos);
	std::string ReturnFunctions[16] = { "abs", "atan", "cos", "deg", "exp", "false", "floor", "log", "pi", "result", "rad", "sgn", "sin", "sqr", "tan", "true" };
	StringManipulation string_manipulation;
	HashTable variableHashTable;
};
}