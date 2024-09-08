#ifndef LANE_H
#define LANE_H

#include <iostream>
#include <cmath>
#include <cstring>
#include "car.h"

class Lane {
public:
    enum Light {
        Green,
        Yellow,
        Red
    };
    int id;
    Car* backCar;
    Car* frontCar;


    Lane(int id_) : id(id_), backCar(nullptr), frontCar(nullptr) {}

    Car* addBackCar();
    void checkFrontBounds();
    void deleteFrontCar();

    //Later: create lines which the car will follow, perhaps using mathematical functions
};

#endif