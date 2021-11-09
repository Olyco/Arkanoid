#pragma once
#include "Object.h"

class Live : public Object {
public:
	Live(int curX, int curY, int width, int height);

	void LoseLive();
};