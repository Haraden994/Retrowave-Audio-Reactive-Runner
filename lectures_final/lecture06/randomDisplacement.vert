/*

00_basic.vert : basic Vertex shader

author: Davide Gadia

Real-Time Graphics Programming - a.a. 2018/2019
Master degree in Computer Science
Universita' degli Studi di Milano

*/


#version 330 core

// vertex position in world coordinates
layout (location = 0) in vec3 position;

// model matrix
uniform mat4 modelMatrix;
// view matrix
uniform mat4 viewMatrix;
// Projection matrix
uniform mat4 projectionMatrix;

void main()
{
    // transformations are applied to each vertex
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0f);
}
