#include <stdio.h>
#include <OpenGL/gl3.h>
#define GLFW_INCLUDE_GLCOREARB
//define GLFW_INCLUDE_GLCOREARB to avoid glfw include gl.h, which is conflict with gl3.h
//ref http://stackoverflow.com/questions/19825582/modern-opengl-with-glfw3-on-mac-osx-10-8-4-missing-glgenvertexarrays-and-glb
#include <GLFW/glfw3.h>
#include "ShaderLoader.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>

GLuint vao;
GLuint vbo[4];

#define ATTRIB_POSITION_LOCATION 0
#define ATTRIB_COLOR_LOCATION 1
#define ATTRIB_UV_LOCATION 2
#define ATTRIB_UV2_LOCATION 3

ShaderLoader shaderLoader;

//图片大小
GLint width,height;

//存储图片信息
unsigned char * imgBuf;
GLuint mTextureID;

//纹理ID
GLuint g_texture1;
GLuint g_texture2;
GLuint g_texture3;


//纹理设置
GLuint g_Sampler1;
GLuint g_Sampler2;
GLuint g_Sampler3;

//texture 坐标
GLuint textureLocation1;
GLuint textureLocation2;
GLuint textureLocation3;

//加载图片
//以RGBA格式存储图片
static bool LoadImg(const char* fname,GLint nID)
{
    
    //初始化FreeImage
    FreeImage_Initialise(TRUE);
    
    //定义图片格式为未知
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    
    //获取图片格式
    fif = FreeImage_GetFileType(fname,0);
    
    //根据获取格式读取图片数据
    FIBITMAP* bitmap = FreeImage_Load(fif,fname,0);
    
    if(!bitmap)
    {
        printf("load error!\n");
        return false;
    }
    
    int x,y;
    RGBQUAD m_rgb;
    
    //获取图片长宽
    width = (int)FreeImage_GetWidth(bitmap);
    height = (int)FreeImage_GetHeight(bitmap);
    
    imgBuf = new unsigned char[width*height*4];
    
    //获取图片数据
    //按RGBA格式保存到数组中
    for(y=0;y<height;y++)
    {
        for(x=0;x<width;x++)
        {
            //获取像素值
            FreeImage_GetPixelColor(bitmap,x,y,&m_rgb);
            
            //将RGB值存入数组
            imgBuf[y*width*4+x*4+0] = m_rgb.rgbRed;
            imgBuf[y*width*4+x*4+1] = m_rgb.rgbGreen;
            imgBuf[y*width*4+x*4+2] = m_rgb.rgbBlue;
            
            //判断是否透明图片
            //如果是就取alpha值保存
            if(FreeImage_IsTransparent(bitmap))
                imgBuf[y*width*4+x*4+3] = m_rgb.rgbReserved;
            else
                imgBuf[y*width*4+x*4+3] = 255;
        }
    }
    
    //绑定纹理ID
    
    if(nID==0)
    {
        glGenTextures(1, &g_texture1);
        glBindTexture(GL_TEXTURE_2D, g_texture1);
    }else if(nID==1)
    {
        glGenTextures(1, &g_texture2);
        glBindTexture(GL_TEXTURE_2D, g_texture2);
    }
    else if(nID==2)
    {
        glGenTextures(1, &g_texture3);
        glBindTexture(GL_TEXTURE_2D, g_texture3);
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgBuf);
    
    //释放FreeImage
    delete imgBuf;
    FreeImage_Unload(bitmap);
    return true;
}

