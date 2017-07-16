 #pragma once
#include "stdafx.h"
#include "Input.h"
#include "math_parser.h"
#include "instruction.h"
#include "string_manipulation.h"
#include <string>
#include <vector>
#include <algorithm>

// Testing
#include <chrono>
#include <thread>

// Temporary debug output
#include <iostream>
#include "windows.h"

namespace InputScripts {
class InputScript
{
public:
	InputScript();
	~InputScript();

	// Will load and convert code to simplified instructions.
	bool LoadScript(std::string);

	// Will execute a frame and store the new input in Result.
	bool ExecuteFrame(Input);

	// Executes a function, returns what function to execute next.
	int ExecuteInstruction(Instruction &, int);

	std::string text_output = "";
	bool text_output_enabled = true;

	Input result;
private:
	int EvaluateScript(std::string);
	void ModifyScript(std::string&);
	bool SetInstructions(std::string);
	std::vector<Instruction> instruction;
	std::vector<Instruction> embedded_functions;
	MathParser math_parser;
	StringManipulation string_functions;
	Instruction GetInstructionFromLine(std::string);
	std::string GetNextLineInScript(std::string&);
	void ApplyParenthesisSemicolonCheat(std::string&, std::string);

	std::string loaded_script;

	std::string GetInstructionDebug(vector<Instruction>& inst);

	static const int scope_limit = 256;						// How many nested {}'s are allowed
	int current_scope = 0;									// The current scope level
	int return_address[scope_limit];						// Line to return to at }
	InstructionType scope_instruction_type[scope_limit];	// for, while, do...
	int initial_function_reference[scope_limit];			// Reserved for "for (this part;;)" (references index in embedded_functions)
	int recursive_function_reference[scope_limit];			// Reserved for "for (;;this part)" (references index in embedded_functions)
	int skip_next_scope_level = 0;
	bool skip_next_scope = false;
};
}