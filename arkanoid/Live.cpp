#include "Live.h"

Live::Live(int curX, int curY, int width, int height) : Object(curX, curY) {
    setSize(sf::Vector2f(width, height));
    texture.loadFromFile("live.png");
    setTexture(&texture);
    setTextureRect(sf::IntRect(0, 0, width, height));
}

void Live::LoseLive() {
    setTextureRect(sf::IntRect(getSize().x, 0, getSize().x, getSize().y));
}
