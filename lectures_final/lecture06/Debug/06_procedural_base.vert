/*

06_procedural_base.vert: Vertex shader for the examples on procedural texturing. It is equal to 05_uv2color.vert

author: Davide Gadia

Real-Time Graphics Programming - a.a. 2018/2019
Master degree in Computer Science
Universita' degli Studi di Milano

*/

#version 330 core

// vertex position in world coordinates
layout (location = 0) in vec3 position;
// UV texture coordinates
layout (location = 2) in vec2 UV;


// model matrix
uniform mat4 modelMatrix;
// view matrix
uniform mat4 viewMatrix;
// Projection matrix
uniform mat4 projectionMatrix;

// the output variable for UV coordinates
out vec2 interp_UV;

void main()
{
		// I assign the values to a variable with "out" qualifier so to use the per-fragment interpolated values in the Fragment shader
		interp_UV = UV;

		// transformations are applied to each vertex
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0f);
}
