#include <stdio.h>
#include <OpenGL/gl3.h>
#define GLFW_INCLUDE_GLCOREARB
//define GLFW_INCLUDE_GLCOREARB to avoid glfw include gl.h, which is conflict with gl3.h
//ref http://stackoverflow.com/questions/19825582/modern-opengl-with-glfw3-on-mac-osx-10-8-4-missing-glgenvertexarrays-and-glb
#include <GLFW/glfw3.h>
#include "ShaderLoader.h"

GLuint vaoId;
GLuint vboIds[2];
ShaderLoader shader;

//设置三角形
void initTriangle(void)
{
    //三角形顶点
    GLfloat vertex_data[] = {
        -1.0f,-1.0f,0.0f,
        1.0f,-1.0f,0.0f,
        0.0f,1.0f,0.0f,
    };
    
    //三角形颜色
    GLfloat color_data[] = {
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    };
    
    glGenBuffers(2, vboIds);
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color_data), color_data, GL_STATIC_DRAW);
    
    glGenVertexArrays(1,&vaoId);
    glBindVertexArray(vaoId);
    
        GLuint locationPosition =  glGetAttribLocation (shader.getProgramID(), "position");
        GLuint locationColor =  glGetAttribLocation (shader.getProgramID(), "color");

        glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
        glVertexAttribPointer(locationPosition,3,GL_FLOAT,GL_FALSE,0,0);
    
        glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
        glVertexAttribPointer(locationColor,3,GL_FLOAT,GL_FALSE,0,0);

        glEnableVertexAttribArray(locationPosition);
        glEnableVertexAttribArray(locationColor);
    
    glBindVertexArray(0);
}

/**
 * 加载并使用着色器
*/
void initShader(void)
{
    shader.load("/Users/wistoneqqx/Documents/github/sbxfc/OpenGLShaderDemo/draw-triangle/simpleShader.vert",
                "/Users/wistoneqqx/Documents/github/sbxfc/OpenGLShaderDemo/draw-triangle/simpleShader.frag");
    shader.bind();
}

/*
 * 绘制三角形
*/
void drawTriangle(void)
{
    glBindVertexArray(vaoId);
    glDrawArrays(GL_TRIANGLES, 0, 3);
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
    window = glfwCreateWindow(320, 240, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
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

