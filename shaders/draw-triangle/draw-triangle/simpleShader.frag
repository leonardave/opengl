#version 330

in vec3 ver_color;
out vec4 frag_color;

void main(void)
{
    frag_color = vec4(ver_color,1.0);
}

