#ifndef CAR_H
#define CAR_H

#include "EBO.h"
#include "VBO.h"
#include "VAO.h"

class Car {
public:
    float throttle = 0;
    float throttleTarget = 0;
    float throttleRate = 0;

    float brake = 0;
    float brakeTarget = 0;
    float brakeRate = 0;

    float steering = 0;
    float steeringTarget = 0;
    float steeringRate = 0;

    float acceleration = 0;
    float velocity = 0;

    float width = 5;
    float height = 4;
    
    float rotation = 0;
    bool turning = false;

    Car* next = nullptr;

    VAO VAOC;
    VBO* VBOC = nullptr;
    EBO* EBOC = nullptr;

    GLfloat carVertices[24] =
    { // |     COORDS      |    |     COLORS     |      
         0.0f,  0.0f,  0.0f,    0.5f, 0.0f,  0.0f, // Bottom Left
         0.0f,  0.0f,  0.0f,    0.5f, 0.3f,  0.0f, // Bottom Right
         0.0f,  0.0f,  0.0f,    0.5f, 0.6f,  0.0f, // Top Right
         0.0f,  0.0f,  0.0f,    0.5f, 0.9f,  0.0f, // Top Left
    };
    GLuint indices[6] =
    {
        0, 1, 2,
        2, 3, 0
    };

    Car(float x, float y); // Constructor declaration


    void setPosition(float x, float y);
    void addToPosition(float x, float y);
    void moveCar(float dt);
    //void turnCar(float dt);
    void addRotation(float angle, int wheel);
    void setSteeringTarget(float target, float rate);
};

#endif // CAR_H