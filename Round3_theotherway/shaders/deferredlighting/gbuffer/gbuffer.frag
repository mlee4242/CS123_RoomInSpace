#version 330 core

in vec3 cameraSpacePosition;
in vec3 cameraSpaceNormal;

// Material data
uniform float shininess;

out vec3 out_cameraSpacePosition;
out vec4 out_cameraSpaceNormal; // Stores shininess in 4th component

vec3 pack(vec3 v) {
    return 0.5 * v + vec3(0.5);
}

void main() {
    out_cameraSpacePosition = pack(cameraSpacePosition);
    out_cameraSpaceNormal = vec4(pack(cameraSpaceNormal), 1.0);//shininess);
}
