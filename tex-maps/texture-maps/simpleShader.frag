#version 330

in vec3 ver_color;
in vec2 ver_uv;

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;

out vec4 frag_color;

void main(void)
{
    vec4 color1 = texture(u_texture1, ver_uv);
    vec4 color2 = texture(u_texture2, ver_uv);

    frag_color = color1 + color2;
}

