#version 430
uniform sampler2D shadowMap;
uniform sampler2D textMap;
uniform sampler2D bumpMap;
uniform sampler2D normalMap;
uniform vec3 diffuse;
uniform vec3 ambient;
uniform vec3 specular;
uniform bool pickable;
uniform bool useTex;
uniform bool useBump;
uniform bool useNormal;
uniform bool lightOn;
uniform bool shadowOn;
uniform bool isSky;
uniform bool isInside;
uniform vec3 pointLightPosition;
uniform vec3 dirLightDir;
in vec2 fragTexCoord;
in vec3 position; // world-space position
in vec3 n;        // world-space normal
in vec3 viewPos;
in vec3 fragPos;
in vec4 fragPosLightSpace;
//in mat3 TNB;
out vec4 fragColor;

float shadowCalculation(){
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords.x = projCoords.x * 0.5 + 0.5;
    projCoords.y = projCoords.y * 0.5 + 0.5;
    float currentDepth = 0.5f * projCoords.z + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture2D(shadowMap, projCoords.xy).x;
    // Get depth of current fragment from light's perspective
    return currentDepth * 1000 - closestDepth * 1000 > 7
            ? 0.75 : 1.0;
}

float  percentageCloserFiltering(float bias){
    float shadow = 0.0;
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords.x = projCoords.x * 0.5 + 0.5;
    projCoords.y = projCoords.y * 0.5 + 0.5;
    float currentDepth = 0.5f * projCoords.z + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture2D(shadowMap, projCoords.xy).x;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x){
        for(int y = -1; y <= 1; ++y){
            float pcfDepth = texture2D(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - 0.008 > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    return shadow;
}

void main()
{
  vec3 WorldSpace_toLight = normalize(pointLightPosition - fragPos);
  // QImage flippe my texture, have to flip the coordiantes
  vec2 uv = vec2(fragTexCoord.x, 1.f - fragTexCoord.y);
  vec3 normal = n;
  if(useTex){
     if(useBump){ // bump mapping
        // do something to the normal based on the bump mapping
     }
     if(useNormal){ // normal mapping
        // do something to the normal based on the normal mapping
        // texture is in tangent space? Convert to world space.

        // normal = normalize(texture2D(normalMap, uv).rgb * 2.0 - 1.0);
        // normal = normalize(TBN * normal);
     }
    float diff = 0.5 + 0.5 * max(0.0, dot(normalize(normal), WorldSpace_toLight));
    vec3 lightDir = normalize(pointLightPosition - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
         spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);

    float shadow = 1.0f;
    vec4 texColor = texture2D(textMap, uv);
    vec3 diffTex = mix(texColor.rgb, diffuse, 0.2).rgb;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float att = 1.0f;
    if(!isSky){
       if(shadowOn){
           shadow = 1.f - 0.25 * percentageCloserFiltering(bias);
       }
       if(isInside){
           att =  .25f * distance(vec3(0, 0, 0), vec3(fragPosLightSpace));
           att =  min(1.f, 1.f / (1.f + att + att * att));
       }
       fragColor = vec4(shadow * (att * diff * diffTex + spec * specular), 1.0f);
    }else{
       fragColor = vec4(diffTex, 1.0f);
    }
  }

  // light on / light off
  if(!lightOn && isInside){
      fragColor = 0.3 * fragColor;
  }
}
