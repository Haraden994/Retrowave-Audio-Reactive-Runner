#version 330 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 UV;
//layout (location = 3) in mat4 instanceMatrix;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

out vec2 interp_UV;

void main()
{
    interp_UV = UV;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0f); 
}