#ifndef MESH_H
#define MESH_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string.h>

#include "glerrors.h"
#include "shader.h"
#include "cglm/cglm.h"
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

typedef struct Mesh
{
    unsigned int attribCount;
    Program program;
    VertexAttribute* attribs;
    GLuint VAO;
    unsigned int triangleCount;
    unsigned int instances;

}Mesh;

void Init_Mesh(Mesh* mesh);

void Add_Vertex_VBO(Mesh* mesh, void* data, unsigned long size, GLint vectorSize, GLenum type);

void Add_Instance_VBO(Mesh* mesh, void* data, unsigned long size, GLint vectorSize, GLenum type);

void Build_VAO(Mesh* mesh);

void draw(Mesh* mesh);


#endif
