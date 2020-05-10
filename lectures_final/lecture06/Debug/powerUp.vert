#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 UV;
//layout (location = 3) in mat4 instanceMatrix;

out vec2 interp_UV;

out vec3 vertexNormal;

void main()
{
    interp_UV = UV;
	vertexNormal = normal;
    gl_Position = vec4(position, 1.0f); 
}