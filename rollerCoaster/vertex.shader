
#version 330 core

layout (location = 0) in vec3 positionPoints;
layout (location = 1) in vec3 colour;


uniform mat4 projection_matrix;

out vec3 outColour;

void main()
{
vec3 basePosition = positionPoints;


gl_Position = vec4(basePosition, 1.0);


outColour = colour;
}
