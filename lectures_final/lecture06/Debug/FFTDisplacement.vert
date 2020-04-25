#version 330 core

#define NUM_OCTAVES 5

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

uniform float offset;

// Interpolated UV coordinates to pass to the fragment shader
out vec2 interp_UV;

out vec3 lightDir;
out vec3 vNormal;
out vec3 vViewPosition;
out vec3 vPosition;

// Some useful functions
vec3 mod289(vec3 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec2 mod289(vec2 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec3 permute(vec3 x) { return mod289(((x*34.0)+1.0)*x); }

//
// Description : GLSL 2D simplex noise function
//      Author : Ian McEwan, Ashima Arts
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License :
//  Copyright (C) 2011 Ashima Arts. All rights reserved.
//  Distributed under the MIT License. See LICENSE file.
//  https://github.com/ashima/webgl-noise
//
float snoise(vec2 v) {

    // Precompute values for skewed triangular grid
    const vec4 C = vec4(0.211324865405187,
                        // (3.0-sqrt(3.0))/6.0
                        0.366025403784439,
                        // 0.5*(sqrt(3.0)-1.0)
                        -0.577350269189626,
                        // -1.0 + 2.0 * C.x
                        0.024390243902439);
                        // 1.0 / 41.0

    // First corner (x0)
    vec2 i  = floor(v + dot(v, C.yy));
    vec2 x0 = v - i + dot(i, C.xx);

    // Other two corners (x1, x2)
    vec2 i1 = vec2(0.0);
    i1 = (x0.x > x0.y)? vec2(1.0, 0.0):vec2(0.0, 1.0);
    vec2 x1 = x0.xy + C.xx - i1;
    vec2 x2 = x0.xy + C.zz;

    // Do some permutations to avoid
    // truncation effects in permutation
    i = mod289(i);
    vec3 p = permute(
            permute( i.y + vec3(0.0, i1.y, 1.0))
                + i.x + vec3(0.0, i1.x, 1.0 ));

    vec3 m = max(0.5 - vec3(
                        dot(x0,x0),
                        dot(x1,x1),
                        dot(x2,x2)
                        ), 0.0);

    m = m*m ;
    m = m*m ;

    // Gradients:
    //  41 pts uniformly over a line, mapped onto a diamond
    //  The ring size 17*17 = 289 is close to a multiple
    //      of 41 (41*7 = 287)

    vec3 x = 2.0 * fract(p * C.www) - 1.0;
    vec3 h = abs(x) - 0.5;
    vec3 ox = floor(x + 0.5);
    vec3 a0 = x - ox;

    // Normalise gradients implicitly by scaling m
    // Approximation of: m *= inversesqrt(a0*a0 + h*h);
    m *= 1.79284291400159 - 0.85373472095314 * (a0*a0+h*h);

    // Compute final noise value at P
    vec3 g = vec3(0.0);
    g.x  = a0.x  * x0.x  + h.x  * x0.y;
    g.yz = a0.yz * vec2(x1.x,x2.x) + h.yz * vec2(x1.y,x2.y);
    return 130.0 * dot(m, g);
}

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
        return mix(frequencyBands[0], frequencyBands[1], UV.y / 0.125);
    }
    if(UV.y <= 0.25){
        return mix(frequencyBands[1], frequencyBands[2], (UV.y - 0.125) / 0.125);
    }
    if(UV.y <= 0.375){
        return mix(frequencyBands[2], frequencyBands[3], (UV.y - 0.25) / 0.125);
    }
    if(UV.y <= 0.5){
        return mix(frequencyBands[3], frequencyBands[4], (UV.y - 0.375) / 0.125);
    }
    if(UV.y <= 0.625){
        return mix(frequencyBands[4], frequencyBands[5], (UV.y - 0.5) / 0.125);
    }
    if(UV.y <= 0.75){
        return mix(frequencyBands[5], frequencyBands[6], (UV.y - 0.625) / 0.125);
    }
    if(UV.y <= 0.875){
        return mix(frequencyBands[6], frequencyBands[7], (UV.y - 0.75) / 0.125);
    }
	if(UV.y <= 1.0){
		return frequencyBands[7];
	}
}

void main()
{
	float speed = time * scrollSpeed;
	// translation for noise and grid scrolling animation
	vec2 translate = vec2(0.0, speed);
	
	// speed used for vertex translation, this will let the vertex move along the z axis and then return to its original position
	float speedFrac = fract(speed) * 0.1;
	//speedFrac = 0;
	
	// the noise is also traslated by multiplying the floor of the translation with an offset depending on the zoom value (offset = zoom * 10^-2)
	// the resulting translation let the noise move row by row along the grid.
	vec2 noisePos = UV * zoom - floor(translate) * (zoom / 100.0);
	float noised = fbm(noisePos);
	
	// we add the street space into the grid by smoothing the noise
	noised *= 1.0 - (smoothstep(0.5 - streetSize - fade, 0.5 - streetSize, UV.x) - smoothstep(0.5 + streetSize, 0.5 + streetSize + fade, UV.x));

	float displacement = (noised * DisplaceByFBands()) * dPower;
	
	vec3 displacedPosition = position + displacement * normal;
	displacedPosition.z += speedFrac * 50; // translate the vertex position in order to achieve the movement illusion
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
