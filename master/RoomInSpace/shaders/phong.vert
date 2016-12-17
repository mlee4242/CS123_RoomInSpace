#version 410

uniform bool leftEye;
uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
uniform vec2 move;
uniform mat4 lightSpaceMatrix;
uniform vec3 pointLightPosition;
in vec3 vertex;
in vec2 texCoord;
in vec3 normal;
//in vec3 tangent;
//out mat3 TNB;
out vec4 color;
out vec2 fragTexCoord;
out vec3 position; // world-space position
out vec3 n;        // world-space normal
out vec4 fragPosLightSpace;
out vec3 viewPos;
out vec3 fragPos;

// http://stackoverflow.com/questions/4200224/random-noise-functions-for-glsl
float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    fragTexCoord = texCoord;
    // move UFO
    vec3 vert = vertex;
    if(move.x > 0){
        vert.z +=  4.0f * cos(move.y);
        vert.x +=  4.0f * sin(move.y);
    }

     gl_Position =  p * v * m * vec4(vert.x, vert.y, vert.z, 1.0f);
     fragPos = vec3(m * vec4(vert, 1.0));
     position = gl_Position.xyz;
     n = normal;
     fragPosLightSpace = lightSpaceMatrix * m * vec4(vert, 1.0f);
     viewPos = vec3(v * vec4(0, 0, 0, 1.f));

     //create the TNB matrix for normal mapping
     //vec3 T = normalize(vec3(model * vec4(tangent, 0.0)));
     //vec3 N = normalize(vec3(model * vec4(normal, 0.0)));
     //vec3 B = cross(T, N);

     //TNB = mat3(T, N, B);
}
