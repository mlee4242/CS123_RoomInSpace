#version 410

uniform bool leftEye;
uniform bool overUnder;
uniform mat4 transform;
in vec3 vertex;
in vec2 texCoord;
in vec3 normal;
out vec2 fragTexCoord;
out vec3 WorldSpace_position; // world-space position
out vec3 WorldSpace_normal;   // world-space normal
void main()
{
    fragTexCoord = texCoord;

    if (overUnder) {
        if (leftEye) {
            fragTexCoord.t = fragTexCoord.t * 0.5 + 0.5;
        } else {
            fragTexCoord.t = fragTexCoord.t * 0.5;
        }
    }
    WorldSpace_position = gl_Position.xyz;
    WorldSpace_normal = normal;
    gl_Position = transform * vec4(vertex, 1.0f);
}
