#include <iostream>
#include <cmath>
#include <cstring>
#include "car.h"
#include "lane.h"
#include "main.h"


Car* Lane::addCarToLane() {
    Car* newCar = new Car(createCar(id));
    if (backCar != nullptr) {
        backCar->next = newCar;
    }
    backCar = newCar;
    if (frontCar == nullptr) {
        frontCar = backCar;
    }
    return newCar;
}