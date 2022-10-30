#version 330 core

layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
 
uniform float xOffset;
uniform float yOffset;
uniform float sizeMod;
out vec3 ourColor; // output a color to the fragment shader

void main()
{
    vec2 scale = vec2(aPos.x * sizeMod, aPos.y * sizeMod);
    vec4 position = vec4(scale.x + xOffset, scale.y + yOffset, aPos.z, 1.0);
    gl_Position = position;
    ourColor = aColor; // set ourColor to the input color we got from the vertex data
}       