#version 330 core
out vec4 FragColor;
//
in vec3 color;

void main()
{
   vec3 col;
   col = color;
   FragColor = vec4(col, 1.0f);
}
