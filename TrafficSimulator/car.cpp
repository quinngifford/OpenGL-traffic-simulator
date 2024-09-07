#include <iostream>
#include <cmath>
#include <cstring>
#include "car.h"

#define resistance 0.1f
#define max_throttle 10.0f
#define max_brake 15.0f
#define max_turn 200.0f
//#define max_throttle_rate 0.1f
#define max_steering_rate 0.8f
//#define max_brake_rate 0.4f

#define PI 3.14159265358979323846


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

    // Assume the back wheel is the first vertex
    float pivotX = carVertices[wheel + 0];
    float pivotY = carVertices[wheel + 1];

    for (int i = 0; i < 4; ++i) {
        float x = carVertices[i * 6] - pivotX;
        float y = carVertices[i * 6 + 1] - pivotY;
        carVertices[i * 6] = x * cosTheta - y * sinTheta + pivotX;
        carVertices[i * 6 + 1] = x * sinTheta + y * cosTheta + pivotY;
    }

    VBOC->Update(carVertices, sizeof(carVertices));
}

// Constructor definition
Car::Car(float x, float y) {
    setPosition(x, y);
    VBOC = new VBO(carVertices, sizeof(carVertices));
    EBOC = new EBO(indices, sizeof(indices));
}

void Car::moveCar(float dt) {

    acceleration = throttle * max_throttle - brake * max_brake - (velocity * resistance) - resistance;
    if (velocity < 0) {
        velocity = 0;
    }
    velocity += acceleration * dt;

    float radians = rotation * PI / 180.0f;
    float x = velocity * dt * cos(radians);
    float y = velocity * dt * sin(radians);
    addToPosition(x, y);
    std::cout << velocity << "\n";

    if (steering != 0) {
        if (steering * max_turn * dt > 0) {
            addRotation(steering * max_turn * dt, 18);
        }
        else {
            addRotation(steering * max_turn * dt, 0);
        }
    }
    VBOC->Update(carVertices, sizeof(carVertices));


#pragma region if else mess
    if (turning = true) {
        if (steering < steeringTarget) {
            steering += steeringRate * dt * max_steering_rate;
            if (steering > steeringTarget) {
                steering = steeringTarget;
                turning = false;
            }
        }
        else {
            steering -= steeringRate * dt * max_steering_rate;
            if (steering < steeringTarget) {
                steering = steeringTarget;
                steering = false;
            }
        }
    }
#pragma endregion
}


void Car::setSteeringTarget(float target, float rate) {
    steeringTarget = target;
    steeringRate = rate;
}

//TO MAINTAIN VELOCITY AT 20: VEL = 20, THROTTLE = 2.1
//TO MAINTAIN VELOCITY AT 30: VEL = 30, THROTTLE = 3.1
//AND SO ON