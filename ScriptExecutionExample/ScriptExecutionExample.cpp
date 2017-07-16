#include "stdafx.h"
#include "input_script.h"
#include <iostream>

#include <chrono>
#include <thread>
#include <windows.h>
#include "string_manipulation.h"

//void PrintDebug(InputScripts::InputScript&, std::string&);
void PrintInputDebug(std::string&, const std::string[]);
void EmulateInputFromKeyboard(Input&);
std::string* MakeDebug(const double**, const std::string[], const std::string&, bool);
std::string* CombineDebug(const std::string[], const std::string[]);

InputScripts::StringManipulation string_functions;
const int num_of_buttons = 22;

int main() {
	// express individuality/make the windows console flickering less annoying
	system("COLOR F0");

	// Placeholder
	Input input;

	// Load script
	InputScripts::InputScript input_script; // ---
	input_script.LoadScript("TestScript.txt"); // ---

	const std::string c_output[] = { 
		"up",
		"down",
		"left",
		"right",
		"a",
		"b",
		"x",
		"y",
		"l",
		"r",
		"zr",
		"zl",
		"start",
		"select",
		"home",
		"rightStickX",
		"rightStickY",
		"leftStickX",
		"leftStickY",
		"touchDown",
		"touchX",
		"touchY"
	};

	const double* before_values[] = {
		&input.Up,
		&input.Down,
		&input.Left,
		&input.Right,
		&input.A,
		&input.B,
		&input.X,
		&input.Y,
		&input.L,
		&input.R,
		&input.ZR,
		&input.ZL,
		&input.Start,
		&input.Select,
		&input.Home,
		&input.RightStickX,
		&input.RightStickY,
		&input.LeftStickX,
		&input.LeftStickY,
		&input.TouchScreenDown,
		&input.TouchScreenX,
		&input.TouchScreenY
	};

	const double* result_values[] = {
		&input_script.result.Up,
		&input_script.result.Down,
		&input_script.result.Left,
		&input_script.result.Right,
		&input_script.result.A,
		&input_script.result.B,
		&input_script.result.X,
		&input_script.result.Y,
		&input_script.result.L,
		&input_script.result.R,
		&input_script.result.ZR,
		&input_script.result.ZL,
		&input_script.result.Start,
		&input_script.result.Select,
		&input_script.result.Home,
		&input_script.result.RightStickX,
		&input_script.result.RightStickY,
		&input_script.result.LeftStickX,
		&input_script.result.LeftStickY,
		&input_script.result.TouchScreenDown,
		&input_script.result.TouchScreenX,
		&input_script.result.TouchScreenY
	};

	std::string* input_debug_text = new std::string[num_of_buttons];
	std::string debug_text = "";
	std::string previous_debug_text = "";

	// Execution test
	do {
		input.ClearInput();
		EmulateInputFromKeyboard(input);

		// Debug
		previous_debug_text = debug_text;
		debug_text = "";
		input_debug_text = MakeDebug(before_values, c_output, " Before: ", true);

		input_script.ExecuteFrame(input); // ---

		input_debug_text = CombineDebug(input_debug_text, MakeDebug(result_values, c_output, " After: ", false));
		PrintInputDebug(debug_text, input_debug_text);
		debug_text += "\nScript output -\n" + input_script.text_output;

		if (debug_text != previous_debug_text) {
			system("CLS");
			cout << debug_text;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(17));
	} while (true);

	system("PAUSE");
    return 0;
}

// Make that fancy input debug text
std::string* MakeDebug(const double** values, const std::string names[], const std::string &before_or_after, bool add_space) {
	std::string* result = new std::string[num_of_buttons];
	std::string t_result = "";

	for (int i = 0; i < num_of_buttons; i++) {
		t_result = " " + names[i] + before_or_after + string_functions.numToString(*values[i]);
		while (t_result.size() < 23 && add_space) {
			t_result += " ";
		}

		result[i] = t_result;
	}

	return result;
}

std::string* CombineDebug(const std::string first[], const std::string second[]) {
	std::string* result = new std::string[num_of_buttons];

	for (int i = 0; i < num_of_buttons; i++) {
		result[i] = first[i] + second[i];
	}

	return result;
}

void PrintInputDebug(std::string &debug_text, const std::string* input_debug_text) {
	for (int i = 0; i < num_of_buttons; i++) {
		debug_text += input_debug_text[i] + "\n";
	}
}

//void PrintDebug(InputScripts::InputScript& input_script, std::string& debug_text)
//{
//	debug_text += "Number of commands detected = " + string_functions.numToString(input_script.instruction.size()) + "\n";
//	if (input_script.instruction.size() > 0) {
//		int line_number = 0;
//		debug_text += "Function #" + string_functions.numToString(line_number) + " = " + string_functions.numToString(input_script.instruction[line_number].instruction_type) + "\n"
//			+ "Value to modify #0 = " + input_script.instruction[line_number].value_to_modify[0] + "\n"
//			+ "Math string #0 = " + input_script.instruction[line_number].math_string[0] + "\n\n";
//	}
//}

void EmulateInputFromKeyboard(Input& input) {
	if (GetAsyncKeyState('W')) { input.Up = 1; }
	if (GetAsyncKeyState('S')) { input.Down = 1; }
	if (GetAsyncKeyState('A')) { input.Left = 1; }
	if (GetAsyncKeyState('D')) { input.Right = 1; }

	if (GetAsyncKeyState(VK_UP)) { input.X = 1; }
	if (GetAsyncKeyState(VK_DOWN)) { input.B = 1; }
	if (GetAsyncKeyState(VK_LEFT)) { input.Y = 1; }
	if (GetAsyncKeyState(VK_RIGHT)) { input.A = 1; }

	if (GetAsyncKeyState(VK_RETURN)) { input.Start = 1; }
	if (GetAsyncKeyState(VK_RSHIFT)) { input.Select = 1; }
	if (GetAsyncKeyState(VK_SPACE)) { input.Home = 1; }

	if (GetAsyncKeyState('E')) { input.R = 1; }
	if (GetAsyncKeyState('Q')) { input.L = 1; }
	if (GetAsyncKeyState('3')) { input.ZR = 1; }
	if (GetAsyncKeyState('1')) { input.ZL = 1; }
}