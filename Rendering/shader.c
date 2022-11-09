#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shader.h"


void attachShaders(Program shaderProgram, Shader fragmentShader, Shader vertexShader)
{
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //free the shader slots once they have been incorperated in the program
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
}

void bindShader(Shader shader)
{

}



const char* parseShader(const char* file)
{


  FILE *fp;
  long fileSize;
  char *buffer;
  fp = fopen(file, "r");



  if(!fp) exit(5);
  //printf("Here!\n");
  fseek(fp, 0L, SEEK_END);
  fileSize = ftell(fp);
  rewind(fp);

  buffer = malloc(fileSize + 1);
  if(fread(buffer, fileSize, 1, fp) != 1)
  {
    fclose(fp);
    free(buffer);
    exit(5);
  }


  //printf("read the buffer!\n");
  fclose(fp);
  //const char* cbuffer = buffer;
  //buffer[fileSize-1] = '\0';
  //printf("\n%s\n",buffer);



  return (const char*)buffer;
}

void printShaderCompilationErrors(Shader shader)
{
  GLint isCompiled = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
  if(isCompiled == GL_FALSE)
  {
    int maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
    char* buffer = malloc(maxLength);
    glGetShaderInfoLog(shader, maxLength, &maxLength, buffer);
    printf("Shader compilation failed:\n%s\n",buffer);
    free(buffer);

  }
}

Program createProgram(char* vertex, char* fragment)
{
  printf("HERE?!\n");
  const char* fragSource = parseShader(fragment);
  const char* vertSource = parseShader(vertex);

  //printf("Fragment Shader:\n%s\n\n", fragSource);
  //printf("Vertex Shader:\n%s\n\n", vertSource);

  Shader vert = glCreateShader(GL_VERTEX_SHADER);
  printf("Here?\n");

  glShaderSource(vert, 1, &vertSource, NULL);
  glCompileShader(vert);
  printf("at least i made it here\n");
  printShaderCompilationErrors(vert);


  Shader frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &fragSource, NULL);
  glCompileShader(frag);
  printShaderCompilationErrors(frag);


  Program shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vert);
  glAttachShader(shaderProgram, frag);
  glLinkProgram(shaderProgram);

  //free the shader slots once they have been incorperated in the program
  glDeleteShader(frag);
  glDeleteShader(vert);
  free((char*)fragSource);
  free((char*)vertSource);
  return shaderProgram;

}

Program createSimpleProgram(char* file)
{
  int nameSize = strlen(file);
  char* fragFile = malloc(nameSize + 1 + 45);
  char* vertFile = malloc(nameSize + 1 + 45);

  strcpy(fragFile, file);
  strcpy(vertFile, file);



  const char* fragSource = parseShader(strcat(fragFile,".frag"));
  const char* vertSource = parseShader(strcat(vertFile,".vert"));

  free(fragFile);
  free(vertFile);

  //printf("Fragment Shader:\n%s\n\n", fragSource);
  //printf("Vertex Shader:\n%s\n\n", vertSource);

  Shader vert = glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(vert, 1, &vertSource, NULL);
  glCompileShader(vert);
  printShaderCompilationErrors(vert);
  printf("aa");
  Shader frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &fragSource, NULL);
  glCompileShader(frag);
  printShaderCompilationErrors(frag);

  Program shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vert);
  glAttachShader(shaderProgram, frag);
  glLinkProgram(shaderProgram);

  //free the shader slots once they have been incorperated in the program
  glDeleteShader(frag);
  glDeleteShader(vert);
  free((char*)fragSource);
  free((char*)vertSource);
  return shaderProgram;

}
