#include "Block.h"

void Block::SetBlockTexture(Color color, int helthNum) {
	setTextureRect(sf::IntRect(static_cast<int>(color) * BLOCK_WIDTH, helthNum * BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT));
}

Block::Block(int curX, int curY): Object(curX, curY) {
	setSize(sf::Vector2f(BLOCK_WIDTH, BLOCK_HEIGHT));
	texture.loadFromFile("blocks.png");
	setTexture(&texture);

	bonusType = BonusType::VOID;
}

float OrdinaryBlock::CollisionProcessing(BonusType type, Object* object) {
	isDeleted = true;
	return 0.f;
}

float BlockWithHealth::CollisionProcessing(BonusType type, Object* object) {
	--curHelthLevel;

	if (curHelthLevel > 0)
		setTextureRect(sf::IntRect(getTextureRect().left, getTextureRect().top - BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT));
	else
		isDeleted = true;
	return 0.f;
}

float AcceleratingBlock::CollisionProcessing(BonusType type, Object* object) {
	isDeleted = true;
	return accelerationCoefficient;
}
