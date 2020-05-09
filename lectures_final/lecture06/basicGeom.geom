#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

//UV Pass-Through: This is the input from the vertex shader
in vec2 interp1_UV[];

//matrices for projection. I need to compute the projection here and not in the vertex because many geometry shaders will need to work in world coordinates.
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

//UV Pass-Through: This is the output to the fragment shader
out vec2 interp_UV;

void main()
{
    int i;
    for(i = 0;i < gl_in.length();i++)
    {
        interp_UV = interp1_UV[i];
        gl_Position = projectionMatrix* viewMatrix * modelMatrix* gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}