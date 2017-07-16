#pragma once
#include "stdafx.h"
#include "Input.h"

Input::Input() {}
Input::~Input() {}

void Input::SetInput(const Input &new_input)
{
	A = new_input.A;
	B = new_input.B;
	X = new_input.X;
	Y = new_input.Y;

	Up = new_input.Up;
	Down = new_input.Down;
	Left = new_input.Left;
	Right = new_input.Right;

	R = new_input.R;
	ZR = new_input.ZR;
	L = new_input.L;
	ZL = new_input.ZL;

	Start = new_input.Start;
	Select = new_input.Select;
	Home = new_input.Home;

	LeftStickX = new_input.LeftStickX;
	LeftStickY = new_input.LeftStickY;
	RightStickX = new_input.RightStickX;
	RightStickY = new_input.RightStickY;

	TouchScreenDown = new_input.TouchScreenDown;
	TouchScreenX = new_input.TouchScreenX;
	TouchScreenY = new_input.TouchScreenY;
}

void Input::ClearInput()
{
	A = 0;
	B = 0;
	X = 0;
	Y = 0;

	Up = 0;
	Down = 0;
	Left = 0;
	Right = 0;

	R = 0;
	ZR = 0;
	L = 0;
	ZL = 0;

	Start = 0;
	Select = 0;
	Home = 0;

	LeftStickX = 0;
	LeftStickY = 0;
	RightStickX = 0;
	RightStickY = 0;

	TouchScreenDown = 0;
	TouchScreenX = 0;
	TouchScreenY = 0;
}