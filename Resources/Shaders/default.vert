#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aInstancePosition;
void main()
{
   vec3 pos = aPos * 0.1 + vec3(aInstancePosition.x,aInstancePosition.y,0.0) * 0.1;
   //pos += float(gl_InstanceID) * 0.2;
   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
}
