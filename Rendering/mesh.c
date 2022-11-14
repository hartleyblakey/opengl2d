#include "mesh.h"


void Init_Mesh(Mesh* mesh)
{
    mesh->attribCount = 0;
    mesh->program = 0;
    mesh->attribs = malloc(sizeof(VertexAttribute)+1);
    mesh->triangleCount = 0;
    mesh->instances = 1;

    mesh->textureCount = 0;

    mesh->uniformCount = 0;
    mesh->uniformLocations = malloc(sizeof(GLint)+1);
    mesh->uniformNames = malloc(sizeof(char*)+1);
    mesh->uniformTypes = malloc(sizeof(TYPE)+1);
}

void Add_Vertex_VBO(Mesh* mesh, void* data, unsigned long size, GLint vectorSize, GLenum type)
{
    mesh->attribCount ++;
    mesh->VBOCount++;
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



void Add_Uniform(Mesh* mesh, const char* name, TYPE type)
{

    mesh->uniformCount++;
    int count = mesh->uniformCount;
    if(count > 1){
        GLint* newLocations = malloc(sizeof(GLint) * count);
        const char** newNames = malloc(sizeof(char*) * count);
        TYPE* newTypes = malloc(sizeof(TYPE) * count);

        memcpy(newLocations, mesh->uniformLocations, sizeof(GLint) * (count-1));
        memcpy(newNames, mesh->uniformNames, sizeof(char*) * (count-1));
        memcpy(newTypes, mesh->uniformTypes, sizeof(TYPE) * (count-1));

        free(mesh->uniformLocations);
        free(mesh->uniformNames);
        free(mesh->uniformTypes);

        mesh->uniformLocations = newLocations;
        mesh->uniformNames = newNames;
        mesh->uniformTypes = newTypes;
    }
    mesh->uniformLocations[count - 1] = glGetUniformLocation(mesh->program, name);
    mesh->uniformNames[count - 1] = name;
    mesh->uniformTypes[count - 1] = type;

}

bool sameString(const char* a,const char* b)
{
    char ca = 5;
    char cb = 5;
    int i = 0;
    while(ca != '\0' && cb != '\0')
    {

        cb = b[i];
        ca = a[i];

        if(ca != cb)
        {
            return false;
        }

        i++;
        if(i > 512)
        {
            return false;
        }
    }
    if(cb != ca)
    {
        return false;
    }
    return true;
}



void Update_Uniform(Mesh* mesh, const char* name, void* data)
{
    int uniformLocation = -1;
    int uniformIndex = -1;
    for(int i = 0; i < mesh->uniformCount; i++)
    {
        if(sameString(name, mesh->uniformNames[i]))
        {
            uniformLocation = mesh->uniformLocations[i];
            uniformIndex = i;
            break;
        }

    }
    if(uniformIndex == -1)
    {
        printf("\nERROR: Uniform name not found\n");
        exit(4);
    }
    glUseProgram(mesh->program);
    switch(mesh->uniformTypes[uniformIndex])
    {
        case FLOAT:
            glUniform1f(uniformLocation,*(float*)data);
            break;
        case INT:
            glUniform1i(uniformLocation,*(int*)data);
            break;
        case UINT:
            glUniform1ui(uniformLocation,*(unsigned int*)data);
            break;
        case IVEC2:
            glUniform2i(uniformLocation,((int*)data)[0],((int*)data)[1]);
            break;
        case VEC2:
            glUniform2f(uniformLocation,((float*)data)[0],((float*)data)[1]);
            break;
        case VEC3:
            glUniform3f(uniformLocation,((float*)data)[0],((float*)data)[1],((float*)data)[2]);
            break;
    }
}

void Add_Texture(Mesh* mesh, const char* name, Texture tex)
{
    mesh->boundTextures[mesh->textureCount] = tex;


    Add_Uniform(mesh, name, INT);
    Update_Uniform(mesh, name, &mesh->textureCount);

    mesh->textureCount++;

}

void Build_VAO(Mesh* mesh)
{
    int currentAttrib = 0;
    mesh->VBOs = malloc(sizeof(GLuint) * mesh->attribCount);

    glGenVertexArrays(1, &(mesh->VAO));
    glBindVertexArray(mesh->VAO);

    for(int i = 0; i < mesh->attribCount; i++)
    {
        if(mesh->attribs[i].perInstance == false)
            mesh->triangleCount = mesh->attribs[i].size/(sizeof(mesh->attribs[i].type) * mesh->attribs[i].vectorSize);
        glGenBuffers(1, &mesh->VBOs[i]);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->VBOs[i]);

        glBufferData(GL_ARRAY_BUFFER, mesh->attribs[i].size, mesh->attribs[i].data, GL_STATIC_DRAW);
        if(mesh->attribs[i].type == GL_INT || mesh->attribs[i].type == GL_UNSIGNED_INT)
        {
             glVertexAttribIPointer(
                                i,
                                mesh->attribs[i].vectorSize,
                                mesh->attribs[i].type,
                                mesh->attribs[i].stride,
                                mesh->attribs[i].firstOffset
                              );
        }
        else{

            glVertexAttribPointer(
                                i,
                                mesh->attribs[i].vectorSize,
                                mesh->attribs[i].type,
                                mesh->attribs[i].normalized,
                                mesh->attribs[i].stride,
                                mesh->attribs[i].firstOffset
                              );
        }
        glEnableVertexAttribArray(i);
        if(mesh->attribs[i].perInstance)
            glVertexAttribDivisor(i, 1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        PERROR();
    }

    for(int i = 0; i < mesh->textureCount; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, mesh->boundTextures[i].id);
    }

    glBindVertexArray(0);
}
void Update_VBO(Mesh* mesh, unsigned int index, void* data)
{
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBOs[index]);
    glBufferData(GL_ARRAY_BUFFER, mesh->attribs[index].size, data, GL_STATIC_DRAW);
}
void Draw(Mesh* mesh)
{
    glBindVertexArray(mesh->VAO);
    //use this shader program for the next draw calls
    glUseProgram(mesh->program);
    //draw the triangles
    glDrawArraysInstanced(GL_TRIANGLES, 0, mesh->triangleCount * 3, mesh->instances);
}

void Delete_Mesh(Mesh* mesh)
{
    glDeleteProgram(mesh->program);

    free(mesh->attribs);

    free(mesh->uniformLocations);
    free(mesh->uniformNames);
    free(mesh->uniformTypes);



}


