#pragma once
#include "Object.h"
#include "Bonus.h"

enum class Color {
	PURPLE,
	RED,
	YELLOW,
	GREEN,
	BLUE,
};

enum class BonusType {
	RESIZE_PADDLE,
	CHANGE_BALL_SPEED,
	BOTTOM_HELPER,
	STICKY_PADDLE,
	NUM_OF_TYPES,
	VOID, //when there is no bonus
};

class Block : public Object {
protected:
	const float BLOCK_WIDTH = 89.f;
	const float BLOCK_HEIGHT = 39.f;

	static const int BONUS_HALF_WIDTH = 18;
public:
	BonusType bonusType;

	bool isDeleted = false;
	bool hasBonus = false;

	Block(int curX, int curY);
	void SetBlockTexture(Color color, int helthLevel);
	virtual float CollisionProcessing(Bonus** bonus, BonusType type, Object* object) = 0; //returns the acceleration coefficient
	Bonus* GenerateBonus(BonusType type, Object* object);
};

class Wall : public Block {
public:
	Wall(int curX, int curY) : Block(curX, curY) {};
	float CollisionProcessing(Bonus** bonus, BonusType type, Object* object)override { return 0.f; };
};

class OrdinaryBlock : public Block {
public:
	OrdinaryBlock(int curX, int curY) : Block(curX, curY) {};
	float CollisionProcessing(Bonus** bonus, BonusType type, Object* object)override;
};

class AcceleratingBlock : public OrdinaryBlock {
	const float accelerationCoefficient = 1.1f;
public:
	AcceleratingBlock(int curX, int curY) : OrdinaryBlock(curX, curY) {};
	float CollisionProcessing(Bonus** bonus, BonusType type, Object* object)override;
};

class BlockWithHealth : public Block {
	int maxHelthLevel;
	int curHelthLevel;
public:
	BlockWithHealth(int curX, int curY, int helthLevel) : maxHelthLevel(helthLevel), curHelthLevel(helthLevel), Block(curX, curY) {};
	float CollisionProcessing(Bonus** bonus, BonusType type, Object* object)override;
};