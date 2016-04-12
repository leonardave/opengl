#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec2 uv2;

out vec3 ver_color;
out vec2 ver_uv[2];

void main(void)
{
    gl_Position = vec4(position, 1.0);
    
    ver_color = color;
    ver_uv[0] = uv;
    ver_uv[1] = uv2;
}

