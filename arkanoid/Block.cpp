#include "Block.h"
#include <iostream>

void Block::SetBlockTexture(Color color, int helthNum) {
	setTextureRect(sf::IntRect(static_cast<int>(color) * BLOCK_WIDTH, helthNum * BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT));
}

Block::Block(int curX, int curY): Object(curX, curY) {
	setSize(sf::Vector2f(BLOCK_WIDTH, BLOCK_HEIGHT));
	texture.loadFromFile("blocks.png");
	setTexture(&texture);

	bonusType = BonusType::VOID;
}

Bonus* Block::GenerateBonus(BonusType type, Object* object) {
	Bonus* bonus = nullptr;

	std::cout << "bonus" << std::endl;

	switch (type) {
	case BonusType::RESIZE_PADDLE:
		bonus = new ResizePaddle(getPosition().x + getSize().x / 2 - BONUS_HALF_WIDTH, getPosition().y + getSize().y / 2, object);
		break;
	case BonusType::CHANGE_BALL_SPEED :
		bonus = new ChangeBallSpeed(getPosition().x + getSize().x / 2 - BONUS_HALF_WIDTH, getPosition().y + getSize().y / 2, object);
		break;
	case BonusType::BOTTOM_HELPER:
		bonus = new BottomHelper(getPosition().x + getSize().x / 2 - BONUS_HALF_WIDTH, getPosition().y + getSize().y / 2, object);
		break;
	case BonusType::STICKY_PADDLE:
		bonus = new StickyPaddle(getPosition().x + getSize().x / 2 - BONUS_HALF_WIDTH, getPosition().y + getSize().y / 2, object);
	}
	
	bonusType = type;
	bonus->needToDraw = true;
	return bonus;
}

float OrdinaryBlock::CollisionProcessing(Bonus** bonus, BonusType type, Object* object) {
	if (hasBonus && !(*bonus))
		*bonus = GenerateBonus(type, object);
	isDeleted = true;
	return 0.f;
}

float BlockWithHealth::CollisionProcessing(Bonus** bonus, BonusType type, Object* object) {
	--curHelthLevel;

	if (curHelthLevel > 0) {
		setTextureRect(sf::IntRect(getTextureRect().left, getTextureRect().top - BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT));
	}
	else {
		if (hasBonus && !(*bonus))
			*bonus = GenerateBonus(type, object);
		isDeleted = true;
	}
	return 0.f;
}

float AcceleratingBlock::CollisionProcessing(Bonus** bonus, BonusType type, Object* object) {
	isDeleted = true;
	return accelerationCoefficient;
}
