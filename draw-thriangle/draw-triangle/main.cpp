#include <stdio.h>
#include <OpenGL/gl3.h>
#define GLFW_INCLUDE_GLCOREARB
//define GLFW_INCLUDE_GLCOREARB to avoid glfw include gl.h, which is conflict with gl3.h
//ref http://stackoverflow.com/questions/19825582/modern-opengl-with-glfw3-on-mac-osx-10-8-4-missing-glgenvertexarrays-and-glb
#include <GLFW/glfw3.h>
#include "ShaderLoader.h"
#include "SimpleShader.h"

GLuint vao;
GLuint vbo[2];

#define ATTRIB_LOCATION_VERTEX 1
#define ATTRIB_LOCATION_COLOR 2

void initShader(void){
    ShaderLoader shader;
    shader.initFromString(vert_shader, frag_shader);
    shader.bind();
}

void initData(void)
{
    //三角形的顶点坐标
    GLfloat vertexPosition[] = {
        -1.0f,-1.0f,0.0f, //第一个顶点坐标
        1.0f,-1.0f,0.0f,  //第二个顶点坐标
        0.0f,1.0f,0.0f,   //第三个顶点坐标
    };
    
    
    //三角形的顶点颜色
    GLfloat vertexColor[] = {
        1.0, 0.0, 0.0,  //第一个顶点颜色的RGB值
        0.0, 1.0, 0.0,  //第二个顶点颜色的RGB值
        0.0, 0.0, 1.0,  //第三个顶点颜色的RGB值
    };
    
    //创建顶点坐标和颜色的VBO对象
    glGenBuffers(2, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPosition), vertexPosition, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColor), vertexColor, GL_STATIC_DRAW);
    
    //创建VAO
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    
        glEnableVertexAttribArray(ATTRIB_LOCATION_VERTEX);
        glEnableVertexAttribArray(ATTRIB_LOCATION_COLOR);
    
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glVertexAttribPointer(ATTRIB_LOCATION_VERTEX,3,GL_FLOAT,GL_FALSE,0,0);
    
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glVertexAttribPointer(ATTRIB_LOCATION_COLOR,3,GL_FLOAT,GL_FALSE,0,0);
    
    glBindVertexArray(0);
}

/**
 * 绘制三角形
 */
void drawTriangle(void)
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

int main(void)
{
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    //使用OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(200, 200, "绘制三角形", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    initData();
    initShader();
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
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

