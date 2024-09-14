#include <iostream>
#include <cmath>
#include <cstring>
#include "car.h"
#include "lane.h"
#include "main.h"
#include "utils.h"


Lane::Lane(int id_) : id(id_) {
    switch (id) {
        case EAST_LEFT_LANE:
            lightx = -15;
            lighty = -7;
            break;
        case EAST_RIGHT_LANE:
            lightx = -15;
            lighty = -13;
            break;
        case NORTH_LEFT_LANE:
            lightx = 7;
            lighty = -15;
            break;
        case NORTH_RIGHT_LANE:
            lightx = 13;
            lighty = -15;
            break;
        case WEST_LEFT_LANE:
            lightx = 15;
            lighty = 7;
            break;
        case WEST_RIGHT_LANE:
            lightx = 15;
            lighty = 13;
            break;
        case SOUTH_LEFT_LANE:
            lightx = -7;
            lighty = 15;
            break;
        case SOUTH_RIGHT_LANE:
            lightx = -13;
            lighty = 15;
            break;
        default:
            lightx = 0;
            lighty = 0;
            break;
    }
}

Car* Lane::addBackCar() {
    Car* newCar = new Car(createCar(id));
    newCar->velocity = 28 * sdis(gen);
    newCar->targetVelocity = newCar->velocity + 6;
    if (backCar != nullptr) {
        backCar->next = newCar;
        newCar->inFront = backCar;
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
    if (frontCar != nullptr) {
        frontCar->inFront = nullptr;
    }
    delete temp;
}


bool Lane::checkIntersectionEntry(Car* tarCar) {
    if (abs(spawnPoints[id][0] - tarCar->carVertices[0]) > 183 || abs(spawnPoints[id][1] - tarCar->carVertices[1]) > 183) {
        if (id % 2 == 0) {
            tarCar->turnLeft();
        }
        else {
            double chance = dis(gen);
            if (chance > 0.5) {
                tarCar->turnRight();
            }
            else {
                tarCar->turnStatus = 2;
            }
            
        }
        return true;
    }
    return false;
}

bool Lane::checkSpawnGap() {
    if (backCar == nullptr) {
        return true;
    }
    if (abs(spawnPoints[id][0] - backCar->carVertices[0]) > 30 || abs(spawnPoints[id][1] - backCar->carVertices[1]) > 30) {
        return true;
    }
    return false;
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