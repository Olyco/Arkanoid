#pragma once
#include "Paddle.h"
#include "Ball.h"
#include "Bottom.h"

enum class TextureIndent{
	ENLARGE_PADDLE,
	REDUCE_PADDLE,
	SPEED_UP_BALL,
	SLOW_DOWN_BALL,
	BOTTOM_HELPER,
	STICKY_PADDLE,
	CHANGE_TRAJECTORY,
	SECOND_BALL,
};

class Bonus : public Object {
protected:
	static const int BONUS_WIDTH = 40;
	static const int BONUS_HEIGHT = 16; 
	const float SPEED_MODUL = 0.2f;
	Object* targetObject;
public:
	bool needToDraw = false;
	bool  isActivated = false;
	bool hasActionTime = false;
	Bonus(int curX, int curY, Object* object);
	virtual void BonusAction() = 0;
	bool UpdatePosition(const Paddle* paddle, int windowHeight, float time); //true if bonus activated
};

class TemporaryBonus : public Bonus {
protected:
	float ACTION_TIME = 7000.f;
	float curActiveTime = 0;

public:
	TemporaryBonus(int curX, int curY, Object* object) :Bonus(curX, curY, object) { hasActionTime = true; };
	void ManageActiveTime(float time);
	virtual void DeactivateBonus() = 0;
};

class ResizePaddle :public TemporaryBonus {
	static const int RESIZE_COEFFICIENT = 30;
	int curCoefficient;
public:
	ResizePaddle(int curX, int curY, Object* object);
	void BonusAction()override;
	void DeactivateBonus();
};

class ChangeBallSpeed : public Bonus {
	const float SPEED_CHANGE_COEFFICIENT = 0.05f;
	float curCoefficient;
public:
	ChangeBallSpeed(int curX, int curY, Object* object);
	void BonusAction()override;
};

class BottomHelper : public TemporaryBonus {
public:
	BottomHelper(int curX, int curY, Object* object);
	void BonusAction()override;
	void DeactivateBonus();
};

class StickyPaddle : public TemporaryBonus {
public:
	StickyPaddle(int curX, int curY, Object* object);
	void BonusAction()override;
	void DeactivateBonus()override;
};

class ChangeBallTrajectory : public Bonus {
public:
	ChangeBallTrajectory(int curX, int curY, Object* object);
	void BonusAction()override;
};

class SecondBall : public Bonus {
public:
	SecondBall(int curX, int curY, Object* object);
	void BonusAction()override;
};
