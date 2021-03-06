#version 410

uniform sampler2D tex;

in vec2 fragTexCoord;
in vec3 WorldSpace_position; // world-space position
in vec3 WorldSpace_normal;   // world-space normal
out vec4 fragColor;

void main()
{
   // fragColor = texture2D(tex, fragTexCoord);
    vec3 WorldSpace_toLight = normalize(vec3(100.0) - WorldSpace_position);
    vec3 phongColor = vec3(0.3 + 0.7 * max(0.0, dot(normalize(WorldSpace_normal), WorldSpace_toLight)));
    vec3 texColor = texture2D(tex, fragTexCoord).rgb;
    vec3 mixColor = mix(texColor, phongColor, 0.5);
    fragColor = vec4(mixColor, 1.0);
 }
