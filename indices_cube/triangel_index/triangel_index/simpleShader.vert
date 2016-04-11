#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 mat_view;
uniform mat4 mat_proj;
uniform mat4 mat_rot;

out vec3 ver_color;

void main(void)
{
    vec4 pos = vec4(position,1);
    gl_Position = mat_proj * mat_view * mat_rot * pos;
    
    ver_color = color;
}