//设置三角形
void initTriangle(void)
{
    //定义两张纹理的配置
    glGenSamplers(1,&g_Sampler1);
    glGenSamplers(1,&g_Sampler2);
    glGenSamplers(1,&g_Sampler3);
    
    glSamplerParameteri(g_Sampler1, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(g_Sampler1, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(g_Sampler1, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(g_Sampler1, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(g_Sampler2, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(g_Sampler2, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(g_Sampler2, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(g_Sampler2, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(g_Sampler3, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(g_Sampler3, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(g_Sampler3, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(g_Sampler3, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    //获取片断着色器中u_texture1和u_texture2指针
    textureLocation1 = glGetUniformLocation(shaderLoader.getProgramID(), "u_texture1");
    textureLocation2 = glGetUniformLocation(shaderLoader.getProgramID(), "u_texture2");
    textureLocation3 = glGetUniformLocation(shaderLoader.getProgramID(), "u_texture3");
    
    
    if(textureLocation1==-1 && textureLocation2 ==-1 && textureLocation3 == -1)
    {
        printf("Get textureLocation error!\n");
    }
    
    static const GLfloat g_vertex_buffer_data[] = {
		-0.4f,0.4f,0.0f,
        -0.4f,-0.6f,0.0f,
        0.4f,0.4f,0.0f,
        0.4f,-0.6f,0.0f
	};
    
	static const GLfloat g_color_buffer_data[] = {
		1,1,1,
        1,1,1,
        1,1,1,
        1,1,1
	};
    
    static const GLfloat uv_data[] = {
        0.0f,1.0f,
        0.0f,0.0f,
        1.0f,1.0f,
        1.0f,0.0f,
    };
    
    static const GLfloat uv_data2[] = {
        0.0f,4.0f,
        0.0f,0.0f,
        4.0f,4.0f,
        4.0f,0.0f,
    };
    
    //1，生成指定的VBO信息
    //顶点信息
    glGenBuffers(4, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    //颜色信息
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    
    //UV信息
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv_data), uv_data, GL_STATIC_DRAW);
    
    //UV2信息
    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv_data2), uv_data2, GL_STATIC_DRAW);
    
    //2，生成VAO，并绑定VBO信息
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glVertexAttribPointer(ATTRIB_POSITION_LOCATION,3,GL_FLOAT,GL_FALSE,0,0);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glVertexAttribPointer(ATTRIB_COLOR_LOCATION,3,GL_FLOAT,GL_FALSE,0,0);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
        glVertexAttribPointer(ATTRIB_UV_LOCATION,2,GL_FLOAT,GL_FALSE,0,0);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
        glVertexAttribPointer(ATTRIB_UV2_LOCATION,2,GL_FLOAT,GL_FALSE,0,0);
        
        glEnableVertexAttribArray(ATTRIB_POSITION_LOCATION);
        glEnableVertexAttribArray(ATTRIB_COLOR_LOCATION);
        glEnableVertexAttribArray(ATTRIB_UV_LOCATION);
        glEnableVertexAttribArray(ATTRIB_UV2_LOCATION);

    glBindVertexArray(0);
    
    
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g_texture1);
    glUniform1i(textureLocation1, 0);
    glBindSampler(0, g_Sampler1);
    
    glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g_texture2);
    glUniform1i(textureLocation2, 1);
    glBindSampler(1, g_Sampler2);
    
    glActiveTexture(GL_TEXTURE2);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g_texture3);
    glUniform1i(textureLocation3, 2);
    glBindSampler(2, g_Sampler3);
}

//设置并使用着色器
void initShader(void)
{
    shaderLoader.load("/Users/wistoneqqx/Documents/github/sbxfc/OpenGLSamplers/texture-maps/simpleShader.vert",
                      "/Users/wistoneqqx/Documents/github/sbxfc/OpenGLSamplers/texture-maps/simpleShader.frag");
    shaderLoader.bind();
}

//画三角形
void drawTriangle(void)
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

}

int main(void)
{
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    //使用OpenGL 3.2
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    if(false==LoadImg("/Users/wistoneqqx/Documents/github/sbxfc/OpenGLSamplers/texture-maps/test01.jpg",0))
        printf("加载图片1失败！\n");
    
    if(false==LoadImg("/Users/wistoneqqx/Documents/github/sbxfc/OpenGLSamplers/texture-maps/test02.jpg",1))
        printf("加载图片2失败！\n");
    
    if(false==LoadImg("/Users/wistoneqqx/Documents/github/sbxfc/OpenGLSamplers/texture-maps/test00.jpg",2))
        printf("加载图片2失败！\n");

    
    initShader();
    initTriangle();
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        drawTriangle();
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

