#include <stdio.h>
#include <OpenGL/gl3.h>
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include "ShaderLoader.h"

GLuint vao;
GLuint vbo[2];
GLuint indexBufferID;

ShaderLoader shaderLoader;


//设置三角形
void initTriangle(void)
{
    //顶点是从上往下，从左往右的顺序排列。
    GLfloat vertex_data[12];
    vertex_data[0] = -0.35f;    vertex_data[1] =  0.30f;  vertex_data[2] = 0;
    vertex_data[3] = 0.35f;     vertex_data[4] = 0.30f;  vertex_data[5] = 0;
    vertex_data[6] = -0.35f;    vertex_data[7] = -0.30f; vertex_data[8] = 0;
    vertex_data[9] = 0.35f;     vertex_data[10] = -0.30f;  vertex_data[11] = 0;
    
    
    GLfloat v_color_data[12];
    v_color_data[0] = 1;    v_color_data[1] = 0;    v_color_data[2] = 0;
    v_color_data[3] = 0;    v_color_data[4] = 1;    v_color_data[5] = 0;
    v_color_data[6] = 0;    v_color_data[7] = 0;    v_color_data[8] = 1;
    v_color_data[9] = 1;    v_color_data[10] = 1;   v_color_data[11] = 0;
    
    
    //索引顺序，从上而下，从右往左.逆时针
    GLuint vertex_indices[6];
    vertex_indices[0] = 0;    vertex_indices[1] = 1;    vertex_indices[2] = 2;
    vertex_indices[3] = 2;    vertex_indices[4] = 1;    vertex_indices[5] = 3;

    //1，生成指定的VBO信息
    //顶点信息
    glGenBuffers(2, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
    
    //颜色信息
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v_color_data), v_color_data, GL_STATIC_DRAW);
    
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);
    
    //2，生成VAO，并绑定VBO信息
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexBufferID);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

}

//设置并使用着色器
void initShader(void)
{
    shaderLoader.load("/Users/wistoneqqx/Documents/opengl/opengl-study-records/triangel_index_gl9/triangel_index/triangel_index/simpleShader.vert",
                      "/Users/wistoneqqx/Documents/opengl/opengl-study-records/triangel_index_gl9/triangel_index/triangel_index/simpleShader.frag");
    shaderLoader.bind();
}

//画三角形
void drawVBO(void)
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
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
    
    initTriangle();
    initShader();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        drawVBO();
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

