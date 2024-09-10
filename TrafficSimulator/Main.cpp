#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "main.h"
#include "shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "car.h"
#include "lane.h"
#include "utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <thread>
#include <vector>

#pragma region Constant Definitions
const float spawnPoints[8][2] = {
    {-110,-7}, {-110,-13}, {7,-110}, {13, -110}, {110,7}, {110,13}, {-7, 110}, {-13, 110}
};



GLfloat verticesRoadVert[] =
{ // |     COORDS       |    |     COLORS     |      
    -15.0f, -100.0f, 0.0f,    0.2f, 0.2f,  0.2f, // Bottom Left
     15.0f, -100.0f, 0.0f,    0.2f, 0.2f,  0.2f, // Bottom Right
     15.0f,  100.0f, 0.0f,    0.2f, 0.2f,  0.2f, // Top Right
    -15.0f,  100.0f, 0.0f,    0.2f, 0.2f,  0.2f, // Top Left
};

GLfloat verticesRoadHor[] =
{ // |     COORDS       |    |     COLORS     |      
    -100.0f, -15.0f, 0.0f,    0.2f, 0.2f,  0.2f, // Bottom Left
     100.0f, -15.0f, 0.0f,    0.2f, 0.2f,  0.2f, // Bottom Right
     100.0f,  15.0f, 0.0f,    0.2f, 0.2f,  0.2f, // Top Right
    -100.0f,  15.0f, 0.0f,    0.2f, 0.2f,  0.2f, // Top Left
};


// Indices for rectangle
GLuint indicesRectangle[] =
{
    0, 1, 2,
    2, 3, 0
};

Lane lanes[8] = {
        Lane(EAST_LEFT_LANE), Lane(EAST_RIGHT_LANE),
        Lane(NORTH_LEFT_LANE), Lane(NORTH_RIGHT_LANE),
        Lane(WEST_LEFT_LANE), Lane(WEST_RIGHT_LANE),
        Lane(SOUTH_LEFT_LANE), Lane(SOUTH_RIGHT_LANE),
};

#pragma endregion


Car createCar(int spawnPoint) {
    Car newCar(spawnPoints[spawnPoint][0], spawnPoints[spawnPoint][1], spawnPoint);
    newCar.addRotation((spawnPoint / 2) * 90, 0);
    newCar.VAOC.Bind();
    newCar.VAOC.LinkAttrib(*newCar.VBOC, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    newCar.VAOC.LinkAttrib(*newCar.VBOC, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    newCar.VAOC.Unbind();
    newCar.VBOC->Unbind();
    newCar.EBOC->Unbind();
    return newCar;
}


int main()
{

#pragma region Window Creation
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(1300, 1300, "Traffic Simulation", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glViewport(0, 0, 1300, 1300);
#pragma endregion




    // Generate Shader object using shaders default.vert and default.frag
    Shader shaderProgram("default.vert", "default.frag");
    glm::mat4 projection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, -1.0f, 1.0f);
    // Activate the shader program and set the projection matrix (scaling up to 100x)
    shaderProgram.Activate();
    GLuint projLoc = glGetUniformLocation(shaderProgram.ID, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));




#pragma region Road Configuration
    // Generate Vertex Array Object and bind it
    VAO VAO1;
    VAO1.Bind();
    // Generate Vertex Buffer Object and link it to vertices
    VBO VBO1(verticesRoadVert, sizeof(verticesRoadVert));
    // Generate Element Buffer Object and link it to indices
    EBO EBO1(indicesRectangle, sizeof(indicesRectangle));
    // Link VBO attributes such as coordinates and colors to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    VAO VAO2;
    VAO2.Bind();
    VBO VBO2(verticesRoadHor, sizeof(verticesRoadHor));
    EBO EBO2(indicesRectangle, sizeof(indicesRectangle));
    VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    VAO2.LinkAttrib(VBO2, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO2.Unbind();
    VBO2.Unbind();
    EBO2.Unbind();


#pragma endregion 
    

    
    
    
    
    


    double lastTime = glfwGetTime();
    int nbFrames = 0;
    double time = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
#pragma region FPS counter
        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0) {

            //std::cout << "FPS: " << nbFrames << std::endl;

            nbFrames = 0;
            lastTime += 1.0;
        }
#pragma endregion

#pragma region Clear Background, Activate Program, Redraw Roads
        // Specify the color of the background
        glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Activate the shader program and bind the VAO
        shaderProgram.Activate();

        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        VAO2.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // Draw primitives, number of indices, datatype of indices, index of indices
        
#pragma endregion

        
        
        double dt = glfwGetTime() - time;
        time = glfwGetTime();

#pragma region Iterate over every lane and car
        for (int i = 0; i < 8; i++) {
            
            if (lanes[i].checkSpawnGap()) {
                double chance = dis(gen);
                if (chance > 0.99994) {
                    lanes[i].addBackCar();
                }
            }
            if (lanes[i].frontCar == nullptr) {
                continue;
            }
            lanes[i].checkFrontBounds();
            
            
            Car* curr = lanes[i].frontCar;
            while (curr != nullptr) {
                curr->VAOC.Bind();
                curr->EBOC->Bind();
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                if (curr->turnStatus == 0) {
                    lanes[i].checkIntersectionEntry(curr);
                }
                curr->moveCar(dt);
                curr = curr->next;
            }
        }
#pragma endregion

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

#pragma region Cleanup
    // Delete all the objects we've created
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();

    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();
#pragma endregion
    
    return 0;
}

//test branch!!!!