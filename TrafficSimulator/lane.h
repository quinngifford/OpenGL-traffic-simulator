#ifndef LANE_H
#define LANE_H

#include "car.h"

class Lane {
public:
    
    int carsThisCycle = 0;
    int light = 2;

    float lightx;
    float lighty;

    float time_left = -1; 

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