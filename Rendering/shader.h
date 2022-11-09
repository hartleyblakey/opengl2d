#ifndef SHADERH
#define SHADERH

#include "glad/glad.h"
#include "GLFW/glfw3.h"

typedef GLuint Program;
typedef GLuint Shader;

void attachShaders(Program prog, Shader frag, Shader vert);
void bindShader(Shader shader);
const char* parseShader(const char* file);
Program createProgram(char* vertex, char* fragment);
Program createSimpleProgram(char* file);


#endif
