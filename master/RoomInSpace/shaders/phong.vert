#version 410

uniform bool leftEye;
uniform bool overUnder;
uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
uniform vec2 move;
uniform mat4 lightSpaceMatrix;
in vec3 vertex;
in vec2 texCoord;
in vec3 normal;
out vec4 color;
out vec2 fragTexCoord;
out vec3 position; // world-space position
out vec3 n;   // world-space normal

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

void main()
{
    fragTexCoord = texCoord;
// move UFO
    vec3 vert = vertex;
    if(move.x > 0){
        vert.z +=  4.0f * cos(move.y);
        vert.x +=  4.0f * sin(move.y);
    }

     gl_Position =  p * v * m * vec4(vert.x, vert.y + 0.2f, vert.z, 1.0f);
     position = gl_Position.xyz;
     n = normal;
     vs_out.FragPos = vec3(m * vec4(vert, 1.0f));
     vs_out.Normal = transpose(inverse(mat3(m))) * normal;
     vs_out.TexCoords = texCoord;
     vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0f);
}
