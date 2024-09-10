#include "utils.h"
#include <iostream>
#include <cmath>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.0, 1.0);
std::uniform_real_distribution<> sdis(0.7, 0.9);

double calculateDistance(float x1, float y1, float x2, float y2) {
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

//get the acceleration needed to stop at the dist value
float doubleIntegralMagic(float a, float v, float d) {
    if (a >= 0) {
        a = -0.1;
    }
    float x = -v / a;
    
    while (((0.5 * a * (x * x)) + (v * x)) > d) {
        a -= 0.01;
        x = -v / a;
    }
    return a;
}