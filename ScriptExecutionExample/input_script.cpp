#pragma once
#include "stdafx.h"
#include "input_script.h"

namespace InputScripts {
InputScript::InputScript()
{
	for (int i = 0; i < scope_limit; i++) {
		return_address[i] = 0;
		scope_instruction_type[i] = null_value;
		initial_function_reference[i] = 0;
		recursive_function_reference[i] = 0;
	}

	// Link input with math parser
	math_parser.AddExternalValue("pad.a", &result.A);
	math_parser.AddExternalValue("pad.b", &result.B);
	math_parser.AddExternalValue("pad.x", &result.X);
	math_parser.AddExternalValue("pad.y", &result.Y);

	math_parser.AddExternalValue("pad.up", &result.Up);
	math_parser.AddExternalValue("pad.down", &result.Down);
	math_parser.AddExternalValue("pad.left", &result.Left);
	math_parser.AddExternalValue("pad.right", &result.Right);

	math_parser.AddExternalValue("pad.r", &result.R);
	math_parser.AddExternalValue("pad.zr", &result.ZR);
	math_parser.AddExternalValue("pad.l", &result.L);
	math_parser.AddExternalValue("pad.zl", &result.ZL);

	math_parser.AddExternalValue("pad.start", &result.Start);
	math_parser.AddExternalValue("pad.select", &result.Select);
	math_parser.AddExternalValue("pad.home", &result.Home);

	math_parser.AddExternalValue("pad.leftStickX", &result.LeftStickX);
	math_parser.AddExternalValue("pad.leftStickY", &result.LeftStickY);
	math_parser.AddExternalValue("pad.rightStickX", &result.RightStickX);
	math_parser.AddExternalValue("pad.rightStickY", &result.RightStickY);

	math_parser.AddExternalValue("pad.touchDown", &result.TouchScreenDown);
	math_parser.AddExternalValue("pad.touchX", &result.TouchScreenX);
	math_parser.AddExternalValue("pad.touchY", &result.TouchScreenY);
}

InputScript::~InputScript()
{
}

bool InputScript::LoadScript(std::string file_path) {
	std::string script = string_functions.LoadTextFromFile(file_path);
	ModifyScript(script);
	EvaluateScript(script);

	// Convert the text script into an array of instructions.
	SetInstructions(script);

	return true;
}

bool InputScript::ExecuteFrame(Input input)
{
	result.SetInput(input);

	current_scope = 0;
	if (text_output_enabled) { text_output = ""; }

	int i = 0;
	while (i < instruction.size()) {
		i = ExecuteInstruction(instruction[i], i);
	}

	if (current_scope) { text_output += "Warning; scope was " + string_functions.numToString(current_scope) + " on exit.\n"; }

	return true;
}

// Execute single instruction. Address only relevent when used with looping commands.
int InputScript::ExecuteInstruction(Instruction &inst, int address)
{
	switch (inst.instruction_type) {
	case opening_bracket:
		if (skip_next_scope) {
			int needed_closing_brackets = 1;
			for (address++; (address < instruction.size()) && (needed_closing_brackets != 0); address++)
			{
				if (instruction[address].instruction_type == opening_bracket) { needed_closing_brackets++; };
				if (instruction[address].instruction_type == closing_bracket) { needed_closing_brackets--; };
			}
			skip_next_scope = false;

			return address;
		}
		else {
			return_address[current_scope] = address;
			current_scope++;
		}
		break;

	case closing_bracket:
		if (current_scope < 1) { cout << loaded_script << "\n" << GetInstructionDebug(instruction)
			<< "on closing bracket at line " << address << " scope was " << current_scope << "\nProgram will now likely crash.\n"; system("PAUSE"); }

		switch (instruction[return_address[current_scope - 1] - 1].instruction_type) {
		case while_statement:
			if (math_parser.Solve(instruction[return_address[current_scope - 1] - 1].math_string[0]) > 0) {
				address = return_address[current_scope - 1];
			}
			else {
				current_scope--;
			}
			break;

		case do_statement:
			if (instruction[address + 1].instruction_type == while_statement) {
				if (math_parser.Solve(instruction[address + 1].math_string[0]) > 0) {
					address = return_address[current_scope - 1];
				}
				else {
					current_scope--;
				}
			}
			else {
				// Throw error here
				std::cout << "do matching while statement not found.\n";
				current_scope--;
			}
			break;

		case for_loop:
			ExecuteInstruction(embedded_functions[instruction[return_address[current_scope - 1] - 1].embedded_function_reference[1]], 0);
			if (math_parser.Solve(instruction[return_address[current_scope - 1] - 1].math_string[0]) > 0) {
				address = return_address[current_scope - 1];
			}
			else {
				current_scope--;
			}
			break;

		case if_statement:
			current_scope--;
			break;

		case else_statement:
			current_scope--;
			break;

		default:
			text_output += "Warning: an ending bracket had no type.\n";
			current_scope--;
			break;
		}

		break;

	case set_value:
		math_parser.SetVariableValue(inst.value_to_modify[0], math_parser.Solve(inst.math_string[0]));
		break;

	case inc_value_by:
		math_parser.SetVariableValue(inst.value_to_modify[0], math_parser.GetVariableValue(inst.value_to_modify[0]) + math_parser.Solve(inst.math_string[0]));
		break;

	case dec_value_by:
		math_parser.SetVariableValue(inst.value_to_modify[0], math_parser.GetVariableValue(inst.value_to_modify[0]) - math_parser.Solve(inst.math_string[0]));
		break;

	case mul_value_by:
		math_parser.SetVariableValue(inst.value_to_modify[0], math_parser.GetVariableValue(inst.value_to_modify[0]) * math_parser.Solve(inst.math_string[0]));
		break;

	case div_value_by:
		math_parser.SetVariableValue(inst.value_to_modify[0], math_parser.GetVariableValue(inst.value_to_modify[0]) / math_parser.Solve(inst.math_string[0]));
		break;

	case inc_value:
		math_parser.SetVariableValue(inst.value_to_modify[0], math_parser.GetVariableValue(inst.value_to_modify[0]) + 1);
		break;

	case dec_value:
		math_parser.SetVariableValue(inst.value_to_modify[0], math_parser.GetVariableValue(inst.value_to_modify[0]) - 1);
		break;

	case for_loop:
		scope_instruction_type[current_scope] = for_loop;

		// Execute initial function
		ExecuteInstruction(embedded_functions[inst.embedded_function_reference[0]], 0);
		break;

	case if_statement:
		if (math_parser.Solve(inst.math_string[0])) {
			math_parser.ifResult = true;
			scope_instruction_type[current_scope] = if_statement;
		}
		else {
			math_parser.ifResult = false;
			skip_next_scope = true;
		}
		break;

	case else_statement:
		if (math_parser.ifResult) {
			skip_next_scope = true;
		}
		else {
			scope_instruction_type[current_scope] = else_statement;
		}
		break;

	case do_statement:
		scope_instruction_type[current_scope] = do_statement;
		break;

	case while_statement:
		if (math_parser.Solve(inst.math_string[0])) {
			scope_instruction_type[current_scope] = while_statement;
		}
		else {
			skip_next_scope = true;
		}
		break;

	case print_statement:
		if (text_output_enabled) {
			text_output += string_functions.numToString(math_parser.Solve(inst.math_string[0])) + "\n";
		}
		break;
		
	case clear_statement:
		if (text_output_enabled) { text_output = ""; }
		break;

	default:
		break;
	}

	return address + 1;
}

// Gets the total number of commands in a given script
int InputScript::EvaluateScript(std::string script)
{
	int result = 0;
	int open_parenthesis_count = 0;

	// Counts the # of ;'s in the string, skips content in parenthesis (for statements)
	for (int i = 0; i < script.length(); i++)
	{
		if (script[i] == '(') { open_parenthesis_count++; }
		if (script[i] == ')') { open_parenthesis_count--; }
		if ((script[i] == ';') && (open_parenthesis_count == 0)) { result++; }

		if (open_parenthesis_count < 0) { /* Throw error here, "Invalid code, too many closing parenthesis at line ." + result */ }
	}

	if (open_parenthesis_count > 0) { /* Throw error here, "Invalid code, not enough closing parenthesis detected."*/ }
	return result;
}

// apply "cheats" to script for easier compilation
void InputScript::ModifyScript(std::string &script)
{
	string_functions.SearchAndReplace(script, "&&", "~A~");
	string_functions.SearchAndReplace(script, "||", "~O~");
	string_functions.SearchAndReplace(script, "^", "~X~");

	string_functions.SearchAndReplace(script, "|", "~Q~");
	string_functions.SearchAndReplace(script, "&", "~W~");

	script = string_functions.RemoveSpaces(script);

	if (script == "") { return; }

	string_functions.SearchAndReplace(script, "==", "~E~");
	string_functions.SearchAndReplace(script, "!=", "~D~");
	string_functions.SearchAndReplace(script, ">=", "~S~");
	string_functions.SearchAndReplace(script, "<=", "~M~");
	string_functions.SearchAndReplace(script, ">", "~L~");
	string_functions.SearchAndReplace(script, "<", "~P~");

	string_functions.SearchAndReplace(script, "else{", "else;{");
	string_functions.SearchAndReplace(script, "do{", "do;{");

	// Add a semicolon after every bracket. } -> };
	for (int i = 0; i < script.length(); i++) {
		if (script[i] == '{') { script.insert(i+ 1, ";"); }
		if (script[i] == '}') { script.insert(i + 1, ";"); }
	}

	// Add a semicolon after certain commands ("if() {}" -> if(); {})
	ApplyParenthesisSemicolonCheat(script, "if");
	ApplyParenthesisSemicolonCheat(script, "for");
	ApplyParenthesisSemicolonCheat(script, "while");

	loaded_script = script;
}

bool InputScript::SetInstructions(std::string script) {
	while (script != "") {
		instruction.push_back(GetInstructionFromLine(GetNextLineInScript(script)));
	}

	return true;
}

// This will take a single line of code and convert it into an instruction.
Instruction InputScript::GetInstructionFromLine(std::string line) {
	Instruction result;

	std::string word = string_functions.GetWord(line, 0, true);
	std::string c_word = string_functions.Capitalize(word);

	//std::cout << "line - " << line << "\n";

	if (c_word == "IF") {
		result.instruction_type = if_statement;
		result.math_string[0] = string_functions.getParenthesis(line, 0);
		return result;
	}

	if (c_word == "FOR") {
		result.instruction_type = for_loop;
		std::string for_funcs = string_functions.getParenthesis(line, 0);
		// Throw error here if {}'s are detected in the for commands.

		// (this part;;)
		embedded_functions.push_back(GetInstructionFromLine(GetNextLineInScript(for_funcs)));
		result.embedded_function_reference[0] = embedded_functions.size() - 1;
		
		// (;this part;)
		result.math_string[0] = GetNextLineInScript(for_funcs);

		// (;;this part)
		embedded_functions.push_back(GetInstructionFromLine(GetNextLineInScript(for_funcs)));
		result.embedded_function_reference[1] = embedded_functions.size() - 1;

		return result;
	}

	if (c_word == "WHILE") {
		result.instruction_type = while_statement;
		result.math_string[0] = string_functions.getParenthesis(line, 0);
		return result;
	}

	if (c_word == "ELSE") {
		result.instruction_type = else_statement;
		return result;
	}

	if (c_word == "DO") {
		result.instruction_type = do_statement;
		return result;
	}

	if (c_word == "PRINT") {
		result.instruction_type = print_statement;
		result.math_string[0] = string_functions.getParenthesis(line, 0);
		return result;
	}

	if (c_word == "CLS") {
		result.instruction_type = clear_statement;
	}

	// = 
	if (line[word.length()] == '=') {
		result.instruction_type = set_value;
		result.value_to_modify[0] = word.c_str();
		result.math_string[0] = line.replace(0, word.length() + 1, "");
		return result;
	}

	// +=
	if ((line[word.length()] == '+') && (line[word.length() + 1] == '=')) {
		result.instruction_type = inc_value_by;
		result.value_to_modify[0] = word.c_str();
		result.math_string[0] = line.replace(0, word.length() + 2, "");
		return result;
	}

	// -=
	if ((line[word.length()] == '-') && (line[word.length() + 1] == '=')) {
		result.instruction_type = dec_value_by;
		result.value_to_modify[0] = word.c_str();
		result.math_string[0] = line.replace(0, word.length() + 2, "");
		return result;
	}

	// *=
	if ((line[word.length()] == '*') && (line[word.length() + 1] == '=')) {
		result.instruction_type = mul_value_by;
		result.value_to_modify[0] = word.c_str();
		result.math_string[0] = line.replace(0, word.length() + 2, "");
		return result;
	}

	// /=
	if ((line[word.length()] == '/') && (line[word.length() + 1] == '=')) {
		result.instruction_type = div_value_by;
		result.value_to_modify[0] = word.c_str();
		result.math_string[0] = line.replace(0, word.length() + 2, "");
		return result;
	}

	// ++
	if ((line[word.length()] == '+') && (line[word.length() + 1] == '+')) {
		result.instruction_type = inc_value;
		result.value_to_modify[0] = word.c_str();
		return result;
	}

	// --
	if ((line[word.length()] == '-') && (line[word.length() + 1] == '-')) {
		result.instruction_type = dec_value;
		result.value_to_modify[0] = word.c_str();
		return result;
	}

	// {
	if (line == "{") {
		result.instruction_type = opening_bracket;
		return result;
	}

	// }
	if (line == "}") {
		result.instruction_type = closing_bracket;
		return result;
	}

	// Throw error here. "Syntax error in user code at " + line

	return result;
}

// This will return the next line up until a semicolon and remove that line from the script.
std::string InputScript::GetNextLineInScript(std::string &script) {
	int open_parenthesis_count = 0;
	int length = 0;

	for (int i = 0; i < script.length(); i++) {
		if (script[i] == '(') { open_parenthesis_count++; }
		if (script[i] == ')') { open_parenthesis_count--; }

		if ((script[i] == ';') && (open_parenthesis_count == 0)) {
			std::string result = script.substr(0, length);
			script = script.substr(length + 1, script.length()); // + 1 to skip the semi-colon
			return result;
		}

		length++;
	}

	// End of the script was reached, returns what was left without semi-colon.
	std::string result = script.substr(0, length);
	script = script.substr(length, script.length());
	return result;
}

// Add a semicolon after certain commands ("if() {}" -> if(); {})
void InputScript::ApplyParenthesisSemicolonCheat(std::string &script, std::string search_for) {
	std::size_t f = script.find(search_for);
	int insert_at;

	while (f != std::string::npos) {
		insert_at = f + search_for.size() + 2 + string_functions.getParenthesis(script, f).size();
		if (script[insert_at] != ';') {
			script.insert(insert_at, ";");
		}

		f = script.find(search_for, insert_at);
	}
}

std::string InputScript::GetInstructionDebug(vector<Instruction>& inst) {
	std::string result = "";
	for (int i = 0; i < inst.size(); i++) {
		result += "Line " + string_functions.numToString(i) + " instruction_type: " + string_functions.numToString(inst[i].instruction_type) + "\n";
	}
	return result;
}
}