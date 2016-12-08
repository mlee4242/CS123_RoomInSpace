#version 410
uniform bool useTex;
uniform sampler2D texMap;
uniform sampler2D normalMap;
uniform vec3 diffuse;
uniform bool pickable;
uniform bool light;
in vec2 fragTexCoord;
in vec3 WorldSpace_position; // world-space position
in vec3 WorldSpace_normal;   // world-space normal
out vec4 fragColor;
void main()
{
  if(useTex){
     vec2 uv = vec2(fragTexCoord.x, 1.f - fragTexCoord.y);
     vec3 normal = WorldSpace_normal;
     if(false){
         normal = texture2D(normalMap, uv).rgb;
         normal = normalize(normal * 2.0 - 1.0);
     }
    vec3 WorldSpace_toLight = normalize(vec3(0, 4, 0) - WorldSpace_position);
    vec3 phongColor = vec3(0.3 + 0.7 * max(0.0, dot(normalize(normal), WorldSpace_toLight)));
    vec4 texColor = texture2D(texMap, uv);
    fragColor = vec4(phongColor.x * mix(texColor.xyz, diffuse, 0.2), 1.f);
   // fragColor = vec4(phongColor, 1.0f);
  }else{
    fragColor = vec4(diffuse, 1.f);
  }
  if(!light){
      fragColor = 0.3 * fragColor;
  }

//  if(pickable){
//      fragColor = vec4(1.0, 0, 0, 1);
//  }
 }
