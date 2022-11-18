#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

out vec3 Normal;
out vec3 FragPos;
out float SinVal;

void main()
{
    float limit = 0;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal; //fix lighting distortion when scaling objects
    float sinval = sin(time*0.5);
    float limitedval = sinval+1; //0:1
    SinVal = sinval;
    vec4 pos = projection * view * model * vec4(aPos.x + Normal.x * limitedval, max(limit, aPos.y - sinval), aPos.z + Normal.z * limitedval, 1.0);
    gl_Position = pos;
}