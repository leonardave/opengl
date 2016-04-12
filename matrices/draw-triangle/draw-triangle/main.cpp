#include <stdio.h>
#include <OpenGL/gl3.h>
#define GLFW_INCLUDE_GLCOREARB
//define GLFW_INCLUDE_GLCOREARB to avoid glfw include gl.h, which is conflict with gl3.h
//ref http://stackoverflow.com/questions/19825582/modern-opengl-with-glfw3-on-mac-osx-10-8-4-missing-glgenvertexarrays-and-glb
#include <GLFW/glfw3.h>
#include "ShaderLoader.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

GLuint vao;
GLuint vbo[2];

#define ATTRIB_POSITION_LOCATION 0
#define ATTRIB_COLOR_LOCATION 1

ShaderLoader shaderLoader;

glm::mat4 rotateY;

GLfloat degrees;

//设置三角形
void initTriangle(void)
{
    degrees = 0.0f;
    
    static const GLfloat g_vertex_buffer_data[] = {
		-0.8f,-0.8f,-0.8f,
		-0.8f,-0.8f, 0.8f,
		-0.8f, 0.8f, 0.8f,
        0.8f, 0.8f,-0.8f,
		-0.8f,-0.8f,-0.8f,
		-0.8f, 0.8f,-0.8f,
        0.8f,-0.8f, 0.8f,
		-0.8f,-0.8f,-0.8f,
        0.8f,-0.8f,-0.8f,
        0.8f, 0.8f,-0.8f,
        0.8f,-0.8f,-0.8f,
		-0.8f,-0.8f,-0.8f,
		-0.8f,-0.8f,-0.8f,
		-0.8f, 0.8f, 0.8f,
		-0.8f, 0.8f,-0.8f,
        0.8f,-0.8f, 0.8f,
		-0.8f,-0.8f, 0.8f,
		-0.8f,-0.8f,-0.8f,
		-0.8f, 0.8f, 0.8f,
		-0.8f,-0.8f, 0.8f,
        0.8f,-0.8f, 0.8f,
        0.8f, 0.8f, 0.8f,
        0.8f,-0.8f,-0.8f,
        0.8f, 0.8f,-0.8f,
        0.8f,-0.8f,-0.8f,
        0.8f, 0.8f, 0.8f,
        0.8f,-0.8f, 0.8f,
        0.8f, 0.8f, 0.8f,
        0.8f, 0.8f,-0.8f,
		-0.8f, 0.8f,-0.8f,
        0.8f, 0.8f, 0.8f,
		-0.8f, 0.8f,-0.8f,
		-0.8f, 0.8f, 0.8f,
        0.8f, 0.8f, 0.8f,
		-0.8f, 0.8f, 0.8f,
        0.8f,-0.8f, 0.8f
	};
    
	static const GLfloat g_color_buffer_data[] = {
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};
    
    //VBO
    glGenBuffers(2, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    
    //VAO
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glVertexAttribPointer(ATTRIB_POSITION_LOCATION,3,GL_FLOAT,GL_FALSE,0,0);
    
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glVertexAttribPointer(ATTRIB_COLOR_LOCATION,3,GL_FLOAT,GL_FALSE,0,0);

        glEnableVertexAttribArray(ATTRIB_POSITION_LOCATION);
        glEnableVertexAttribArray(ATTRIB_COLOR_LOCATION);
    
    glBindVertexArray(0);
}

//设置并使用着色器
void initShader(void)
{
    shaderLoader.load("/Users/wistoneqqx/Documents/github/sbxfc/OpenGLMatrices/draw-triangle/draw-triangle/simpleShader.vert",
                      "/Users/wistoneqqx/Documents/github/sbxfc/OpenGLMatrices/draw-triangle/draw-triangle/simpleShader.frag");
    shaderLoader.bind();
}

//画三角形
void drawTriangle(void)
{
    GLuint rotateLocation = glGetUniformLocation(shaderLoader.getProgramID(), "rotateY");
    if(rotateLocation != -1)
    {
        glUniformMatrix4fv(rotateLocation, 1, GL_FALSE, &rotateY[0][0]);
    }
    
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles
    glBindVertexArray(0);
}

//键盘处理
void keyboardCallbackHandler(GLFWwindow * window,int key, int x, int y,int z)
{
    if(key==68)
    {
        degrees += 5.0f;
    }
    else if(key==65)
    {
        degrees -= 5.0f;
    }


    //创建一个单位矩阵
    rotateY = glm::mat4(1.0);
    rotateY = glm::rotate(rotateY,degrees,glm::vec3(0.0f,1.0f,0.0f));
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
    glfwSetKeyCallback(window,keyboardCallbackHandler);
    
    initTriangle();
    initShader();
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        
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

