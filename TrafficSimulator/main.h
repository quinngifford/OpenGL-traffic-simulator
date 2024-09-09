#ifndef MAIN_H
#define MAIN_H

#include "car.h"


enum LaneID {
    EAST_LEFT_LANE,
    EAST_RIGHT_LANE,
    NORTH_LEFT_LANE,
    NORTH_RIGHT_LANE,
    WEST_LEFT_LANE,
    WEST_RIGHT_LANE,
    SOUTH_LEFT_LANE,
    SOUTH_RIGHT_LANE
};

extern const float spawnPoints[8][2];
extern GLfloat verticesRoadVert[];
extern GLfloat verticesRoadHor[];
extern GLuint indicesRectangle[];



Car createCar(int spawnPoint);

#endif
