#pragma once
#include "Block.h"
#include <vector>

using std::vector;

enum class HelthNum{
	INF, //Wall
	ONE,
	TWO,
	THREE,
};

struct BlockOnMap{
	Color color;
	HelthNum helthNum;
};

class Map{
	static const int NUM_OF_ROWS = 8;
	static const int NUM_OF_COLUMNS = 10;
	static const int NUM_OF_WALL_BLOCKS = 6;

	static const int VERTICAL_INTERVAL = 4;
	static const int HORIZONTAL_INTERVAL = 4;

	static const int BLOCK_WIDTH = 89;
	static const int BLOCK_HEIGHT = 39;

	static const int MAP_WIDTH = 1052;

	BlockOnMap layout[NUM_OF_ROWS][NUM_OF_COLUMNS] =
	{
		{{Color::BLUE, HelthNum::ONE}, {Color::GREEN, HelthNum::ONE}, {Color::BLUE, HelthNum::ONE}, {Color::BLUE, HelthNum::TWO}, {Color::RED, HelthNum::THREE}, {Color::RED, HelthNum::THREE}, {Color::BLUE, HelthNum::TWO}, {Color::BLUE, HelthNum::ONE}, {Color::GREEN, HelthNum::ONE}, {Color::BLUE, HelthNum::ONE}},
		{{Color::GREEN, HelthNum::ONE}, {Color::RED, HelthNum::ONE}, {Color::GREEN, HelthNum::ONE}, {Color::BLUE, HelthNum::ONE}, {Color::BLUE, HelthNum::ONE}, {Color::BLUE, HelthNum::ONE}, {Color::BLUE, HelthNum::ONE}, {Color::GREEN, HelthNum::ONE}, {Color::RED, HelthNum::ONE}, {Color::GREEN, HelthNum::ONE}},
		{{Color::RED, HelthNum::ONE}, {Color::YELLOW, HelthNum::ONE}, {Color::RED, HelthNum::TWO}, {Color::GREEN,HelthNum::THREE}, {Color::PURPLE, HelthNum::INF}, {Color::PURPLE, HelthNum::INF}, {Color::GREEN,HelthNum::THREE}, {Color::RED, HelthNum::TWO}, {Color::YELLOW, HelthNum::ONE}, {Color::RED, HelthNum::ONE}},
		{{Color::YELLOW, HelthNum::THREE}, {Color::BLUE, HelthNum::ONE}, {Color::YELLOW, HelthNum::ONE}, {Color::RED, HelthNum::TWO}, {Color::GREEN, HelthNum::THREE}, {Color::GREEN, HelthNum::THREE}, {Color::RED, HelthNum::TWO}, {Color::YELLOW, HelthNum::ONE}, {Color::BLUE, HelthNum::ONE}, {Color::YELLOW, HelthNum::THREE}},
		{{Color::BLUE, HelthNum::ONE}, {Color::BLUE, HelthNum::ONE}, {Color::PURPLE, HelthNum::INF}, {Color::YELLOW, HelthNum::ONE}, {Color::RED, HelthNum::TWO}, {Color::RED, HelthNum::TWO}, {Color::YELLOW, HelthNum::ONE}, {Color::PURPLE, HelthNum::INF}, {Color::BLUE, HelthNum::ONE}, {Color::BLUE, HelthNum::ONE}},
		{{Color::GREEN, HelthNum::THREE}, {Color::BLUE, HelthNum::ONE}, {Color::BLUE, HelthNum::ONE}, {Color::BLUE, HelthNum::ONE}, {Color::YELLOW, HelthNum::ONE}, {Color::YELLOW, HelthNum::ONE}, {Color::BLUE, HelthNum::ONE}, {Color::BLUE, HelthNum::ONE}, {Color::BLUE, HelthNum::ONE}, {Color::GREEN, HelthNum::THREE}},
		{{Color::RED, HelthNum::ONE}, {Color::GREEN, HelthNum::ONE}, {Color::BLUE, HelthNum::ONE}, {Color::BLUE, HelthNum::ONE}, {Color::PURPLE, HelthNum::INF}, {Color::PURPLE, HelthNum::INF}, {Color::BLUE, HelthNum::ONE}, {Color::BLUE, HelthNum::ONE}, {Color::GREEN, HelthNum::ONE}, {Color::RED, HelthNum::ONE}},
		{{Color::YELLOW, HelthNum::TWO}, {Color::RED, HelthNum::ONE}, {Color::GREEN, HelthNum::ONE}, {Color::BLUE, HelthNum::ONE}, {Color::BLUE, HelthNum::TWO}, {Color::BLUE, HelthNum::TWO}, {Color::BLUE, HelthNum::ONE}, {Color::GREEN, HelthNum::ONE}, {Color::RED, HelthNum::ONE}, {Color::YELLOW, HelthNum::TWO}}
	};

	void FillMap(int leftIndent, int topIndent);
	void DeleteMap();
public:
	const int NUM_OF_BLOCKS = NUM_OF_ROWS * NUM_OF_COLUMNS - NUM_OF_WALL_BLOCKS;
	int numOfDeleted = 0;
	vector<vector<Block*>> blocksMap{ NUM_OF_ROWS, vector<Block*>{NUM_OF_COLUMNS, nullptr} };

	Map(int leftIndent, int topIndent);
	~Map();

	sf::FloatRect GetRectangle() const;
	sf::Vector2i GetMapSize()const;

	HelthNum CheckHelthNum(sf::Vector2i& positionOnMap);

	void DisplayMap(sf::RenderWindow& window);
};