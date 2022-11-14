#ifndef TEXTURE_H
#define TEXTURE_H

#include "stb_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string.h>

typedef struct Texture{
    GLuint id;
    GLenum type;
}Texture;

//Texture TEXTURES[32];
//unsigned int TEXTURE_COUNT;


Texture Upload_Texture_2D(const char* filename);




#endif
