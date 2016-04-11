#version 330

in vec3 position;
in vec3 color;
out vec3 ver_color;

void main(void)
{
    vec3 pos = position;
    gl_Position = vec4(pos, 1.0);
    ver_color = color;
}

