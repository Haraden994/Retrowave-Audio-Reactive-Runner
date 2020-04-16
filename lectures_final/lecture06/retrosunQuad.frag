#version 330 core

// output shader variable
out vec4 fragColor;

// UV texture coordinates, interpolated in each fragment by the rasterization process
in vec2 interp_UV;

uniform float u_time;

float circle(in vec2 st, in float radius, in float edge){
    vec2 dist = st - vec2(0.5);
    return 1.0 - smoothstep(radius - (radius * edge),
                         radius + (radius * edge),
                         dot(dist , dist) * 4.0);
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

void main()
{
    vec2 texelSize = vec2(1.0 / interp_UV.x, 1.0 / interp_UV.y);
    
    float sunPct = circle(interp_UV, 0.5, 0.04);
    
    // sun haze
    float sunHazePct = circle(interp_UV, 0.7, 0.35);
    fragColor = vec4(vec3(0.909, 0.167, 0.596) * sunHazePct, sunHazePct * 0.35);
    
    // sun color
    // get a 0 -> 1 value within our sun
    float sunValue = smoothstep(0.3, 0.63, interp_UV.y);
    // line size increases the closer we are to the bottom of the sun
    float lineSize = mix(0.3, 0.0, interp_UV.y);
    
    // lerp between our sun colors to get a gradient
    vec4 sunColor = vec4(mix(vec3(0.909, 0.167, 0.596), vec3(1, 0.913, 0.305), sunValue), 1.0);
    
    // line speed ( 1 - to make it go down)
    float lineInt = 1.0 - fract(u_time * 0.3);
    
    // line count
    float lineY = fract(interp_UV.y * 14.0);
    
    // select our lines, invert so that active lines = 0
    float lines = 1.0 - line(lineInt, lineSize, 0.05, lineY);
    
    // cutoff the top lines
    lines = max(lines, step(0.6, interp_UV.y));
    
    sunPct *= lines;
    
    if(fragColor.rgb == vec3(0.0, 0.0, 0.0)){
    	discard;
    }
    
    // Add all together
    fragColor = ((1.0 - sunPct) * fragColor) + (sunColor * sunPct);
}