#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string.h>


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

typedef struct VertexAttribute
{
    GLint vectorSize;
    GLenum type;
    GLboolean normalized;
    int stride;
    const void* firstOffset;
    void* data;
    unsigned long size;
    bool perInstance;

}VertexAttribute;
//

typedef struct Mesh
{
    unsigned int attribCount;
    Program program;
    VertexAttribute* attribs;
    GLuint VAO;
    unsigned int triangleCount;
    unsigned int instances;

}Mesh;


void Init_Mesh(Mesh* mesh)
{
    mesh->attribCount = 0;
    mesh->program = 0;
    mesh->attribs = malloc(sizeof(VertexAttribute)+1);
    mesh->triangleCount = 0;
    mesh->instances = 1;
}

void Add_Attribute(Mesh* mesh, void* data, unsigned long size, GLint vectorSize, GLenum type)
{
    mesh->attribCount ++;
    if(mesh->attribCount > 1){
        VertexAttribute* newAttribs = malloc(sizeof(VertexAttribute) * mesh->attribCount + 0);
        memcpy(newAttribs, mesh->attribs, sizeof(VertexAttribute) * (mesh->attribCount - 1));
        free(mesh->attribs);
        mesh->attribs = newAttribs;
    }
    printf("\nSizeof Type: %i\n",sizeof(type));
    VertexAttribute r;
    r.vectorSize = vectorSize;
    r.type = type;
    r.normalized = false;
    r.stride = vectorSize * sizeof(type);
    r.firstOffset = (void*)0;
    r.data = data;
    r.size = size;
    r.perInstance = false;
    mesh->attribs[mesh->attribCount-1] = r;
}


void Init_VAO(Mesh* mesh)
{
    int currentAttrib = 0;
    GLuint* VBOs = malloc(sizeof(GLuint) * mesh->attribCount);

    glGenVertexArrays(1, &(mesh->VAO));
    glBindVertexArray(mesh->VAO);

    for(int i = 0; i < mesh->attribCount; i++)
    {
        if(mesh->attribs[i].perInstance == false)
            mesh->triangleCount = mesh->attribs[i].size/(sizeof(mesh->attribs[i].type) * mesh->attribs[i].vectorSize);
        glGenBuffers(1, &VBOs[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);

        glBufferData(GL_ARRAY_BUFFER, mesh->attribs[i].size, mesh->attribs[i].data, GL_STATIC_DRAW);
        glVertexAttribPointer(
                              i,
                              mesh->attribs[i].vectorSize,
                              mesh->attribs[i].type,
                              mesh->attribs[i].normalized,
                              mesh->attribs[i].stride,
                              mesh->attribs[i].firstOffset
                              );
        glEnableVertexAttribArray(i);
        if(mesh->attribs[i].perInstance)
            glVertexAttribDivisor(i, 1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        PERROR();
    }
    glBindVertexArray(0);
}

void draw(Mesh* mesh)
{
    glBindVertexArray(mesh->VAO);
    //use this shader program for the next draw calls
    glUseProgram(mesh->program);
    //draw the triangles
    glDrawArraysInstanced(GL_TRIANGLES, 0, mesh->triangleCount * 3, mesh->instances);
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


/*
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
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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

    */
    //
    Mesh mesh;
    Init_Mesh(&mesh);

    Add_Attribute(&mesh, square, sizeof(Vertex) * 6, 3, GL_FLOAT);
    Add_Attribute(&mesh, positions[0], sizeof(float)* 16, 2, GL_FLOAT);
    mesh.instances = 8;
    mesh.program = shaderProgram;
    mesh.attribs[1].perInstance = true;
    Init_VAO(&mesh);

    printf("\nTriangle Count: %u\n",mesh.triangleCount);
    printf("\nAttrib Count: %u\n",mesh.attribCount);

    glfwSwapBuffers(window);

    while(!glfwWindowShouldClose(window))
    {

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
     //use these verticies and attributes for the next draw calls
        glBindVertexArray(mesh.VAO);

        //use this shader program for the next draw calls
        //glUseProgram(shaderProgram);

        draw(&mesh);
        //draw the triangles
        //glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 8);

        //swap the buffers, showing the drawn buffer on the screen
        glfwSwapBuffers(window);

        glfwPollEvents();
        PERROR();
    }
    /*
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    */
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
