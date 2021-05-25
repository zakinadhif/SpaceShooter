#pragma once

#include <random>

namespace zfge::Random
{

float getFloat();
int   getInt();

int   getInt(int min, int max);

std::mt19937& getEngine();

}
