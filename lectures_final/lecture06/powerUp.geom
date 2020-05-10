#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec2 UV[];

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

uniform float time;
uniform int explodeValue;

out vec2 interp_UV;

vec4 Explode(vec4 position, vec3 normal)
{
    float magnitude = 10.0;
    vec3 direction = normal * explodeValue * time * magnitude;
    return position + vec4(direction, 0.0);
} 

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(b, a));
}

void main(){
	for(int i = 0; i < gl_in.length(); i++){
		interp_UV = UV[i];
		gl_Position = projectionMatrix * viewMatrix * modelMatrix * Explode(gl_in[i].gl_Position, GetNormal());
		EmitVertex();
	}
	EndPrimitive();
}