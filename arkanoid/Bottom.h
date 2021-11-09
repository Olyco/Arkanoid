#pragma once
#include "Object.h"

class Bottom: public Object {
	static const int BOTTOM_WIDTH = 1012;
	static const int BOTTOM_HEIGHT = 30;
public:
	bool isActive;

	Bottom(int windowWidth, int windowHeight);
};