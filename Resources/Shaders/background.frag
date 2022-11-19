#version 330 core

uniform sampler2D backgroundTex;
uniform ivec2 uResolution;

out vec4 FragColor;

in vec2 uv;



void main()
{
   vec3 col = vec3(1);
   vec4 tex = texture(backgroundTex,uv);
   col.rgb = tex.rgb;
   //col.rgb = vec3(1);
   FragColor = vec4(col, 1.0f);
}
