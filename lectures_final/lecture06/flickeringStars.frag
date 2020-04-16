#version 330

in vec2 interp_UV;
out vec4 fragColor;

uniform float u_time;

float random(vec2 ab) 
{
	float f = (cos(dot(ab ,vec2(21.9898,78.233))) * 43758.5453);
	return fract(f);
}

float noise(in vec2 xy) 
{
	vec2 ij = (xy);
	vec2 uv = xy-ij;
	uv = uv*uv*(3.0-2.0*uv);
	

	float a = random(vec2(ij.x, ij.y ));
	float b = random(vec2(ij.x+1., ij.y));
	float c = random(vec2(ij.x, ij.y+1.));
	float d = random(vec2(ij.x+1., ij.y+1.));
	float k0 = a;
	float k1 = b-a;
	float k2 = c-a;
	float k3 = a-b-c+d;
	return (k0 + k1*uv.x + k2*uv.y + k3*uv.x*uv.y);
}

void main() {

	float color = pow(noise(interp_UV), 1000.0) * 20.0;
	float r1 = noise(interp_UV*noise(vec2(sin(u_time*0.01))));
	float r2 = noise(interp_UV*noise(vec2(cos(u_time*0.01), sin(u_time*0.01))));
	float r3 = noise(interp_UV*noise(vec2(sin(u_time*0.05), cos(u_time*0.05))));
	
	fragColor = vec4(vec3(color*r1 + 0.2, color*r2 + 0.129, color*r3 + 0.286), 1.0);

}