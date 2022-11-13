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

typedef enum {
    CHAR,
    INT,
    IVEC2,
    UINT,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    MAT2,
    MAT3,
    MAT4
}TYPE;

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
    unsigned int uniformCount;
    unsigned int triangleCount;
    unsigned int instances;
    Program program;
    VertexAttribute* attribs;
    GLint* uniformLocations;
    const char** uniformNames;
    TYPE* uniformTypes;
    GLuint VAO;

}Mesh;

void Init_Mesh(Mesh* mesh);

void Add_Vertex_VBO(Mesh* mesh, void* data, unsigned long size, GLint vectorSize, GLenum type);

void Add_Instance_VBO(Mesh* mesh, void* data, unsigned long size, GLint vectorSize, GLenum type);

void Add_Uniform(Mesh* mesh, const char* name, TYPE type);

void Update_Uniform(Mesh* mesh, const char* name, void* data);

void Build_VAO(Mesh* mesh);

void Draw(Mesh* mesh);

void Delete_Mesh(Mesh* mesh);


#endif
