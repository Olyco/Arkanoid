#pragma once
#include "Live.h"
#include <vector>

class LiveBar {
	static const int NUM_OF_LIVES = 4;
	static const int LIVES_INDENT = 10;
	static const int LIVE_WIDTH = 36;
	static const int LIVE_HEIGHT = 33;

	std::vector<Live*> lives{ NUM_OF_LIVES, nullptr };
public:
	int curLivesNum = NUM_OF_LIVES;

	LiveBar(int curX, int curY);
	~LiveBar();

	bool ProcessBallDrop();
	void DisplayLiveBar(sf::RenderWindow& window) const;
};