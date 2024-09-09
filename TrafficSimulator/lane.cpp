#include <iostream>
#include <cmath>
#include <cstring>
#include "car.h"
#include "lane.h"
#include "main.h"
#include "utils.h"

Lane::Lane(int id_) : id(id_) {}

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

bool Lane::checkIntersectionEntry(Car* tarCar) {
    if (abs(spawnPoints[id][0] - tarCar->carVertices[0]) > 90 || abs(spawnPoints[id][1] - tarCar->carVertices[1]) > 93) {
        if (id % 2 == 0) {
            tarCar->turnLeft();
        }
        else {
            double chance = dis(gen);
            if (chance > 0.5) {
                tarCar->turnRight();
            }
        }
        return true;
    }
    return false;
}

bool Lane::checkSpawnGap() {
    if (abs(spawnPoints[id][0] - backCar->carVertices[0]) > 30 || abs(spawnPoints[id][1] - backCar->carVertices[1]) > 30) {
        return true;
    }
    return false;
}


void Lane::deleteFrontCar() {
    Car* temp = frontCar;
    frontCar = frontCar->next;
    delete temp;
}

bool Lane::checkFrontBounds() {
    switch (id)
    {
    case EAST_LEFT_LANE:
        if (frontCar->carVertices[1] > 100) {
            deleteFrontCar();
            return true;
        }
        return false;
    case EAST_RIGHT_LANE:
        if (frontCar->carVertices[0] > 100 || frontCar->carVertices[1] < -100) {
            deleteFrontCar();
            return true;
        }
        return false;
    case SOUTH_LEFT_LANE:
        if (frontCar->carVertices[0] > 100) {
            deleteFrontCar();
            return true;
        }
        return false;
    case SOUTH_RIGHT_LANE:
        if (frontCar->carVertices[1] < -100 || frontCar->carVertices[0] < -100) {
            deleteFrontCar();
            return true;
        }
        return false;
    case WEST_LEFT_LANE:
        if (frontCar->carVertices[1] < -100) {
            deleteFrontCar();
            return true;
        }
        return false;
    case WEST_RIGHT_LANE:
        if (frontCar->carVertices[0] < -100 || frontCar->carVertices[1] > 100) {
            deleteFrontCar();
            return true;
        }
        return false;
    case NORTH_LEFT_LANE:
        if (frontCar->carVertices[0] < -100) {
            deleteFrontCar();
            return true;
        }
        return false;
    case NORTH_RIGHT_LANE:
        if (frontCar->carVertices[1] > 100 || frontCar->carVertices[0] > 100) {
            deleteFrontCar();
            return true;
        }
        return false;
    default:
        return false;
    }
}