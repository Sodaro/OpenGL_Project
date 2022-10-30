#version 410
out vec4 FragColor;
uniform vec4 color_a;
uniform vec4 color_b;
uniform float lerp_value;

void main()
{
    vec4 color = mix(color_a,color_b,lerp_value);
    FragColor = color;
} 