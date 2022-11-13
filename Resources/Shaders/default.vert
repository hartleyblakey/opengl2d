#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aInstancePosition;
layout (location = 2) in uint aColorPacked;
layout (location = 3) in float aSize;

uniform ivec2 uResolution;
uniform float uTime;

vec3 unpackColor(uint packedColor)
{
   vec3 color;
  // packedColor = uint(0x0000FF00);
   color.r = float((packedColor >> 24) & 255u) / 255.0;
   color.g = float((packedColor >> 16) & 255u) / 255.0;
   color.b = float((packedColor >> 8) & 255u) / 255.0;
   return color;
}

out vec3 color;

void main()
{
   vec3 pos = aPos;
   pos *= aSize;

   pos.xy += aInstancePosition;
   //vec3 pos = aPos * 10.0;
   color = unpackColor(aColorPacked);
   pos *= 0.01;
   pos.x *= (float(uResolution.y)/float(uResolution.x));
   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
}
