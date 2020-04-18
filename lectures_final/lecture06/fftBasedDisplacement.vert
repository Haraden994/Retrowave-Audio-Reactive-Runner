#version 330 core

// vertex position in world coordinates
layout (location = 0) in vec3 position;
// vertex normal in world coordinate
layout (location = 1) in vec3 normal;
// UV texture coordinates
layout (location = 2) in vec2 UV;

// the output variable for UV coordinates
out vec2 interp_UV;

// model matrix
uniform mat4 modelMatrix;
// view matrix
uniform mat4 viewMatrix;
// Projection matrix
uniform mat4 projectionMatrix;

//set of magnitude per band values
uniform float deepbass;
uniform float bass;
uniform float mediumbass;
uniform float medium1;
uniform float medium2;
uniform float mediumhigh;
uniform float high;
uniform float veryhigh;

//small boost
float displacementPower=0.3;

float displaceFFT(){
    if(UV.y<=0.125){
        return mix(deepbass,bass,UV.y/0.125);
    }
    if(UV.y<=0.25){
        return mix(bass,mediumbass,(UV.y-0.125)/0.125);
    }
    if(UV.y<=0.375){
        return mix(mediumbass,medium1,(UV.y-0.25)/0.125);   
    }
    if(UV.y<=0.5){
        return mix(medium1,medium2,(UV.y-0.375)/0.125);   
    }
    if(UV.y<=0.625){
        return mix(medium2,mediumhigh,(UV.y-0.5)/0.125);   
    }
    if(UV.y<=0.75){
        return mix(mediumhigh,high,(UV.y-0.625)/0.125);   
    }
    if(UV.y<=0.875){
        return mix(high,veryhigh,(UV.y-0.75)/0.125);   
    }
}

void main(){
	interp_UV = UV;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position+displaceFFT()*displacementPower*normal, 1.0f);
	//gl_Position = vec4(position+displaceFFT()*displacementPower*normal, 1.0 );
}
