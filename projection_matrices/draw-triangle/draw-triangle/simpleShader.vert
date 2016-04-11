#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 ver_color;
uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 View;

void main(void)
{
    vec4 tranformVector = vec4(position, 1.0);
    gl_Position = tranformVector * Model * Projection * View;
    
    ver_color = color;
}

