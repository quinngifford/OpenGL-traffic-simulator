#ifndef UTILS_H
#define UTILS_H

#include <random>

extern std::random_device rd;
extern std::mt19937 gen;
extern std::uniform_real_distribution<> dis;

#endif