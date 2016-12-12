#version 410

uniform sampler2D textMap;
uniform sampler2D bumpMap;
uniform sampler2D normalMap;
uniform sampler2D shadowMap;
uniform vec3 diffuse;
uniform bool pickable;
uniform bool useTex;
uniform bool useBump;
uniform bool useNormal;
uniform bool light;
uniform bool isSky;
uniform bool isInside;
in vec2 fragTexCoord;
in vec3 WorldSpace_position; // world-space position
in vec3 WorldSpace_normal;   // world-space normal
out vec4 fragColor;
const vec3 lightPosition = vec3(0, 1.8, 0);

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture2D(shadowMap, projCoords.xy).r;
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    float bias = 0.001;
    // Check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    return shadow;
}


void main()
{
  vec3 normal = WorldSpace_normal;
  vec3 WorldSpace_toLight = normalize(lightPosition - WorldSpace_position);
  // don't know what the hell is this
  vec2 uv = vec2(fragTexCoord.x, 1.f - fragTexCoord.y);

  if(useTex){
     if(useBump){ // bump mapping
        // do something
     }
     if(useNormal){ // normal mapping
        // do something
        // normal = texture2D(normalMap, uv).rgb * 2.0 - 1.0;
     }
    vec3 phongColor = vec3(0.3 + 0.7 * max(0.0, dot(normalize(normal), WorldSpace_toLight)));
    vec4 texColor = texture2D(textMap, uv);
    float coef = phongColor.x;
    if(isSky){
        coef = 1.f;
    }
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
    fragColor = (1.0 - 0.5 * shadow) * vec4( coef * mix(texColor.rgb, diffuse, 0.2), 1.f);
  }else{
    fragColor = vec4(diffuse, 1.f);
  }

  // light on / light off
  if(!light && isInside){
      fragColor = 0.3 * fragColor;
  }

//  if(useNormal){
//      fragColor = texture2D(normalMap, uv);
//  }

//  if(useBump){
//      fragColor = vec4(0.0, 0, 1.0, 1);
//  }

//  if(pickable){
//      fragColor = vec4(1.0, 0, 0, 1);
//  }
 }
