#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 ver_color;

uniform vec2 stepxy;

void main(void)
{
    vec3 pos = position;
    pos.x += stepxy.x/20;
    pos.y += stepxy.y/20;
    
    gl_Position = vec4(pos, 1.0);
    
    ver_color = color;
}

