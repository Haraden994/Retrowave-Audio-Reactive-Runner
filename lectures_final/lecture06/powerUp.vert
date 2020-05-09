#version 330 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 UV;
//layout (location = 3) in mat4 instanceMatrix;

out vec2 interp_UV;

void main()
{
    interp_UV = UV;
    gl_Position = vec4(position, 1.0f); 
}