#pragma once

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<GL/glew.h>

class Shader {
    GLuint vertex, fragment;
    
public:
    GLuint Program;
    Shader(const GLchar *vertexPath, const GLchar *fragmentPath) {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        
        vShaderFile.exceptions(std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::badbit);
        
        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            
            
            std::stringstream vShaderStream, fShaderStream;
            
            
            
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            
        }
        catch (std::ifstream::failure e) {
            std::cout << "ERROR:SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        }
        
        
        
        //进行格式转换
        const GLchar *vShaderCode = vertexCode.c_str();
        const GLchar *fShaderCode = fragmentCode.c_str();
        
        // import and compile the shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);	// 加载字符串
        glCompileShader(vertex);	// 编译
        
        // 检查是否编译成功
        GLint success;
        GLchar infoLog[512];
        
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);	// 加载字符串
        glCompileShader(fragment);	// 编译
        
        // 检查是否编译成功
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        
        // create program and link it
        this->Program = glCreateProgram();
        glAttachShader(this->Program, vertex);	// 把目标文件加入
        glAttachShader(this->Program, fragment);
        glLinkProgram(this->Program);
        
        // 检查链接是否错误
        glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
            std::cout << "ERROR::PROGRAM::FRAGMENT::LINK_FAILED\n" << infoLog << std::endl;
        }
        
        // validate
        
        glValidateProgram(this->Program);
        glGetProgramiv(this->Program, GL_VALIDATE_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::VALIDATE\n" << infoLog << std::endl;
        }
        
        
    }
    ~Shader() {
        glDetachShader(this->Program, vertex);
        glDetachShader(this->Program, fragment);
        
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        
        glDeleteProgram(this->Program);
    }
    void Use() {
        glUseProgram(this->Program);
    }
    
};
