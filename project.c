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

vec2* ENEMYPOSITIONS;
float* ENEMYSIZES;
unsigned int* ENEMYCOLORS;

int ENEMYCOUNT = 0;
const int ENEMYMAX = 2000;

void initPositions()
{
    ENEMYPOSITIONS = malloc(ENEMYMAX * sizeof(vec2) + 1);
    ENEMYCOLORS = malloc(ENEMYMAX * sizeof(unsigned int) + 1);
    ENEMYSIZES = malloc(ENEMYMAX * sizeof(float) + 1);
    for(int i = 0; i < ENEMYMAX; i++)
    {
        vec2 pos;
        pos[0]=sin((float)i) * 100.0;
        pos[1]= 100 * (float)i / (float)ENEMYMAX;
        ENEMYPOSITIONS[i][0] = pos[0];
        ENEMYPOSITIONS[i][1] = pos[1];

        ENEMYCOLORS[i] = 0xFF00FFFF;

        ENEMYSIZES[i] = 3.0;
    }
    ENEMYSIZES[0] = 5.0;
}

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
    float speed = 25.0;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        player->position[1] += speed * frameTime;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        player->position[0] -= speed * frameTime;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        player->position[1] -= speed * frameTime;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        player->position[0] += speed * frameTime;
    }
}

float SQUARE[12] = { -1,-1 , 1,-1 ,-1, 1 , 1,-1 , 1, 1, -1, 1};

void createBackgroundMesh(Mesh* backgroundMesh)
{
    Init_Mesh(backgroundMesh);
    Add_Vertex_VBO(backgroundMesh, SQUARE, sizeof(float) * 12, 2, GL_FLOAT);

    backgroundMesh->instances = 1;

    Program backgroundProgram = createSimpleProgram("Resources/Shaders/background");
    backgroundMesh->program = backgroundProgram;

    Texture backgroundTex = Upload_Texture_2D("/home/hartley/projects/opengl2d/Resources/Textures/frog6.png");

    Build_VAO(backgroundMesh);
    Add_Texture(backgroundMesh,"backgroundTex",backgroundTex);

    printf("\nTriangle Count: %u\n",backgroundMesh->triangleCount);
    printf("\nAttrib Count: %u\n",backgroundMesh->attribCount);

    Add_Uniform(backgroundMesh,"uResolution", IVEC2);
    Add_Uniform(backgroundMesh,"uTime", FLOAT);
}


void createEnemyMesh(Mesh* enemyMesh)
{
     Vertex square[6];

    square[0] = New_Vertex(-1,-1,0);
    square[1] = New_Vertex( 1,-1,0);
    square[2] = New_Vertex(-1, 1,0);
    square[3] = New_Vertex( 1,-1,0);
    square[4] = New_Vertex( 1, 1,0);
    square[5] = New_Vertex(-1, 1,0);

    Program shaderProgram = createSimpleProgram("Resources/Shaders/enemy");

    Init_Mesh(enemyMesh);

    Add_Vertex_VBO(enemyMesh, square, sizeof(Vertex) * 6, 3, GL_FLOAT);
    Add_Instance_VBO(enemyMesh, &ENEMYPOSITIONS[0][0], sizeof(float) * ENEMYMAX * 2, 2, GL_FLOAT);
    Add_Instance_VBO(enemyMesh, ENEMYCOLORS, ENEMYMAX * sizeof(unsigned int), 1, GL_UNSIGNED_INT);
    Add_Instance_VBO(enemyMesh, ENEMYSIZES, ENEMYMAX * sizeof(float), 1, GL_FLOAT);
    enemyMesh->instances = ENEMYMAX;
    enemyMesh->program = shaderProgram;

    Texture frogTex = Upload_Texture_2D("/home/hartley/projects/opengl2d/Resources/Textures/frog4.png");


    Add_Texture(enemyMesh,"frogTex",frogTex);

    Build_VAO(enemyMesh);

    printf("\nTriangle Count: %u\n",enemyMesh->triangleCount);
    printf("\nAttrib Count: %u\n",enemyMesh->attribCount);


    Add_Uniform(enemyMesh,"uResolution", IVEC2);
    Add_Uniform(enemyMesh,"uTime", FLOAT);
}


int main()
{
    Player player;
    player.position[0] = 0;
    player.position[1] = 0;
    player.health = 100;

    initPositions();


    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    //PERROR();

    printf("back to main\n");
    GLFWwindow* window = glfwCreateWindow(800, 600, "Simple game project", NULL, NULL);
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

    Mesh enemyMesh;
    createEnemyMesh(&enemyMesh);
    Mesh backgroundMesh;
    createBackgroundMesh(&backgroundMesh);


    PERROR();

   // Add_Instance_VBO(&enemyMesh, sizes, 8 * sizeof(float), 1, GL_FLOAT);
//#define PRINTFPS

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
        Update_Uniform(&enemyMesh,"uResolution", uResolution);
        Update_Uniform(&enemyMesh,"uTime", &time);

        Update_Uniform(&backgroundMesh,"uResolution", uResolution);
        Update_Uniform(&backgroundMesh,"uTime", &time);

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        processInput(window,&player,time-lastTime);

        //return 4;

        ENEMYPOSITIONS[0][0] = player.position[0];
        ENEMYPOSITIONS[0][1] = player.position[1];
        for(int i = 1; i < ENEMYMAX; i++)
        {
            float speed = sin((float)i / 10.0) * 0.5 + 0.5 + 1.0;

            ENEMYPOSITIONS[i][0] += glm_sign(-ENEMYPOSITIONS[i][0] + player.position[0]) * (time-lastTime) * 5 * speed;
            ENEMYPOSITIONS[i][1] += glm_sign(-ENEMYPOSITIONS[i][1] + player.position[1]) * (time-lastTime) * 5*speed;

            if(glm_vec2_distance(ENEMYPOSITIONS[i],player.position) < 5.0)
            {
                ENEMYPOSITIONS[i][0] = sin((float)i) * 300.0;
                ENEMYPOSITIONS[i][1] = (300 * (float)i) / (float)ENEMYMAX - 150;
            }
        }

        Update_VBO(&enemyMesh, 1, ENEMYPOSITIONS);
        Draw(&backgroundMesh);
        Draw(&enemyMesh);
        //swap the buffers, showing the drawn buffer on the screen
        glfwSwapBuffers(window);

        glfwPollEvents();
        fps += (time-lastTime) * 1000.0;
        if(floor(time) != floor(lastTime))
        {
#ifdef PRINTFPS
            printf("%f\n",fps / ((float)passedFrames));
#endif
            printf("pos: %f\n",ENEMYPOSITIONS[1][0]);
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
    //glDeleteProgram(shaderProgram);
    //glDeleteProgram(backgroundProgram);
    Delete_Mesh(&enemyMesh);
    Delete_Mesh(&backgroundMesh);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
