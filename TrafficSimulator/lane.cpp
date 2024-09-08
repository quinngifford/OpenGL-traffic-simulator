#include <iostream>
#include <cmath>
#include <cstring>
#include "car.h"
#include "lane.h"
#include "main.h"

Lane::Lane(int id_) : id(id_), backCar(nullptr), frontCar(nullptr) {
    
}

Car* Lane::addBackCar() {
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

void Lane::deleteFrontCar() {
    Car* temp = frontCar;
    frontCar = frontCar->next;
    delete temp;
}

void Lane::checkFrontBounds() {
    switch (id)
    {
    case 1:
    case 2:
        if (frontCar->carVertices[1] > 100) {
            deleteFrontCar();
        }
        break;
    case 3:
    case 4:
        if (frontCar->carVertices[0] > 100) {
            deleteFrontCar();
        }
        break;
    case 5:
    case 6:
        if (frontCar->carVertices[1] < -100) {
            deleteFrontCar();
        }
        break;
    case 7:
    case 8:
        if (frontCar->carVertices[0] < -100) {
            deleteFrontCar();
        }
        break;
    default:
        break;
    }
}