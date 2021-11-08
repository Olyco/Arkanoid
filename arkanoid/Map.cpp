#include "Map.h"
#include "Game.h"
#include <iostream>

Map::Map(int leftIndent, int topIndent) {
	FillMap(leftIndent, topIndent);
}

Map::~Map() {
	DeleteMap();
}

void Map::DeleteMap() {
	for (int i = 0; i < NUM_OF_ROWS; ++i) {
		for (int j = 0; j < NUM_OF_COLUMNS; ++j)
			delete blocksMap[i][j];
	}
}

void Map::FillMap(int leftIndent, int topIndent) {
	int curX;
	int curY = topIndent;
	int count1 = 0, count2 = 0;
	for (int i = 0; i < NUM_OF_ROWS; ++i) {
		curX = leftIndent;
		for (int j = 0; j < NUM_OF_COLUMNS; ++j) {
			Block* block;
			int chanceOfBonus = rand() % 100;
			switch (layout[i][j].helthNum) {
			case HelthNum::INF:
				block = new Wall(curX, curY);
				break;
			case HelthNum::ONE:{ //50/50 accelerating block
				int chanceOfAccelerating = rand() % 100;

				if (chanceOfAccelerating >= 80) {
					block = new AcceleratingBlock(curX, curY);
					++count1;
				}
				else {
					block = new OrdinaryBlock(curX, curY);

					if (chanceOfBonus >= 60) {
						block->hasBonus = true;
						++count2;
					}
				}

				break;
			}
			default:
				block = new BlockWithHealth(curX, curY, static_cast<int>(layout[i][j].helthNum));

				if (chanceOfBonus >= 60) {
					block->hasBonus = true;
					++count2;
				}
				break;
			}
			block->SetBlockTexture(layout[i][j].color, static_cast<int>(layout[i][j].helthNum));

			blocksMap[i][j] = block;
			curX += BLOCK_WIDTH + HORIZONTAL_INTERVAL;
		}
		curY += BLOCK_HEIGHT + VERTICAL_INTERVAL;
	}
	std::cout << count1 << std::endl;
	std::cout << count2 << std::endl;

}

sf::FloatRect Map::GetRectangle() const {
	float width = (BLOCK_WIDTH + HORIZONTAL_INTERVAL) * NUM_OF_COLUMNS - HORIZONTAL_INTERVAL;
	float height = (BLOCK_HEIGHT + VERTICAL_INTERVAL) * NUM_OF_ROWS - VERTICAL_INTERVAL;
	sf::FloatRect blocksMapRect(this->blocksMap[0][0]->GetLeftBorder(), this->blocksMap[0][0]->GetTopBorder(), width, height);
	return blocksMapRect;
}

sf::Vector2i Map::GetMapSize()const {
	sf::Vector2i mapSize(NUM_OF_COLUMNS, NUM_OF_ROWS);
	return mapSize;
}

void Map::DisplayMap(sf::RenderWindow& window) {
	for (int i = 0; i < NUM_OF_ROWS; ++i) {
		for (int j = 0; j < NUM_OF_COLUMNS; ++j) {
			if(!blocksMap[i][j]->isDeleted)
				window.draw(*blocksMap[i][j]);
		}
	}
}