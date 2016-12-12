#version 330 core
in vec3 vertex;
in vec2 texCoord;
uniform mat4 lightSpaceMatrix;
uniform mat4 m;
void main()
{
    gl_Position = lightSpaceMatrix * m * vec4(vertex, 1.0f);
}
