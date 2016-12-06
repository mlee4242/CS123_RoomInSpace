#version 410

uniform bool leftEye;
uniform bool overUnder;
uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
in vec3 diffuse;
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

//    gl_Position = p * v * m * vec4( 0.05 *  vertex.x,  0.05 *vertex.y, -0.05 * vertex.z, 1.0f);
//     gl_Position = p * v * m * vec4( 0.01f * vertex.x,  -0.01f * vertex.y, 0.01f * vertex.z, 1.0f);
     gl_Position = p * v * m * vec4(vertex, 1.0f);
     WorldSpace_position = gl_Position.xyz;
     WorldSpace_normal = normal;
//      gl_Position = p * v * m * vec4( 0, 0, 1, 1.0f);
}
