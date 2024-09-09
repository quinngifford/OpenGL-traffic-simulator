#ifndef LANE_H
#define LANE_H

#include "car.h"

class Lane {
public:
    enum Light {
        Green,
        Yellow,
        Red
    };

    int id;

    Car* backCar = nullptr;
    Car* frontCar = nullptr;


    Lane(int id_);

    Car* addBackCar();
    bool checkFrontBounds();
    void deleteFrontCar();
    bool checkIntersectionEntry(Car* tarCar);
    bool checkSpawnGap();
    //float distanceFromLight(Car * tarCar);

    //Later: create lines which the car will follow, perhaps using mathematical functions
};

#endif