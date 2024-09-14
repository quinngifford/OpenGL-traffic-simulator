#include <iostream>
#include <cmath>
#include <cstring>
#include "car.h"
#include "utils.h"
#include "main.h"
#include "lane.h"

#define PI 3.14159265358979323846
#define resistance 0.1f
#define max_throttle 10.0f
#define max_brake 40.0f
#define max_turn 300.0f
#define stop_gap 10.0f

#define slowdown_distance_car 20.0f
#define slowdown_distance_light 60.0f

#define acceleration_rate 0.3f




void Car::setPosition(float x, float y) {
    carVertices[0] = x; carVertices[1] = y;
    carVertices[6] = x + 5; carVertices[7] = y;
    carVertices[12] = x + 5; carVertices[13] = y + 4;
    carVertices[18] = x; carVertices[19] = y + 4;
}

void Car::addToPosition(float x, float y) {
    carVertices[0] += x; carVertices[1] += y;
    carVertices[6] += x; carVertices[7] += y;
    carVertices[12] += x; carVertices[13] += y;
    carVertices[18] += x; carVertices[19] += y;
}

void Car::addRotation(float angle, int wheel) {
    rotation += angle;
    
    float radians = angle * PI / 180.0f;
    float cosTheta = cos(radians);
    float sinTheta = sin(radians);

    float pivotX = carVertices[wheel];
    float pivotY = carVertices[wheel + 1];

    for (int i = 0; i < 4; ++i) {
        float x = carVertices[i * 6] - pivotX;
        float y = carVertices[i * 6 + 1] - pivotY;
        carVertices[i * 6] = x * cosTheta - y * sinTheta + pivotX;
        carVertices[i * 6 + 1] = x * sinTheta + y * cosTheta + pivotY;
    }

    VBOC->Update(carVertices, sizeof(carVertices));
}

Car::Car(float x, float y, int id) : lane(id) {
    setPosition(x, y);
    VBOC = new VBO(carVertices, sizeof(carVertices));
    EBOC = new EBO(indices, sizeof(indices));
}

void Car::moveCar(float dt) {

    //acceleration = throttle * max_throttle - brake * max_brake - (velocity * resistance) - resistance;
    if (velocity < 0) {
        velocity = 0;
    }
    velocity += acceleration * dt;
    
    float radians = rotation * PI / 180.0f;
    float x = velocity * dt * cos(radians);
    float y = velocity * dt * sin(radians);
    addToPosition(x, y);

    VBOC->Update(carVertices, sizeof(carVertices));

    //if we havent enterned intersection
    if (turnStatus == 0) {
        //if light is green
        if (lanes[lane].light == 0) {
            //if car in front
            if (inFront != nullptr) {
                followCar(dt);
            }
        }
        //if light is red
        else if (lanes[lane].light == 2) {
            //if red and car in front
            if (inFront != nullptr) {
                //if red and car in front hasn't entered intersection
                if (inFront->turnStatus == 0) {
                    stopForCar(dt);
                }
                //if red and car in front has entered intersection
                else {
                    stopForLight(dt);
                }
            }
            //if red and no car in front
            else {
                stopForLight(dt);
            }
        }
        //if light is yellow
        else if (lanes[lane].light == 1) {
            //if car in front
            if (inFront != nullptr) {
                //if I'm making the light
                if (makingLight()) {
                    followCar(dt);
                }
            }
            //if no car in front and I'm not making the light
            else if (!makingLight()) {
                stopForLight(dt);
            }
        }
    }
    else {
        if (inFront != nullptr) {
            followCar(dt);
        }
    }
    
    if (turnStatus == 1) {
        if (rotation < rotationTarget) {
            addRotation(rotationRate * dt * max_turn, 18);
            if (rotation >= rotationTarget) {
                rotation = rotationTarget;
                turnStatus = 2;
            }
        }
        else {
            addRotation(-rotationRate * dt * max_turn, 0);
            if (rotation <= rotationTarget) {
                rotation = rotationTarget;
                turnStatus = 2;
            }
        }
    } 
}

void Car::setRotationTarget(float target, float rate) {
    rotationTarget = target;
    rotationRate = rate;
    if (rotationTarget != rotation) {
        turnStatus = 1;
    }
}

void Car::turnLeft() {
    rotationTarget = rotation + 90;
    rotationRate = 0.2f*(1+velocity*0.01);
    turnStatus = 1;
}
void Car::turnRight() {
    rotationTarget = rotation - 90;
    rotationRate = 0.8f*(1+velocity*0.01);
    turnStatus = 1;
}

bool Car::makingLight() {
    float distToLight = calculateDistance(carVertices[0], carVertices[1], lanes[lane].lightx, lanes[lane].lighty);
    float velocityNeeded = distToLight / lanes[lane].time_left;
    if (velocityNeeded > velocity) {
        return false;
    }
    return true;
}

void Car::stopForCar(float dt) {
    int count = 0;
    Car* curr = lanes[lane].frontCar;
    while (curr != this) {
        if (curr->turnStatus == 0) {
            count++;
        }
        curr = curr->next;
    }
    float dist_ = calculateDistance(carVertices[0], carVertices[1], lanes[lane].lightx, lanes[lane].lighty) - stop_gap;
    float dist = dist_ - (count * 10);
    if (dist < 0) {
        return;
    }
    if (dist < 60) {
        acceleration = doubleIntegralMagic(acceleration, velocity, dist);
    }
}
void Car::stopForLight(float dt) {
    float dist = calculateDistance(carVertices[0], carVertices[1], lanes[lane].lightx, lanes[lane].lighty) - stop_gap;
    if (dist < 60) {
        acceleration = doubleIntegralMagic(acceleration, velocity, dist);
    }
    
}
void Car::followCar(float dt) {
    float dist = calculateDistance(carVertices[0], carVertices[1], inFront->carVertices[0], inFront->carVertices[1]) - stop_gap;
    if (dist < 23 && inFront->velocity < velocity) {
        acceleration -= acceleration_rate * dt;
    }
    else {
        if (velocity < targetVelocity) {
            acceleration += acceleration_rate * dt;
        }
        else {
            acceleration = 0;
        }
    }
}



