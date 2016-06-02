//
//  main.cpp
//  demo
//
//  Created by sbxfc on 16/6/2.
//  Copyright © 2016年 me.rungame.sbxfc. All rights reserved.
//

#include <stdio.h>
#include <OpenGL/gl3.h>
#define GLFW_INCLUDE_GLCOREARB
//define GLFW_INCLUDE_GLCOREARB to avoid glfw include gl.h, which is conflict with gl3.h
//ref http://stackoverflow.com/questions/19825582/modern-opengl-with-glfw3-on-mac-osx-10-8-4-missing-glgenvertexarrays-and-glb
#include <GLFW/glfw3.h>
#include "ShaderLoader.h"
#include "SimpleShader.h"

#define ATTRIB_LOCATION_VERTEX 1
#define ATTRIB_LOCATION_COLOR 2

GLuint vao;
GLuint vbo[2];

//初始化顶点数据
void initVertexData(void){
    
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
    
    //创建VBO
    glGenBuffers(2, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPosition), vertexPosition, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColor), vertexColor, GL_STATIC_DRAW);
    
    //创建VAO
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);//开始记录
    
    //glEnableVertexAttribArray 可以激活指定索引的顶点属性数组.
    //默认情况下,顶点属性数组是未被激活的,
    //当它们被激活之后才可以被glDrawArrays或glDrawElements这类函数所读取、渲染。
    glEnableVertexAttribArray(ATTRIB_LOCATION_VERTEX);
    glEnableVertexAttribArray(ATTRIB_LOCATION_COLOR);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(ATTRIB_LOCATION_VERTEX,3,GL_FLOAT,GL_FALSE,0,0);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(ATTRIB_LOCATION_COLOR,3,GL_FLOAT,GL_FALSE,0,0);
    
    glBindVertexArray(0);//记录结束
}

//初始化着色器
void initShader(void){
    ShaderLoader shader;
    shader.initFromString(vert_shader, frag_shader);
    shader.bind();
}

/**
 * 绘制三角形
 */
void draw(void)
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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    /*初始化三角形数据*/
    initVertexData();
    /*初始化着色器*/
    initShader();
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        draw();
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
