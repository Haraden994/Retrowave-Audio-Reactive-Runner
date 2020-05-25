#version 330 core

// vertex position in world coordinates
layout (location = 0) in vec3 position;
// vertex normal in world coordinate
layout (location = 1) in vec3 normal;
// UV texture coordinates
layout (location = 2) in vec2 UV;

// model matrix
uniform mat4 modelMatrix;
// view matrix
uniform mat4 viewMatrix;
// Projection matrix
uniform mat4 projectionMatrix;
// normal matrix
uniform mat3 normalMatrix;

uniform vec3 pointLightPosition;

// Array storing values for the eight frequency bands
uniform float frequencyBands[];

uniform float time;
uniform float scrollSpeed;
// The amount of zoom applied to the UV coordinates is used to "zoom" in/out the noise
uniform float zoom;
uniform float dPower;
uniform float streetSize;
uniform float fade;

// Interpolated UV coordinates to pass to the fragment shader
out vec2 interp_UV;

out vec3 lightDir;
out vec3 vNormal;
out vec3 vViewPosition;
out vec3 vPosition;

int NUM_OCTAVES = 5;

float rand(vec2 n)
{ 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

float noise(vec2 p)
{
	vec2 ip = floor(p);
	vec2 u = fract(p);
	u = u*u*(3.0-2.0*u);
	
	float res = mix(
		mix(rand(ip),rand(ip+vec2(1.0,0.0)),u.x),
		mix(rand(ip+vec2(0.0,1.0)),rand(ip+vec2(1.0,1.0)),u.x),u.y);
	return res*res;
}

// Fractal brownian motion
float fbm(vec2 x)
{
	float v = 0.0;
	float a = 0.5;
	vec2 shift = vec2(100);
	// Rotate to reduce axial bias
    mat2 rot = mat2(cos(0.5), sin(0.5), -sin(0.5), cos(0.5));
	for (int i = 0; i < NUM_OCTAVES; ++i) {
		v += a * noise(x);
		x = (rot * x) * 2.0 + shift;
		a *= 0.5;
	}
	return v;
}

// Displace vertices according to their V value, the grid is divided in eight zones, one for each Frequency Band.
float DisplaceByFBands(){
    if(UV.y <= 0.125){
        return mix(frequencyBands[0], frequencyBands[1], 
								UV.y / 0.125);
    }
    if(UV.y <= 0.25){
        return mix(frequencyBands[1], frequencyBands[2], 
								(UV.y - 0.125) / 0.125);
    }
    if(UV.y <= 0.375){
        return mix(frequencyBands[2], frequencyBands[3], 
								(UV.y - 0.25) / 0.125);
    }
    if(UV.y <= 0.5){
        return mix(frequencyBands[3], frequencyBands[4], 
								(UV.y - 0.375) / 0.125);
    }
    if(UV.y <= 0.625){
        return mix(frequencyBands[4], frequencyBands[5], 
								(UV.y - 0.5) / 0.125);
    }
    if(UV.y <= 0.75){
        return mix(frequencyBands[5], frequencyBands[6], 
								(UV.y - 0.625) / 0.125);
    }
    if(UV.y <= 0.875){
        return mix(frequencyBands[6], frequencyBands[7], 
								(UV.y - 0.75) / 0.125);
    }
	if(UV.y <= 1.0){
		return mix(frequencyBands[7], frequencyBands[0], 
								(UV.y - 0.875) / 0.125);
	}
}

void main()
{
	float speed = time * scrollSpeed;
	// translation for noise and grid scrolling animation
	vec2 translate = vec2(0.0, speed);
	
	// speed used for vertex translation, this will let the vertex 
	//move along the z axis and then return to its original position
	float speedFrac = fract(speed) * 0.1;
	
	// The noise is also traslated by multiplying the floor of the translation 
	// with an offset depending on the zoom value (offset = zoom * 10^-2).
	// The resulting translation let the noise move row by row along the grid.
	vec2 noisePos = UV * zoom - floor(translate) * (zoom / 99.0);
	float noised = fbm(noisePos);
	
	// we add the street space into the grid by smoothing the noise
	noised *= 1.0 - (smoothstep(0.5 - streetSize - fade, 0.5 - streetSize, UV.x) 
				- smoothstep(0.5 + streetSize, 0.5 + streetSize + fade, UV.x));

	float displacement = (noised * DisplaceByFBands()) * dPower;
	
	vec3 displacedPosition = position + displacement * normal;
	// translate the vertex position in order to achieve the movement illusion
	displacedPosition.z += speedFrac * 50;
	vPosition = displacedPosition;
	
	vec4 modelView = viewMatrix * modelMatrix * vec4(displacedPosition, 1.0);
	
	vViewPosition = -modelView.xyz;
	// transformations are applied to the normal
	vNormal = normalize( normalMatrix * normal );

	// light incidence direction (in view coordinate)
	vec4 lightPos = viewMatrix  * vec4(pointLightPosition, 1.0);
	lightDir = lightPos.xyz - modelView.xyz;
	
	interp_UV = UV;
	gl_Position = projectionMatrix * modelView;
}
