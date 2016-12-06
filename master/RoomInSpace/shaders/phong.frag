#version 410

uniform sampler2D texMap;
in vec3 diffuseColor;
in vec2 fragTexCoord;
in vec3 WorldSpace_position; // world-space position
in vec3 WorldSpace_normal;   // world-space normal
out vec4 fragColor;

void main()
{
//    fragColor = texture2D(texMap, fragTexCoord);
//    vec3 WorldSpace_toLight = normalize(vec3(100.0) - WorldSpace_position);
//    vec3 phongColor = vec3(0.3 + 0.7 * max(0.0, dot(normalize(WorldSpace_normal), WorldSpace_toLight)));
//    vec3 texColor = texture2D(texMap, fragTexCoord).rgb;
//    vec3 mixColor = mix(texColor, diffuseColor, 0.5)

    fragColor = vec4(diffuseColor, 1.f);//vec4(mixColor, 1.0);
 }
