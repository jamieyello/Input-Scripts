#pragma once
#include "stdafx.h"

// This is just for proof of concept data type.

// All fields are double as they are directly referenced by the math parser (which only uses doubles)

class Input
{
public:
	Input();
	~Input();

	double A = 0;
	double B = 0;
	double X = 0;
	double Y = 0;

	double Up = 0;
	double Down = 0;
	double Left = 0;
	double Right = 0;

	double R = 0;
	double ZR = 0;
	double L = 0;
	double ZL = 0;

	double Start = 0;
	double Select = 0;
	double Home = 0;

	double LeftStickX = 0;
	double LeftStickY = 0;
	double RightStickX = 0;
	double RightStickY = 0;

	double TouchScreenDown = 0;
	double TouchScreenX = 0;
	double TouchScreenY = 0;

	void SetInput(const Input&);
	void ClearInput();
private:
};