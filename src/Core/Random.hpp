#pragma once

#include <SFML/Graphics/Color.hpp>

#include <random>

namespace zfge::Random
{

float getFloat();
int   getInt();

float getFloat(float min, float max);
int   getInt(int min, int max);

bool getBool(double chanceToGetTrue);

sf::Color getColor();

std::mt19937& getEngine();

}
