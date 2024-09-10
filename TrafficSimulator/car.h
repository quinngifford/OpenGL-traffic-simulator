#ifndef CAR_H
#define CAR_H

#include "EBO.h"
#include "VBO.h"
#include "VAO.h"

class Car {
public:
    float throttle = 0;
    

    float brake = 0;
    bool waitingForLight = false;
    

    //float steering = 0;
    //float steeringTarget = 0;
    //float steeringRate = 0;

    float rotationTarget = 0;
    float rotationRate = 0;

    float acceleration = 0;
    float velocity = 0;

    float width = 5;
    float height = 4;
    
    float rotation = 0;
    int turnStatus = 0;

    int lane;
    Car* next = nullptr;
    Car* inFront = nullptr;

    VAO VAOC;
    VBO* VBOC = nullptr;
    EBO* EBOC = nullptr;

    GLfloat carVertices[24] =
    { // |     COORDS      |    |     COLORS     |      
         0.0f,  0.0f,  0.0f,    0.5f, 0.0f,  0.0f, // Back Right Wheel (Bottom Left Vertice)
         0.0f,  0.0f,  0.0f,    0.5f, 0.3f,  0.0f, // Front Right Wheel (Bottom Right Vertice)
         0.0f,  0.0f,  0.0f,    0.5f, 0.6f,  0.0f, // Front Left Wheel (Top Right Vertice)
         0.0f,  0.0f,  0.0f,    0.5f, 0.9f,  0.0f, // Back Left Wheel (Top Left Vertice)
    };
    GLuint indices[6] =
    {
        0, 1, 2,
        2, 3, 0
    };

    Car(float x, float y, int id);


    void setPosition(float x, float y);
    void addToPosition(float x, float y);
    void moveCar(float dt);
    void addRotation(float angle, int wheel);
    void setRotationTarget(float angle, float rate);
    void turnLeft();
    void turnRight();
    bool makingLight();
    void stopForCar(float dt);
    void stopForLight(float dt);
   // void setSteeringTarget(float target, float rate);
};

#endif // CAR_H