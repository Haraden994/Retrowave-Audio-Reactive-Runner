/*

05_uv2color.frag: the Fragment shader assign to the fragment a color based on the UV texture coordinates of the vertex (U and V are saved in R and G color channels)
author: Davide Gadia

Real-Time Graphics Programming - a.a. 2018/2019
Master degree in Computer Science
Universita' degli Studi di Milano

*/

#version 330 core

// output shader variable
out vec4 colorFrag;

// UV texture coordinates, interpolated in each fragment by the rasterization process
in vec2 interp_UV;

void main(){
	// we set UV as RG channels, we set B at 0
	colorFrag = vec4(interp_UV,0.0,1.0);
}
