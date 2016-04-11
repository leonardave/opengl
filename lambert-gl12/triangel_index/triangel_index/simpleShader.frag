#version 330

in vec3 ver_color;
in vec3 ver_normal;

out vec4 frag_color;

void main(void)
{
    //灯的位置
    vec3 lightPos = vec3(-2.0f,0.5f,10.0f);
    vec3 lightColor = vec3(1,1,1);
    
    float f = 0.6f;
    float af = 0.4f;//环境光系数
    float val = max(0.0, dot(normalize(ver_normal), normalize(lightPos))) * f;
 
    vec3 color1 = ver_color * af;//环境光
    vec3 color2 = lightColor * val;//漫反射
    
    vec3 color = color1 + color2;
    
    frag_color = vec4(color, 1.0);
}

