#include "mesh.h"


void Init_Mesh(Mesh* mesh)
{
    mesh->attribCount = 0;
    mesh->program = 0;
    mesh->attribs = malloc(sizeof(VertexAttribute)+1);
    mesh->triangleCount = 0;
    mesh->instances = 1;
}

void Add_Vertex_VBO(Mesh* mesh, void* data, unsigned long size, GLint vectorSize, GLenum type)
{
    mesh->attribCount ++;
    if(mesh->attribCount > 1){
        VertexAttribute* newAttribs = malloc(sizeof(VertexAttribute) * mesh->attribCount + 0);
        memcpy(newAttribs, mesh->attribs, sizeof(VertexAttribute) * (mesh->attribCount - 1));
        free(mesh->attribs);
        mesh->attribs = newAttribs;
    }
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

void Add_Instance_VBO(Mesh* mesh, void* data, unsigned long size, GLint vectorSize, GLenum type)
{
    Add_Vertex_VBO(mesh, data, size, vectorSize, type);
    mesh->attribs[mesh->attribCount-1].perInstance = true;
}

void Build_VAO(Mesh* mesh)
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
