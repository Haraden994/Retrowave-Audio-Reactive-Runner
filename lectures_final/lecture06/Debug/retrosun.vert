
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
	// Original vertex position is copied in a local variable.
	vec3 flattened = position;
	// Z coordinate is set at 0
	flattened.z = 0.0;
	
	// I assign the values to a variable with "out" qualifier so to use the per-fragment interpolated values in the Fragment shader
	interp_UV = UV;

	// transformations are applied to each vertex
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0f);
}
