#version 330 core

out vec4 fragColor;

in vec2 interp_UV;

uniform vec3 color;
uniform int blink;
uniform float time;

void main(){
	vec3 startingColor = color;
	vec3 actualColor = color;
	float blinkSpeed = 20.0;
	float pct = abs(sin(time*blinkSpeed));
	vec3 blinkColor;
	if(blink == 1){
		blinkColor = vec3(0.0, 1.0, 0.0); //green blink for speed up
		actualColor = mix(startingColor, blinkColor, pct);
	}
	else if(blink == -1){
		blinkColor = vec3(1.0, 0.0, 0.0); //red blink for speed down
		actualColor = mix(startingColor, blinkColor, pct);
	}
	else
		actualColor = startingColor;

	fragColor = vec4(actualColor, 1.0);
	
}