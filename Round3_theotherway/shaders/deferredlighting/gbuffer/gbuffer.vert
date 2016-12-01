#version 330 core

layout(location = 0) in vec3 position; // Position of the vertex
layout(location = 1) in vec3 normal;   // Normal of the vertex

// Transformation matrices
uniform mat4 p;
uniform mat4 v;
uniform mat4 m;
uniform mat3 normalMatrix;

out vec3 cameraSpacePosition;
out vec3 cameraSpaceNormal;

void main() {
    vec4 cameraSpacePosition4 = v * m * vec4(position, 1.0);

    gl_Position = p * cameraSpacePosition4;

    cameraSpaceNormal = normalMatrix * normal;
    cameraSpacePosition = cameraSpacePosition4.xyz;
}
