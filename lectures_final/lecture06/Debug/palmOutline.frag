#version 330 core

out vec4 fragColor;

in vec2 interp_UV;

uniform vec3 color;

void main(){

	fragColor = vec4(color, 1.0);
	
}