#version 330

uniform float u_time;

in vec2 i_UV;

uniform int explodeValue;

out vec4 fragColor;

float rand(vec2 n)
{ 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

float line(float center, float size, float edge, float y) {
	return max(
        max(
        	smoothstep(center - size - edge, center - size, y) *
            smoothstep(center + size + edge, center + size, y),
        	smoothstep(center + size + edge - 1.0, center + size - 1.0, y)
        ),
        smoothstep(center - size + 1.0 - edge, center - size + 1.0, y)
    );
}

void main(){

    float v = smoothstep(0.2, 1.0, i_UV.y);
	vec3 gradient;
	
	float random = rand(i_UV);
	vec3 flicker;
	float flickerSpeed = 100.0;
	
	if(u_time < 0){
		gradient = mix(vec3(1.0, 0.0, 1.0), vec3(0.0, 1.0, 1.0), v);
		flicker = vec3(abs(sin(u_time * random * flickerSpeed)), 0.0, 0.0);
	}
	else{
		gradient = mix(vec3(0.0, 1.0, 1.0), vec3(1.0, 0.0, 1.0), v);
		flicker = vec3(0.0, abs(sin(u_time * random * flickerSpeed)), 0.0);
	}
	
    float lineSize = 0.3;
    float lineSpeed = 1.0 - fract(u_time * 5.0);
    float lineY = fract(i_UV.y * 10.0);
    float lines = 1.0 - line(lineSpeed, lineSize, 0.05, lineY);
	
	if(explodeValue == 0)
		gradient *= lines;
	else
		gradient = flicker;
	
    fragColor = vec4(gradient, 1.0);
}