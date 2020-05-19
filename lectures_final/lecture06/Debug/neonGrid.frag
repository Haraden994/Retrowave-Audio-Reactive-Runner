#version 330

in vec2 interp_UV;

in vec3 lightDir;
in vec3 vNormal;
in vec3 vViewPosition;
in vec3 vPosition;

out vec4 outColor;

// lighting components color
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 ambientColor;
// weights
uniform float Kd;
uniform float Ks;
uniform float Ka;
// attenuation parameters
uniform float constant;
uniform float linear;
uniform float quadratic;
// shininess coefficient
uniform float shininess;

const vec3 gridColor = vec3(1.0, 0.0, 1.0);
const float edgeThickness = 2.1;
const float edgeSharpness = 20.0;
const float edgeSubtract = 0.3;
const float glowStrength = 10.0;
// the model is composed of 99x99 faces, we need to multiply each of the UV coordinate by the size of the model to achieve the amount of zoom needed to draw the fragment grid.
const float gridZoom = 99.0;

uniform float streetSize;
uniform float fade;
uniform float frequencyBands[];

vec3 Grid(){
	vec2 st = vec2(interp_UV * gridZoom);
	st = fract(st);
	st = abs(st - 0.5) * edgeThickness;
	st = pow(st, vec2(edgeSharpness)) - edgeSubtract;
	
	float c = clamp(st.x + st.y, 0.0, 1.0) * glowStrength;
	/*float street = step(streetSize, st.x);
	street *= step(streetSize, 1.0 - st.x);
	c *= 1.0 - (smoothstep(0.5 - streetSize - fade, 0.5 - streetSize, interp_UV.x) - smoothstep(0.5 + streetSize, 0.5 + streetSize + fade, interp_UV.x));
	c *= street;*/
	return c * gridColor;
}

vec3 deepBass;
vec3 bass;
vec3 mediumBass;
vec3 medium;
vec3 mediumHigh;
vec3 high;
vec3 veryHigh;
vec3 over;

// method to visualize the eight zones relative to the eight frequency bands
vec3 BandsColor(){
    if(interp_UV.y <= 0.125){
        return mix(deepBass, bass, interp_UV.y / 0.125);
    }
    if(interp_UV.y <= 0.25){
        return mix(bass, mediumBass, (interp_UV.y - 0.125) / 0.125);
    }
    if(interp_UV.y <= 0.375){
        return mix(mediumBass, medium, (interp_UV.y - 0.25) / 0.125);
    }
    if(interp_UV.y <= 0.5){
        return mix(medium, mediumHigh, (interp_UV.y - 0.375) / 0.125);
    }
    if(interp_UV.y <= 0.625){
        return mix(mediumHigh, high, (interp_UV.y - 0.5) / 0.125);
    }
    if(interp_UV.y <= 0.75){
        return mix(high, veryHigh, (interp_UV.y - 0.625) / 0.125);
    }
    if(interp_UV.y <= 0.875){
        return mix(veryHigh, over, (interp_UV.y - 0.75) / 0.125);
    }
	if(interp_UV.y <= 1.0){
		return mix(over, deepBass, (interp_UV.y - 0.875) / 0.125);
	}
}

void main() 
{
	vec3 bgColor = Ka * ambientColor;
	
	//vec3 fNorm = normalize(vNormal);
	vec3 fNorm = normalize(cross(dFdx(vViewPosition), dFdy(vViewPosition)));
	float lightDistance = length(lightDir.xyz);
	vec3 lightIncidence = normalize(lightDir.xyz);
	
	float attenuation = 1.0 / (constant + linear * lightDistance + quadratic * (lightDistance*lightDistance));
	
	float lambert = max(dot(lightIncidence, fNorm), 0.0);
	
	if(lambert > 0.0)
	{
		vec3 viewVector = normalize(vViewPosition);
		vec3 halfVector = normalize(lightIncidence + viewVector);
		
		float specAngle = max(dot(halfVector, fNorm), 0.0);
		// compute the shininess
		float shine = pow(specAngle, shininess);
		// apply diffusive, specular and shininess components to the final color
		bgColor += vec3(Kd * lambert * diffuseColor + Ks * shine * specularColor);
		bgColor *= attenuation;
	}
	
   	outColor = vec4(bgColor + Grid(), 1.0f);
	// Uncomment all the following lines to see the 8 frequency UV areas
	/*
	vec3 deepBass = vec3(1.0, 0.0, 0.0);
	bass = vec3(1.0, 0.5, 0.0);
	mediumBass = vec3(1.0, 1.0, 0.0);
	medium = vec3(0.5, 1.0, 0.0);
	mediumHigh = vec3(0.0, 1.0, 0.0);
	high = vec3(0.0, 1.0, 0.5);
	veryHigh = vec3(0.0, 1.0, 1.0);
	over = vec3(0.0, 0.0, 1.0);
	outColor = vec4(BandsColor(), 1.0f);
	*/
}