//
//  ShaderLoader.cpp
//  draw-triangle
//
//  Created by sbx_fc on 14-8-25.
//  Copyright (c) 2014年 rungame. All rights reserved.
//

#include "ShaderLoader.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>

ShaderLoader::ShaderLoader():programID(0)
{
    
}

/**
 * 加载顶点、片段着色器文件
 * @param filename1 vertex shader
 * @param filename2 fragment shader
 *
 */
bool ShaderLoader::load(const char* filename1, const char* filename2)
{
    //创建着色器
    GLuint vsId, fsId;
    compile(&vsId, GL_VERTEX_SHADER, filename1);
    compile(&fsId, GL_FRAGMENT_SHADER, filename2);
    
    //创建程序容器
    programID = glCreateProgram();
    
    if(programID != 0){
        if (vsId){
            glAttachShader(programID, vsId);
        }
        
        if (fsId){
            glAttachShader(programID, fsId);
        }
    }
    
    return link();
}

/**
 * @sId 着色器ID
 * @shaderType 着色器类型
 * @fname 着色器文件地址
 */
bool ShaderLoader::compile(GLuint* sId, GLenum shaderType, const GLchar* fname)
{
    FILE* fp = fopen(fname,"rt");
    
    if(fp == NULL){
        
        printf("文件不存在!");
        return false;
    }
    
    std::vector<std::string> fLines;
    char sz[255];
    
    while( fgets(sz,255,fp) ){
        fLines.push_back( sz );
    }
    
    const char** codes = new const char*[fLines.size()];
    
    for(int i=0;i<int(fLines.size());i++){
        codes[i] = fLines[i].c_str();
    }

    
    //创建并返回着色器句柄
    auto shaderID = glCreateShader(shaderType);
    //替换着色器的源代码
    glShaderSource(
            shaderID,              //指定要替换源代码的Shader
            (GLint)fLines.size(),  //字符指针数组string中的元素个数
            codes,                 //字符指针数组
            NULL
        );
    
    //编译着色器
    glCompileShader(shaderID);
    
    delete[] codes;
    fLines.erase(fLines.begin(),fLines.end());
    
    //如果编译失败，获取相应的编译信息
    GLint succ;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &succ);
    if (succ == GL_FALSE)
    {
        GLsizei len = 0;
        GLchar *log;
        
        //获取着色器失败日志
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &len);
        log = (GLchar*)malloc(len);
        glGetShaderInfoLog(shaderID, len, &succ, log);
        
        /* print an error message and the info log */
        fprintf(stderr, "Unable to compile shader: log is%s\n", log);
        free(log);
        glDeleteShader(shaderID);
        return false;
    }
    
    *sId = shaderID;
    return succ == GL_TRUE;
}

 /*
 * 属性绑定到着色器制定位置。
 * 将顶点属性绑定到程序之后需要对程序进行连接。
 * @param index 绑定的属性的索引
 * @param name 绑定的属性名称
 */
void ShaderLoader::addAttribute(const char* name, GLuint index)
{
    glBindAttribLocation(programID, index, name);
}

/**
 *  连接程序容器，
 */
bool ShaderLoader::link()
{
    GLint result;
    
    /*
    *  在连接操作执行以后，你可以任意修改shader的源代码、对shader重新编译
    *  不会影响整个程序，除非重新连接程序
    */
    glLinkProgram(programID);
    
    if (_vertShader)
    {
        /*
        *  删除Shader,释放shader占用的内存,并使该shader失效
        *  但并非真正删除程序里的Shader
        *  删除程序里的shader参见
        *  void glDetachShader(GLuint program, GLuint shader);
        */
        glDeleteShader(_vertShader);
    }
    
    if (_fragShader)
    {
        glDeleteShader(_fragShader);
    }
    
    //获取链接状态
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    if(result == GL_FALSE)
    {
        GLint length;
        /* get the program info log */
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
        char* log = (char*)malloc(length);
        glGetProgramInfoLog(programID, length, &result, log);
        
        /* print an error message and the info log */
        fprintf(stderr, "Program linking failed: %s\n", log);
        free(log);
        
        /* delete the program */
        glDeleteProgram(programID);
        programID = 0;
    }
    _vertShader = _fragShader = 0;
    return result == GL_TRUE;
}

void ShaderLoader::bind()
{
    /*
     * 加载并使用连连接好的程序。
     * 如果程序已经在使用时候对程序进行重新编译，编译后的应用程序会自动替换以前被调用的程序，
     * 不需要重新调用此函数。
    */
    glUseProgram(programID);
}

unsigned int ShaderLoader::getProgramID()
{
    return programID;
}

void ShaderLoader::unbind()
{
    //使用固定功能管线
    glUseProgram(0);
}

ShaderLoader::~ShaderLoader()
{
    if(programID)
    {
        glDeleteProgram(programID);
    }
}

