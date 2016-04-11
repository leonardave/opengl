#include <stdio.h>
#include <OpenGL/gl3.h>
#define GLFW_INCLUDE_GLCOREARB
//define GLFW_INCLUDE_GLCOREARB to avoid glfw include gl.h, which is conflict with gl3.h
//ref http://stackoverflow.com/questions/19825582/modern-opengl-with-glfw3-on-mac-osx-10-8-4-missing-glgenvertexarrays-and-glb
#include <GLFW/glfw3.h>
#include "ShaderLoader.h"

GLuint vao;
GLuint vbo[2];

#define ATTRIB_POSITION_LOCATION 0
#define ATTRIB_COLOR_LOCATION 1

ShaderLoader shaderLoader;

//三角形偏移
float step_xy[2];

//设置三角形
void initTriangle(void)
{
    GLfloat vertex_data[] = {
        -1.0f,-1.0f,0.0f,
        1.0f,-1.0f,0.0f,
        0.0f,1.0f,0.0f,
    };
    
    GLfloat color_data[] = {
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    };
    
    //VBO
    glGenBuffers(2, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color_data), color_data, GL_STATIC_DRAW);
    
    //VAO
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(ATTRIB_POSITION_LOCATION,3,GL_FLOAT,GL_FALSE,0,0);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(ATTRIB_COLOR_LOCATION,3,GL_FLOAT,GL_FALSE,0,0);

    glEnableVertexAttribArray(ATTRIB_POSITION_LOCATION);
    glEnableVertexAttribArray(ATTRIB_COLOR_LOCATION);
}

//设置并使用着色器
void initShader(void)
{
    
    shaderLoader.load("/Users/wistoneqqx/Documents/opengl/opengl-study-records/read-a-shader-file/draw-triangle/draw-triangle/simpleShader.vert",
                      "/Users/wistoneqqx/Documents/opengl/opengl-study-records/read-a-shader-file/draw-triangle/draw-triangle/simpleShader.frag");
    shaderLoader.bind();
}

//画三角形
void drawTriangle(void)
{
    //修改uniform的值
    GLuint location = glGetUniformLocation(shaderLoader.getProgramID(), "stepxy");
    if(location != -1)
    {
        glUniform2fv(location,  //指明要更改的uniform变量的位置
                     1,         //指明要更改的元素个数,如果目标uniform变量不是一个数组，那么这个值应该设为1；如果是数组，则应该设置为>=1。
                     step_xy    //用来更新指定的uniform变量。
                    );
    }
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

//键盘处理
void keyboardCallbackHandler(GLFWwindow * window,int key, int x, int y,int z)
{
    if(key==87)
    {
        step_xy[1] += 1;
    }
    else if(key==83)
    {
        step_xy[1] -= 1;
    }
    else if(key==68)
    {
        step_xy[0] +=1;
    }
    else if(key==65)
    {
        step_xy[0] -=1;
    }
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
    
    /*设置键盘事件监听*/
    step_xy[0] = 0.0f;
    step_xy[1] = 0.0f;
    glfwSetKeyCallback(window,keyboardCallbackHandler);
    
    initTriangle();
    initShader();
    
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

