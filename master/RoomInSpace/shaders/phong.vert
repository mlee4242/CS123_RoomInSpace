#version 410

uniform bool leftEye;
uniform bool overUnder;
uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
uniform vec2 move;
in vec3 vertex;
in vec2 texCoord;
in vec3 normal;
out vec2 fragTexCoord;
out vec3 WorldSpace_position; // world-space position
out vec3 WorldSpace_normal;   // world-space normal
void main()
{
    fragTexCoord = texCoord;

//    if (overUnder) {
//        if (leftEye) {
//            fragTexCoord.t = fragTexCoord.t * 0.5 + 0.5;
//        } else {
//            fragTexCoord.t = fragTexCoord.t * 0.5;
//        }
//    }

//     gl_Position =  p * v * m * vec4( vertex.x + 0.25, vertex.z - 0.1 , -vertex.y + 0.25, 1.0f); // 12/6 night
//     gl_Position =  p * v * m * vec4( vertex.x , vertex.z , vertex.y, 1.0f); // 12/6 night
    vec3 vert = vertex;
    if(move.x > 0){
        vert.x += 0.5f * sin(move.y) ;
    }

     gl_Position =  p * v * m * vec4(0.1f * vert, 1.0f); // 12/6 night
     WorldSpace_position = gl_Position.xyz;
     WorldSpace_normal = normal;


}
