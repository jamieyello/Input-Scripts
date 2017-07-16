#pragma once
#include "stdafx.h"

#include <iostream>

namespace InputScripts {
enum InstructionType // Psuedo OP-code.
{
	null_value,		 // no function
	set_value,		 // value = value;
	inc_value_by,	 // value += value;
	dec_value_by,	 // value -= value;
	mul_value_by,	 // value *= value;
	div_value_by,	 // value /= value;
	inc_value,		 // value++;
	dec_value,		 // value--;
	for_loop,		 // for (value = value; value < value; value++) { code; }
	if_statement,	 // if (value) { code; }
	else_statement,	 // (following 'If') else { code; }
	do_statement,	 // do {} while ();
	while_statement, // while () do {};
	print_statement, // print();
	clear_statement, // cls;
	opening_bracket, // {
	closing_bracket  // }
};

class Instruction
{
public:
	Instruction();

	InstructionType instruction_type = null_value;

	// Below fields may not be used depending on the type of instruction.
	// Used by =, +=, -=, *=, /=, ++, --
	std::string value_to_modify[4];

	// Used by =, +=, -=, *=, /=, ++, --, for, if, do, while
	std::string math_string[4];

	// Used by for
	int embedded_function_reference[4]; // Recursive objects don't work, an int value referencing a position in a separate vector<Instruction> is much safer than a pointer.
private:
};
}