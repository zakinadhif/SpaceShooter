#pragma once

#include <random>

namespace zfge::Random
{

float getFloat();
int   getInt();

std::mt19937& getEngine();

}
