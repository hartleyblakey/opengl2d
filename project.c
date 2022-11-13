#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string.h>


#include "shader.h"
#include "mesh.h"
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




typedef struct Player
{
    vec2 position;
    int health;
}Player;

void processInput(GLFWwindow* window, Player* player, float frameTime)
{
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        player->position[1] += 10.0 * frameTime;
    }
}

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

    positions[0][0] = player.position[0];
    positions[0][1] = player.position[1];

    unsigned int colors[8];

    colors[0] = 0x2FFFFFFF;
    colors[1] = 0xFF2F2FFF;
    colors[2] = 0xFF2F2FFF;
    colors[3] = 0xFF2F2FFF;
    colors[4] = 0xFF2F2FFF;
    colors[5] = 0xFF2F2FFF;
    colors[6] = 0xFF2F2FFF;
    colors[7] = 0xFF2F2FFF;

    float sizes[8] = {5,10,15,20,5,24,2,6};


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

    Mesh mesh;
    Init_Mesh(&mesh);

    Add_Vertex_VBO(&mesh, square, sizeof(Vertex) * 6, 3, GL_FLOAT);
    Add_Instance_VBO(&mesh, positions[0], sizeof(float)* 16, 2, GL_FLOAT);
    Add_Instance_VBO(&mesh, colors, 8 * sizeof(unsigned int), 1, GL_UNSIGNED_INT);
    Add_Instance_VBO(&mesh, sizes, 8 * sizeof(float), 1, GL_FLOAT);
    mesh.instances = 8;
    mesh.program = shaderProgram;
    Build_VAO(&mesh);

    printf("\nTriangle Count: %u\n",mesh.triangleCount);
    printf("\nAttrib Count: %u\n",mesh.attribCount);


    Add_Uniform(&mesh,"uResolution", IVEC2);
    Add_Uniform(&mesh,"uTime", FLOAT);



    glfwSwapBuffers(window);
    float time = glfwGetTime();
    float lastTime = time;
    float fps = 0.0;
    int passedFrames = 0;
    while(!glfwWindowShouldClose(window))
    {
        passedFrames ++;
        lastTime = time;
        time = glfwGetTime();
        int uResolution[2] = {WINDOWWIDTH,WINDOWHEIGHT};
        Update_Uniform(&mesh,"uResolution", uResolution);
        Update_Uniform(&mesh,"uTime", &time);

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        processInput(window,&player,time-lastTime);
        Draw(&mesh);

        //swap the buffers, showing the drawn buffer on the screen
        glfwSwapBuffers(window);

        glfwPollEvents();
        fps += (time-lastTime) * 1000.0;
        if(floor(time) != floor(lastTime))
        {
            printf("%f\n",fps / ((float)passedFrames));
            fps = 0;
            passedFrames = 0;
        }


        PERROR();
    }
    /*
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    */
    glDeleteProgram(shaderProgram);
    Delete_Mesh(&mesh);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
