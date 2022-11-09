#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "shader.h"
#include "cglm/cglm.h"
//#include "vmath.h"
//next tutorial: 4

int WINDOWHEIGHT = 600;
int WINDOWWIDTH = 800;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    WINDOWHEIGHT = height;
    WINDOWWIDTH = width;
}  

typedef struct Vertex
{
    float x;
    float y;
    float z;
}Vertex;

Vertex New_Vertex(float x, float y, float z)
{
    struct Vertex a;
    a.x = x;
    a.y = y;
    a.z = z;
    return a;
}

unsigned int TOTAL_ERRORS = 0;

void PERROR()
{
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        // Process/log the error.
        printf("OPENGL ERROR: %i\n", err);
        TOTAL_ERRORS++;
        if(TOTAL_ERRORS > 10)
            exit(6);
    }
}

typedef struct Player
{
    vec2 position;
    int health;
}Player;



int main()
{
    Player player;
    player.position[0] = 0;
    player.position[1] = 0;
    player.health = 100;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Vertex square[6];

    square[0] = New_Vertex(-1,-1,0);
    square[1] = New_Vertex( 1,-1,0);
    square[2] = New_Vertex(-1, 1,0);

    square[3] = New_Vertex( 1,-1,0);
    square[4] = New_Vertex( 1, 1,0);
    square[5] = New_Vertex(-1, 1,0);
    vec2 positions[8];

    for(int i = 0; i < 8; i++)
    {
        positions[i][0] = i;
        positions[i][1] = i;
    }


    Vertex vertices[6];

    vertices[0] = New_Vertex(-0.5f, -0.5f * sqrt(3.0f) / 3.0f, 0.0f);
    vertices[1] = New_Vertex(0.5f, -0.5f * sqrt(3) / 3, 0.0f);
    vertices[2] = New_Vertex(0.0f, 0.5f * sqrt(3) * 2 / 3, 0.0f);

    vertices[3] = New_Vertex(-0.5f / 2, 0.5f * sqrt(3) / 6, 0.0f);
    vertices[4] = New_Vertex(0.5f / 2, 0.5f * sqrt(3) / 6, 0.0f);
    vertices[5] = New_Vertex(0.0f, -0.5f * sqrt(3) / 3, 0.0f);


    GLuint indices[] =
    {
        0, 3, 5, // Lower left triangle
        3, 2, 4, // Lower right triangle
        5, 4, 1 // Upper triangle
    };

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        printf("failed to create window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("failed to init GLAD\n");
        return -1;
    }

    //create a viewport in the window to draw into
    //and set a callback so that the viewport resolution updates with window resizes
    glViewport(0,0,800,600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Program shaderProgram = createSimpleProgram("Resources/Shaders/default");



    //create references to GPU objects
    GLuint VAO, VBO, EBO;

    //create a vertex array and vertex buffer on the gpu
    //and pass a reference to them to the corresponding variables
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //set this VAO as the active VAO
    glBindVertexArray(VAO);

    //set this VBO as the active VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //pass the vertices to the GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

   // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //tell the gpu:
    //1. which vertex attribute slot the data from the currently bound VBO goes in
    //2. how many values per vertex
    //3. what datatype they are in
    //4.
    //5. amount of data between each vertex
    //6. pointer to the start of the values in the array
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    GLuint positionData;
    glGenBuffers(1, &positionData);
    glBindBuffer(GL_ARRAY_BUFFER, positionData);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, positions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

    //unbind the VAO and VBO for error checking, incase we accidentally call
    //a function that modifes the currently bound VAO or VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glfwSwapBuffers(window);

    int checked = 0;
    //checked ++;
    //checked --;
    while(!glfwWindowShouldClose(window))
    {



        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        PERROR();

        //use this shader program for the next draw calls
        glUseProgram(shaderProgram);

        //use these verticies and attributes for the next draw calls
        glBindVertexArray(VAO);

        //draw the triangles
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 4);

        //swap the buffers, showing the drawn buffer on the screen
        glfwSwapBuffers(window);

        glfwPollEvents();

        checked = 1;
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
