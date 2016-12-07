#version 410
uniform bool useTex;
uniform sampler2D texMap;
uniform vec3 diffuse;
in vec2 fragTexCoord;
in vec3 WorldSpace_position; // world-space position
in vec3 WorldSpace_normal;   // world-space normal
out vec4 fragColor;
uniform bool light;

void main()
{
  if(useTex){
    vec2 uv = vec2(fragTexCoord.x, 1.f - fragTexCoord.y);
    fragColor = texture2D(texMap, uv);
    vec3 WorldSpace_toLight = normalize(vec3(100.0) - WorldSpace_position);
    vec3 phongColor = vec3(0.3 + 0.7 * max(0.0, dot(normalize(WorldSpace_normal), WorldSpace_toLight)));
    vec3 texColor = texture2D(texMap, fragTexCoord).rgb;
    vec3 mixColor = mix(texColor, diffuse, 0.5);
  }else{
    fragColor = vec4(diffuse, 1.f);
  }
  if(!light){
      fragColor = 0.3 * fragColor;
  }
 }
