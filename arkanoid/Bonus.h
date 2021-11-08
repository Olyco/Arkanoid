#pragma once
#include "Object.h"
#include "Paddle.h"
#include "Ball.h"
#include "Bottom.h"

class Bonus : public Object {
protected:
	static const int BONUS_WIDTH = 36;
	static const int BONUS_HEIGHT = 13;
	const float SPEED_MODUL = 0.2f;
	Object* targetObject;
public:
	bool needToDraw = false;
	bool  isActivated = false;
	bool hasActionTime = false;
	Bonus(int curX, int curY);
	virtual void BonusAction() = 0;
	bool UpdatePosition(const Paddle* paddle, int windowHeight, float time); //true if bonus activated

	//float GetActiveTime()const { return ACTIVE_TIME; };
};

class TemporaryBonus : public Bonus {
protected:
	float ACTION_TIME = 8000.f; //мб разным для разных бонусов?
	float curActiveTime = 0;

public:
	TemporaryBonus(int curX, int curY) :Bonus(curX, curY) { hasActionTime = true; };
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
	int curCoefficient;
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
