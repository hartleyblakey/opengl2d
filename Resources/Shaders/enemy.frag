#version 330 core

uniform sampler2D frogTex;
uniform ivec2 uResolution;

out vec4 FragColor;

in vec3 color;
in vec2 uv;



void main()
{
   vec3 col;
   col = color;
   vec4 tex = texture(frogTex,uv);
   //tex.a = clamp(distance(uv,vec2(0.5)),0.0,1.0);
   if(tex.a < 0.5)
   {
      discard;
      return;
   }
  // if(tex.r < 0.5){discard;return;}

   //tex.rgb = mix(vec3(1.0,0.9,1.0),tex.rgb,((tex.a-0.3)/0.7));

   col.rgb = tex.rgb;
   FragColor = vec4(col, 1.0f);
}
