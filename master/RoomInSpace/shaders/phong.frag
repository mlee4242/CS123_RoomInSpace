#version 410

uniform sampler2D textMap;
uniform sampler2D bumpMap;
uniform sampler2D normalMap;
uniform vec3 diffuse;
uniform bool pickable;
uniform bool useTex;
uniform bool useBump;
uniform bool useNormal;
uniform bool light;
in vec2 fragTexCoord;
in vec3 WorldSpace_position; // world-space position
in vec3 WorldSpace_normal;   // world-space normal
out vec4 fragColor;
void main()
{
  vec3 normal = WorldSpace_normal;
  vec3 WorldSpace_toLight = normalize(vec3(0, 4, 0) - WorldSpace_position);

  if(useTex){
     vec2 uv = vec2(fragTexCoord.x, 1.f - fragTexCoord.y);

     if(useBump && false){ // bump mapping
        // do something
     }
     if(useNormal && false){ // normal mapping
        // do something
     }
    vec3 phongColor = vec3(0.3 + 0.7 * max(0.0, dot(normalize(normal), WorldSpace_toLight)));
    vec4 texColor = texture2D(textMap, uv);
    fragColor = vec4(phongColor.x * mix(texColor.xyz, diffuse, 0.2), 1.f);
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
