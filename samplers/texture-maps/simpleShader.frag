#version 330

in vec3 ver_color;
in vec2 ver_uv[2];

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform sampler2D u_texture3;

out vec4 frag_color;

void main(void)
{
    vec4 color1 = texture(u_texture1, ver_uv[0]);
    vec4 color2 = texture(u_texture2, ver_uv[0]);
    vec4 color3 = texture(u_texture3, ver_uv[1]);

    //把图上的黄色星星显示出来
    if(color3.r==0 && color3.g==0 && color3.b==0)
        frag_color = color1+color2 ;
    else
        frag_color = color3;
}

