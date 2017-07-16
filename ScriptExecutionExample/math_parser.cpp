#include "stdafx.h"
#include "math_parser.h"

#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

namespace InputScripts {
MathParser::MathParser()
{
}

void MathParser::SetVariableValue(std::string variable, double value)
{
	if (HasExternalFlag(variable))
	{
		*std::get<1>(external_values[GetExternalValueIndex(variable)]) = value;
	}
	else
	{
		variableHashTable.RemoveItem(variable);
		variableHashTable.AddItem(variable, value);
	}
}

double MathParser::GetVariableValue(std::string variable)
{
	if (HasExternalFlag(variable))
	{
		return *std::get<1>(external_values[GetExternalValueIndex(variable)]);
	}
	return variableHashTable.FindVal(variable);
}

double MathParser::Solve(std::string str)
{
	if (str == "") { return 0; }
	int neededParenthesis = 0;

	// This part handles AND NOT OR XOR == != >= <= > <
	for (int i = str.size(); i > 0; i--) {
		if (str[i] == '~') {
			i -= 2;
			if (str[i + 1] == 'A') { // BITWISE AND
				std::string firstHalf = str; firstHalf.replace(i, str.size(), "");
				std::string secondHalf = str; secondHalf.replace(0, i + 3, "");

				return (int)Solve(firstHalf) && (int)Solve(secondHalf);
			}
			if (str[i + 1] == 'W') { // LOGICAL AND
				std::string firstHalf = str; firstHalf.replace(i, str.size(), "");
				std::string secondHalf = str; secondHalf.replace(0, i + 3, "");

				return (int)Solve(firstHalf) & (int)Solve(secondHalf);
			}
			if (str[i + 1] == 'N') { // NOT
				std::string firstHalf = str; firstHalf.replace(i, str.size(), "");
				std::string secondHalf = str; secondHalf.replace(0, i + 3, "");
				return ~(int)Solve(secondHalf);
			}
			if (str[i + 1] == 'O') { // BITWISE OR
				std::string firstHalf = str; firstHalf.replace(i, str.size(), "");
				std::string secondHalf = str; secondHalf.replace(0, i + 3, "");
				return (int)Solve(firstHalf) || (int)Solve(secondHalf);
			}
			if (str[i + 1] == 'Q') { // LOGICAL OR
				std::string firstHalf = str; firstHalf.replace(i, str.size(), "");
				std::string secondHalf = str; secondHalf.replace(0, i + 3, "");
				return (int)Solve(firstHalf) | (int)Solve(secondHalf);
			}
			if (str[i + 1] == 'X') { // XOR
				std::string firstHalf = str; firstHalf.replace(i, str.size(), "");
				std::string secondHalf = str; secondHalf.replace(0, i + 3, "");
				return (int)Solve(firstHalf) ^ (int)Solve(secondHalf);
			}
			if (str[i + 1] == 'E') { // ==
				std::string firstHalf = str; firstHalf.replace(i, str.size(), "");
				std::string secondHalf = str; secondHalf.replace(0, i + 3, "");
				return Solve(firstHalf) == Solve(secondHalf);
			}
			if (str[i + 1] == 'D') { // !=
				std::string firstHalf = str; firstHalf.replace(i, str.size(), "");
				std::string secondHalf = str; secondHalf.replace(0, i + 3, "");
				return Solve(firstHalf) != Solve(secondHalf);
			}
			if (str[i + 1] == 'S') { // >=
				std::string firstHalf = str; firstHalf.replace(i, str.size(), "");
				std::string secondHalf = str; secondHalf.replace(0, i + 3, "");
				return Solve(firstHalf) >= Solve(secondHalf);
			}
			if (str[i + 1] == 'M') { // <=
				std::string firstHalf = str; firstHalf.replace(i, str.size(), "");
				std::string secondHalf = str; secondHalf.replace(0, i + 3, "");
				return Solve(firstHalf) <= Solve(secondHalf);
			}
			if (str[i + 1] == 'L') { // >
				std::string firstHalf = str; firstHalf.replace(i, str.size(), "");
				std::string secondHalf = str; secondHalf.replace(0, i + 3, "");
				return Solve(firstHalf) > Solve(secondHalf);
			}
			if (str[i + 1] == 'P') { // <
				std::string firstHalf = str; firstHalf.replace(i, str.size(), "");
				std::string secondHalf = str; secondHalf.replace(0, i + 3, "");
				return Solve(firstHalf) < Solve(secondHalf);
			}

			// Throw some sort of error at this point, "operator error"
		}

		do {
			if (str[i] == ')') { neededParenthesis++; }
			if (str[i] == '(') { neededParenthesis--; }
			if (neededParenthesis) { i--; }
		} while (neededParenthesis && (i > 0));
	}

	return Solve2(str);
}

bool MathParser::AddExternalValue(std::string str, double* value)
{
	std::string flag = string_manipulation.GetWord(str, 0, false); // get first segment of "given.value"
	if (flag == "") { return false; }
	if (flag.size() == str.size()) { return false; } // If flag isn't any shorter than the string (no '.' was used)

	std::string field = string_manipulation.GetWord(str, flag.size() + 1, true);
	if (field == "") { return false; }

	if (!HasExternalFlag(str)) { external_flag.push_back(flag); }
	external_values.push_back(std::make_tuple(str, value));

	return true;
}

bool MathParser::HasExternalFlag(std::string str)
{
	if (external_flag.size() == 0) { return false; }
	str = string_manipulation.GetWord(str, 0, false);

	for (int i = 0; i < external_flag.size(); i++)
	{
		if (str == external_flag[i]) { return true; }
	}

	return false;
}

int MathParser::GetExternalValueIndex(std::string str)
{
	for (int i = 0; i < external_values.size(); i++)
	{
		if (str == std::get<0>(external_values[i])) { return i; }
	}
	return -1;
}

//Second solve handles + - * / % !
double MathParser::Solve2(std::string str) {
	if (str == "") { return 0; }
	double result = 0;
	double cache = 0;
	double positive = 1;
	bool div = 0;
	bool mult = 0;
	bool mod = 0;
	bool not = 0;

	for (int strPos = 0; strPos < str.length(); strPos++) {
		if (isLetter(str[strPos]) || isNumber(str[strPos]) || str[strPos] == '(') {
			if (!div && !mult && !mod) {
				if (!not) {
					cache = getValue(str, strPos)*positive;
				}
				else {
					cache = !getValue(str, strPos)*positive; not = 0;
				}
			}
			else {
				if (mult) {
					if (!not) {
						cache *= getValue(str, strPos); mult = 0;
					}
					else {
						cache *= !getValue(str, strPos); mult = 0; not = 0;
					}

				}
				if (div) {
					if (!not) {
						cache /= getValue(str, strPos); div = 0;
					}
					else {
						cache /= !getValue(str, strPos); div = 0; not = 0;
					}
				}

				if (mod) {
					if (!not) {
						cache = fmod(cache, getValue(str, strPos)); mod = 0;
					}
					else {
						cache = !fmod(cache, getValue(str, strPos)); mod = 0; not = 0;
					}

				}
			}
		}
		if (str[strPos] == '+') { result += cache; cache = 0; positive = 1; }
		if (str[strPos] == '-') { result += cache; cache = 0; positive = -1; }
		if (str[strPos] == '*') { mult = 1; }
		if (str[strPos] == '/') { div = 1; }
		if (str[strPos] == '%') { mod = 1; }
		if (str[strPos] == '!') { not = !not; }
	}

	return result + cache;
}

bool MathParser::isLetter(char i) {
	bool ret = 0;
	if ((static_cast<int>(i) > 96 && static_cast<int>(i) < 123) || (static_cast<int>(i) > 64 && static_cast<int>(i) < 91)) {
		ret = 1;
	}
	else {
		ret = 0;
	}
	return ret;
}

bool MathParser::isNumber(char i) {
	bool ret = 0;
	if (static_cast<int>(i) > 47 && static_cast<int>(i) < 58) {
		ret = 1;
	}
	else {
		ret = 0;
	}
	return ret;
}

bool MathParser::isSystemVar(std::string str)
{
	for (int i = 0; i < ARRAY_SIZE(ReturnFunctions); i++)
	{
		if (ReturnFunctions[i] == str) { return true; }
	}
	return false;
}

// Returns the value of an element in a string given the position.
double MathParser::getValue(std::string &str, int &strPos) {
	if (isLetter(str[strPos])) { return getValueWord(str, strPos); }
	if (isNumber(str[strPos])) { return getValueNumber(str, strPos); }
	if (str[strPos]) { // ?
			// I still have no clue what this is supposed to do
		std::string gp = string_manipulation.getParenthesis(str, strPos);
		int res = Solve(gp);
		strPos += gp.size() + 1;
		return res;
	}
	// Throw this error here. "getVal() error\n" + str + endl + str[strPos] + endl;

	return 0;
}

// Gets the value of a number in a string. There's probably an easier way to do this.
double MathParser::getValueNumber(std::string str, int &strPos) {
	double result = 0;
	double decimalCount = 1;
	int go = 1;
	//char strc = str[3]; Uhhh

	while (go) {
		int strv = str[strPos];
		strv -= '0';
		if (isNumber(str[strPos])) { result *= 10; result += strv; strPos++; }
		else { break; }
	}
	if (str[strPos] == '.') {
		strPos++;
		go = 1;
		while (go) {
			int strv = str[strPos];
			strv -= '0';
			if (isNumber(str[strPos])) { decimalCount *= 0.1; result += decimalCount*strv; strPos++; }
			else { go = 0; }
		}
	}
	return result;
}

double MathParser::getValueWord(std::string str, int &strPos) {
	std::string result = "";

	// This seems unstable
	while (true) {
		if (isLetter(str[strPos]) || isNumber(str[strPos]) || str[strPos] == '.') { result += str[strPos]; strPos++; }
		else { break; }
	}

	if (isSystemVar(result)) {
		std::string parenthesis;
		if (str[strPos] == '(') {
			parenthesis = string_manipulation.getParenthesis(str.replace(0, strPos, ""), 0);
			strPos += parenthesis.size() + 1;
		}

		return getSystemVar(result, parenthesis);

	}

	std::string parenthesis;
	if (str[strPos] == '(') {
		parenthesis = string_manipulation.getParenthesis(str.replace(0, strPos, ""), 0);
		strPos += parenthesis.size() + 1;
		if (getCommadContent(parenthesis, 1) == "") {
			result += "(" + string_manipulation.numToString(Solve(parenthesis)) + ")";
		}
		else {
			result += "(" + string_manipulation.numToString(Solve(getCommadContent(parenthesis, 0))) + "," + string_manipulation.numToString(Solve(getCommadContent(parenthesis, 1))) + ")";
		}
	}

	return GetVariableValue(result);
}

double MathParser::getSystemVar(std::string var, std::string sentParenthesis = "") {
	if (var == "true") { return 1; }
	if (var == "false") { return 0; }
	if (var == "pi") { return 3.14159265359; }
	if (var == "floor") {
		int res = Solve(sentParenthesis);
		return res;
	}
	if (var == "abs") { return std::abs(Solve(sentParenthesis)); }
	if (var == "atan") { return std::atan(Solve(sentParenthesis)); }
	if (var == "cos") { return std::cos(Solve(sentParenthesis)); }
	if (var == "deg") { return (Solve(sentParenthesis) / 3.14159265359) * 180; }
	if (var == "exp") { return std::exp(Solve(sentParenthesis)); }
	if (var == "log") { return std::log(Solve(sentParenthesis)); }
	if (var == "rad") { return (Solve(sentParenthesis) / 180)*3.14159265359; }
	if (var == "result") { return ifResult; }
	if (var == "sgn") {
		double res = Solve(sentParenthesis);
		if (res > 0) { return 1; }
		if (res < 0) { return -1; }
		if (res == 0) { return 0; }
	}
	if (var == "sin") { return std::sin(Solve(sentParenthesis)); }
	if (var == "sqr") { return std::sqrt(Solve(sentParenthesis)); }
	if (var == "tan") { return std::tan(Solve(sentParenthesis)); }

	// throw error here.
	// cout << "getSystemVar() error\n"; system("pause");
	return 0;
}


//Get Comma'd content
std::string MathParser::getCommadContent(std::string str, int pos = 0) {
	getCommadContentEnd = 0;
	std::string result = "";
	int com = 0;
	bool reqquote = 0;
	int unsigned reqpara = 0;
	for (int u = 0; u < str.size(); u++) {

		if (str[u] == '/"') { reqquote = !reqquote; }
		if (str[u] == '(') { reqpara++; }
		if (str[u] == ')') { reqpara--; }
		if (str[u] == ',' && reqquote == 0 && reqpara == 0) { com++; u++; }
		if (com == pos) { result += str[u]; }
	}

	if (pos > com) { getCommadContentEnd = 1; }
	return result;
}
}