#version 330 core

uniform ivec2 uResolution;
uniform float uTime;

layout (location = 0) in vec2 aPos;

out vec3 color;
out vec2 uv;

void main()
{
    //vec3 aPos = vec3(1);
    //aPos.xy = vec2(gl_VertexID/2,gl_VertexID%2) * 2.0;
   uv = aPos * 0.5 + 0.5;

   vec2 pos = aPos;
   gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
}
