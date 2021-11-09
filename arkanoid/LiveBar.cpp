#include "LiveBar.h"

LiveBar::LiveBar(int curX, int curY) {
	for (int i = 0; i < NUM_OF_LIVES; ++i)
		lives[i] = new Live(curX + i * (LIVE_WIDTH + LIVES_INDENT), curY, LIVE_WIDTH, LIVE_HEIGHT);
}

LiveBar::~LiveBar() {
	for (int i = 0; i < NUM_OF_LIVES; ++i)
		delete lives[i];
}

bool LiveBar::ProcessBallDrop() {
	--curLivesNum;
	lives[curLivesNum]->LoseLive();

	if (curLivesNum > 0)
		return true;

	return false;
}

void LiveBar::DisplayLiveBar(sf::RenderWindow& window) const {
	for (int i = 0; i < NUM_OF_LIVES; ++i)
		window.draw(*lives[i]);
}