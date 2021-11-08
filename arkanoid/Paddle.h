#pragma once
#include "Object.h"

enum class Direction {
	PAUSE,
	LEFT,
	RIGHT,
};

class Paddle : public Object {
	static const int PADDLE_WIDTH = 130;
	static const int PADDLE_HEIGHT = 24;
	const float SPEED_MODUL = 0.5f;

	void ProcessBordersCollision(int borderWidth, int windowWidth);
public:
	Direction direction = Direction::PAUSE;
	bool isSticky;

	Paddle(int windowWidth, int topIndent);
	void UpdatePosition(int borderWidth, int windowWidth, float time);
};