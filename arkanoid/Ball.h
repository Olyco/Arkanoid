#pragma once
#include "Object.h"
#include "Paddle.h"
#include "Bottom.h"
#include "Block.h"

class Ball :public Object {
	static const int BALL_SIZE = 22;
	const float DEFAULT_PROJ_MODUL = 0.25f;

	const float MAX_PROJ_MODUL = 0.4f;
	const float MIN_PROJ_MODUL = 0.22f;

public:
	bool isOnPaddle;
	bool isActive;
	Ball(int windowWidth, int topIndent);

	void SetDefaultSpeed();
	void UpdateSpeed(sf::Vector2f& newSpeedProjection);

	void UpdatePosition(const Paddle* paddle, float time);
	void FollowPaddle(const Paddle* paddle);

	void ProcessBordersCollision(int borderWidth, int borderHeight, int windowWidth, Bottom* bottom);
	void ProcessPaddleCollision(Paddle* paddle);
	void ProcessBlockCollision(Block* block);

	void AccelerateBall(float accelerationCoefficient);
	bool BallIsActive(int windowHeight);
};