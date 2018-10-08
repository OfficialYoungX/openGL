#define GLEW_STATIC
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include"SOIL2/SOIL2.h"
#include "SOIL2/stb_image.h"
#include "shader.h"
#include<iostream>
#include <cmath>
using namespace std;
//设置窗口大小
const GLfloat WIDTH = 1200, HEIGHT = 600;

int main()
{
    glfwInit();//glfw初始化
    //glfwWindowHint部分：设置一些关于窗口的选项，在进行绘制前的基本设置
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);// 设置OpenGL主版本号，使用版本为3以后的，所以参数设为3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);// 设置OpenGL副版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);// OpenGL模式，第二个参数表示核心模式
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);// for mac
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);// 窗口是否可调整大小，第二个参数表示不允许改变窗口大小
    // 创建窗口对象
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "texture B16040517", nullptr, nullptr);
    int screenWidth, screenHeight;
    //读取窗口实际上的缓存空间，得到实际大小
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    //判断窗口是否创建成功
    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //设置当前绘制窗口
    glfwMakeContextCurrent(window);
    //启动OpenGL并判断启动是否成功
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialise GLEW" << std::endl;
        return -1;
    }
    glViewport(0, 0, screenWidth, screenHeight);//设置视图大小，起点（前两个参数）及视图宽高，默认情况下为占据整个打开窗口的像素矩形
    Shader ourShader = Shader("shader/core.vs", "shader/core.frag");
    
    //now the vertex information
    /*GLfloat vertices_1[] =      //每一行坐标（x,y,z)为一个点的位置,取值为[-1,1],画点顺序逆时针
     {
     //first triangle position
     -0.5f,-0.5f,0.0f,
     0.5f,-0.5f,0.0f,
     -0.5f,0.5f,0.0f,
     //second  triangle
     0.5f,-0.5f,0.0f,
     0.5f,0.5f,0.0f,
     -0.5f,0.5f,0.0f,
     };*/
    
    const GLfloat textureWidth = 500;
    const GLfloat textureHeight = 500;
    const GLfloat texture_xoffset = 20;
    GLfloat texture_xoffset_rate = texture_xoffset/(WIDTH/2);
    GLfloat
    ver_1_x = -texture_xoffset_rate, ver_1_y = (textureWidth/2)/(HEIGHT/2),
    ver_2_x = -texture_xoffset_rate, ver_2_y = -(textureWidth/2)/(HEIGHT/2),
    ver_3_x = -(textureWidth/(WIDTH/2)+texture_xoffset_rate), ver_3_y = -(textureHeight/2)/(HEIGHT/2),
    ver_4_x = -(textureWidth/(WIDTH/2)+texture_xoffset_rate), ver_4_y = (textureHeight/2)/(HEIGHT/2);
    
    
    GLfloat vertices_1[] = {
        //position_1             // colors            //texture coords
        ver_1_x,ver_1_y,0.0f,    1.0f, 0.0f, 0.0f,    1.0f,1.0f, // 右上角
        ver_2_x,ver_2_y,0.0f,    0.0f, 1.0f, 0.0f,    1.0f,0.0f, // 右下角
        ver_3_x,ver_3_y,0.0f,    0.0f, 0.0f, 1.0f,    0.0f,0.0f, // 左下角
        ver_4_x,ver_4_y,0.0f,    0.0f, 0.0f, 1.0f,    0.0f,1.0f // 左上角
    };
    
    unsigned int indices_1[]{ // 索引
        0,1,3, // 第一个三角形
        1,3,2 // 第二个三角形
    };
    
    // second data
    
    GLfloat vertices_2[] = {
        //position_1             // colors            //texture coords
        -ver_1_x,ver_1_y,0.0f,    1.0f, 0.0f, 0.0f,    1.0f,1.0f, // 右上角
        -ver_2_x,ver_2_y,0.0f,    0.0f, 1.0f, 0.0f,    1.0f,0.0f, // 右下角
        -ver_3_x,ver_3_y,0.0f,    0.0f, 0.0f, 1.0f,    0.0f,0.0f, // 左下角
        -ver_4_x,ver_4_y,0.0f,    0.0f, 0.0f, 1.0f,    0.0f,1.0f // 左上角
    };
    
    unsigned int indices_2[]{ // 索引
        0,1,3, // 第一个三角形
        1,3,2 // 第二个三角形
    };
    
    GLuint VBO[2], VAO[2], EBO[2];//VBO 顶点缓冲对象
    
    
    glGenVertexArrays(1, &VAO[0]);
    glGenBuffers(1, &VBO[0]);
    glGenBuffers(1, &EBO[0]);
    // 1. 绑定顶点数组对象
    glBindVertexArray(VAO[0]);
    // 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW);
    // 3. 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_1), indices_1, GL_STATIC_DRAW);
    // 4. 设定顶点属性指针
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
//    ---------------------------------------------------------------------------------------------------
    
    glGenVertexArrays(1, &VAO[1]);
    glGenBuffers(1, &VBO[1]);
    glGenBuffers(1, &EBO[1]);
    // 1. 绑定顶点数组对象
    glBindVertexArray(VAO[1]);
    // 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_2), vertices_2, GL_STATIC_DRAW);
    // 3. 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_2), indices_2, GL_STATIC_DRAW);
    // 4. 设定顶点属性指针
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    -----------------------------------------------------------------------------------------------------
//
//    //Unbind
//    //transfer the index
//    GLuint EBO[2];
//    glGenBuffers(1, &EBO[0]); // 创建索引缓冲对象
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_1), indices_1, GL_STATIC_DRAW); // 把索引复制到缓冲里
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
////    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//----------------------------------------------------------------------------
    
// 贴图设置
    GLuint texture[2];
    
    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = SOIL_load_image("images/1.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // texture 2
    // ---------
    glGenTextures(1, &texture[1]);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = SOIL_load_image("images/2.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.Use(); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:
    glUniform1i(glGetUniformLocation(ourShader.Program, "texture1"), 0);
    glUniform1i(glGetUniformLocation(ourShader.Program, "texture2"), 1);
    
    //判断窗口是否关闭，不关闭继续执行
    while (!glfwWindowShouldClose(window))
    {
        const float color[] = {0.0f, 0.0f, 0.0f, 1.0f};
        glClearColor(color[0], color[1], color[2], color[3]);//R,G,B,Alpha
        glClear(GL_COLOR_BUFFER_BIT);//上色，背景颜色初始化
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        
        // render container
        ourShader.Use();
        
        
        GLuint alpha_localtion = glGetUniformLocation(ourShader.Program, "alpha");
        float timeValue = glfwGetTime();
        float alpha = sin(timeValue) / 2.0f + 0.5f;
        glUniform1f(alpha_localtion,alpha);
        glBindVertexArray(VAO[0]); // 绑定**
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO[0]);
    glDeleteBuffers(1, &VBO[0]);
    glDeleteBuffers(1, &EBO[0]);
    
    glfwTerminate();
    return 0;
}
