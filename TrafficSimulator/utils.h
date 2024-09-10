#ifndef UTILS_H
#define UTILS_H

#include <random>

extern std::random_device rd;
extern std::mt19937 gen;
extern std::uniform_real_distribution<> dis;
extern std::uniform_real_distribution<> sdis;

double calculateDistance(float x1, float y1, float x2, float y2);

#endif