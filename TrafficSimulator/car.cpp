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
#define max_brake 15.0f
#define max_turn 300.0f
#define stop_gap 5.0f

#define braking_distance_car 20.0f
#define braking_distance_light 40.0f
#define slowdown_distance_car 20.0f
#define slowdown_distance_light 60.0f

#define throttle_rate 0.2f
#define brake_rate 0.2f
#define fast_brake_rate 0.4f
#define emergency_brake_rate 1.2f
//#define max_steering_rate 0.8f

//TO MAINTAIN VELOCITY AT 20: VEL = 20, THROTTLE = 2.1
//TO MAINTAIN VELOCITY AT 30: VEL = 30, THROTTLE = 3.1
//AND SO ON


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

// Constructor definition
Car::Car(float x, float y, int id) : lane(id) {
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

    if (velocity > 0) {
        std::cout << "Brake: " << brake << "  Throttle: " << throttle << "  Velocity: " << velocity << "\n";
    }
    
    float radians = rotation * PI / 180.0f;
    float x = velocity * dt * cos(radians);
    float y = velocity * dt * sin(radians);
    addToPosition(x, y);

    VBOC->Update(carVertices, sizeof(carVertices));

    if (turnStatus == 0) {
        if (lanes[lane].light == 0) {
            if (inFront != nullptr) {
                stopForCar(dt);
            }
        }
        else if (lanes[lane].light == 2) {
            if (inFront != nullptr) {
                if (inFront->turnStatus == 0) {
                    stopForCar(dt);
                }
                else {
                    stopForLight(dt);
                }
            }
            else {
                stopForLight(dt);
            }
        }
        else if (lanes[lane].light == 1) {
            if (inFront != nullptr) {
                if (makingLight()) {
                    stopForCar(dt);
                }
            }
            else if (!makingLight()) {
                stopForLight(dt);
            }
        }
    }
    else {
        if (inFront != nullptr) {
            stopForCar(dt);
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
#pragma region Steering Option
    /*
    * if (steering != 0) {
        if (steering > 0) {
            addRotation(steering * max_turn * dt, 18);
        }
        else {
            addRotation(steering * max_turn * dt, 0);
        }
    }

    if (turning = true) {
        if (steering < steeringTarget) {
            steering += steeringRate * dt * max_steering_rate;
            if (steering >= steeringTarget) {
                steering = steeringTarget;
                turning = false;
            }
        }
        else {
            steering -= steeringRate * dt * max_steering_rate;
            if (steering <= steeringTarget) {
                steering = steeringTarget;
                steering = false;
            }
        }
    }
    */
#pragma endregion

    
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
    rotationRate = 0.2f;
    turnStatus = 1;
}
void Car::turnRight() {
    rotationTarget = rotation - 90;
    rotationRate = 0.8f;
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
    float dist = calculateDistance(carVertices[0], carVertices[1], inFront->carVertices[0], inFront->carVertices[1]) - stop_gap;
    
    if (dist + velocity * 0.5 < 5 && velocity > inFront->velocity) {
        brake += emergency_brake_rate * dt;
        throttle = 0;
    }
    if (dist + velocity * 0.5 < 20 && velocity > inFront->velocity) {
        brake += fast_brake_rate * dt;
        throttle = 0;
    }
    if (dist + velocity * 0.5 < 30 && velocity > inFront->velocity) {
        brake += brake_rate * dt;
        throttle = 0;
    }
    if (dist + velocity * 0.5 < 40 && velocity > inFront->velocity) {
        throttle -= throttle_rate * dt;
    }
    else {
        if (throttle < throttleTarget) {
            throttle += throttle_rate * dt;
        }
        if (brake > 0) {
            brake -= emergency_brake_rate * dt;
        }
    }
    if (brake < 0) {
        brake = 0;
    }
    else if (brake > max_brake) {
        brake = max_brake;
    }
    if (throttle < 0) {
        throttle = 0;
    }
    else if (throttle > throttleTarget) {
        throttle = throttleTarget;
    }
    
    
}
void Car::stopForLight(float dt) {
    float dist = calculateDistance(carVertices[0], carVertices[1], lanes[lane].lightx, lanes[lane].lighty) - stop_gap;
    if (dist < slowdown_distance_light + velocity) {
        if (throttle > 0) {
            throttle -= throttle_rate * dt;
        }
        else if (throttle < 0) {
            throttle = 0;
        }
        if (dist < braking_distance_light + velocity) {
            brake += brake_rate * dt;
            if (brake > max_brake) {
                brake = max_brake;
            }
        }
        else {
            brake -= brake_rate * dt;
            if (brake < 0) {
                brake = 0;
            }
        }
    }
}

#pragma region steering function
/*
void Car::setSteeringTarget(float target, float rate) {
    steeringTarget = target;
    steeringRate = rate;
    if (target != steering) {
        turning = true;
    }
}
*/
#pragma endregion




