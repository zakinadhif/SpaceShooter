#pragma once

#include <random>

namespace zfge::Random
{

float getFloat();
int   getInt();

float getFloat(float min, float max);
int   getInt(int min, int max);

std::mt19937& getEngine();

}
