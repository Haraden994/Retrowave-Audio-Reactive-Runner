#version 330

in vec2 interp_UV;

out vec4 outColor;

const vec3 gridColor = vec3(1.0, 0.0, 1.0);
const float edgeThickness = 2.0;
const float edgeSharpness = 20.0;
const float edgeSubtract = 0.3;
const float glowStrength = 10.0;
const float gridZoom = 99.0;

void main() 
{
	vec3 bgColor = vec3(0.2, 0.0, 0.2);
	vec2 st = vec2(interp_UV * gridZoom);
	st = fract(st);
	st = abs(st - 0.5) * edgeThickness;
	st = pow(st, vec2(edgeSharpness)) - edgeSubtract;
	
	float c = clamp(st.x + st.y, 0.0, 1.0) * glowStrength;
	
   	outColor = vec4(bgColor + gridColor * c, 1.0f);   
}