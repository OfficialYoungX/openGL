#define GLEW_STATIC
#include<GL/glew.h>

#include<GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include"SOIL2/SOIL2.h"
#include "SOIL2/stb_image.h"

#include<iostream>
#include <cmath>

#include "shader.h"
#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
using namespace std;
//设置窗口大小
const GLfloat WIDTH = 1000, HEIGHT = 800;
GLfloat lastX = WIDTH / 2;
GLfloat lastY = HEIGHT / 2;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool keys[1024];
Camera camera(glm::vec3(1.0f,1.0f,1.0f)); // 相机初始化
bool firstMouse = true; 

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, GLdouble x, GLdouble y);
void DoMovement(GLFWwindow *window);
void MouseScrollCallback(GLFWwindow *window, GLdouble xoffset, GLdouble yoffset);
void MousePosition(GLFWwindow *window, GLdouble x, GLdouble y);
void BackToStatus();

glm::vec3 lightPos_1 = glm::vec3(1.0f,0.0f,2.0f); // 光源1位置
glm::vec3 lightPos_2 = glm::vec3(-1.0f,0.0f,2.0f); // 光源2位置

//camera.Position =

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
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "3D-DCA-B16040517", nullptr, nullptr);
    int screenWidth, screenHeight;
    //读取窗口实际上的缓存空间，得到实际大小
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glfwSetKeyCallback(window, KeyCallback); // 注册键盘事件
    glfwSetCursorPosCallback(window, MouseCallback); // 注册鼠标d移动事件
    glfwSetScrollCallback(window, MouseScrollCallback); // 注册鼠标滚动事件
//    glfwSetCursorPosCallback(window, MousePosition);
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
    //设置视图大小，起点（前两个参数）及视图宽高，默认情况下为占据整个打开窗口的像素矩形
    glViewport(0, 0, screenWidth, screenHeight);
    
    Shader lightingShader = Shader("shader/core.vs", "shader/core.frag");
    Shader lampShader_1 = Shader("shader/light.vs", "shader/light.frag");
    Shader lampShader_2 = Shader("shader/light2.vs", "shader/light2.frag");
    
    GLfloat vertices[] = {
        //position_1             // 法向量
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    
    GLuint VBO, containerVAO;
    glGenVertexArrays(1, &containerVAO);
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(containerVAO);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    
    // Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
    GLuint lightVAO[2];
    glGenVertexArrays(1, &lightVAO[0]);
    glBindVertexArray(lightVAO[0]);
    // We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Set the vertex attributes (only position data for the lamp))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the normal vectors
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    
    // The second light
    glGenVertexArrays(1, &lightVAO[1]);
    glBindVertexArray(lightVAO[1]);
    // We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Set the vertex attributes (only position data for the lamp))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the normal vectors
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        DoMovement(window);
        
        // Clear the colorbuffer
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        
        
        // change the light_1 position
        float radius = 2.0f;
        lightPos_1.x = radius * sin(glfwGetTime());
        lightPos_1.z = radius * cos(glfwGetTime());
        
        // change the light_2 position
        lightPos_2.y = radius * sin(glfwGetTime());
        // Use cooresponding shader when setting uniforms/drawing objects
        lightingShader.Use();
        GLint objectColorLoc = glGetUniformLocation(lightingShader.Program, "objectColor");
        GLint lightColorLoc_1  = glGetUniformLocation(lightingShader.Program, "lightColor_1");
        GLint lightColorLoc_2  = glGetUniformLocation(lightingShader.Program, "lightColor_2");
        GLint lightPosLoc_1    = glGetUniformLocation(lightingShader.Program, "lightPos_1");
        GLint lightPosLoc_2    = glGetUniformLocation(lightingShader.Program, "lightPos_2");
        glUniform3f(objectColorLoc, 1.0f, 1.0f, 1.0f); // 白色物体
        glUniform3f(lightColorLoc_1,  1.0f, 1.0f, 1.0f); // 白色光源
        glUniform3f(lightColorLoc_2,  0.0f, 1.0f, 1.0f); // 青色光源
        glUniform3f(lightPosLoc_1,    lightPos_1.x, lightPos_1.y, lightPos_1.z);
        glUniform3f(lightPosLoc_2,    lightPos_2.x, lightPos_2.y, lightPos_2.z);
        
        
        // Create camera transformations
        glm::mat4 view;
        view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc  = glGetUniformLocation(lightingShader.Program,  "view");
        GLint projLoc  = glGetUniformLocation(lightingShader.Program,  "projection");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program,  "viewPos");
        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewPosLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniform3f(viewPosLoc,camera.Position.x,camera.Position.y,camera.Position.z);
        
        // Draw the container (using container's vertex attributes)
        glBindVertexArray(containerVAO);
        glm::mat4 model;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        
        // ------------------------------------------------------------------------------------
        // Also draw the lamp object, again binding the appropriate shader
        lampShader_1.Use();
        // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
        modelLoc = glGetUniformLocation(lampShader_1.Program, "model");
        viewLoc  = glGetUniformLocation(lampShader_1.Program, "view");
        projLoc  = glGetUniformLocation(lampShader_1.Program, "projection");
        // Set matrices
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        model = glm::mat4();
        model = glm::translate(model, lightPos_1);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        // Draw the light object (using light's vertex attributes)
        glBindVertexArray(lightVAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        
        // ------------------------------------------------------------------------------------
        
        lampShader_2.Use();
        // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
        modelLoc = glGetUniformLocation(lampShader_2.Program, "model");
        viewLoc  = glGetUniformLocation(lampShader_2.Program, "view");
        projLoc  = glGetUniformLocation(lampShader_2.Program, "projection");
        // Set matrices
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        model = glm::mat4();
        model = glm::translate(model, lightPos_2);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        // Draw the light object (using light's vertex attributes)
        glBindVertexArray(lightVAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        
        // ------------------------------------------------------------------------------------
        
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void MousePosition(GLFWwindow *window, GLdouble x, GLdouble y){
    lightPos_1.x = x/WIDTH;
    lightPos_1.y = y/HEIGHT;
}

void MouseCallback(GLFWwindow *window, GLdouble x, GLdouble y){
    if(firstMouse)
    {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }
    
    GLfloat xoffset = x - lastX;
    GLfloat yoffset = lastY - y;  // Reversed since y-coordinates go from bottom to left
    
    lastX = x;
    lastY = y;
    
    camera.ProcessMouseMovement(xoffset*3, yoffset*2);
}

void MouseScrollCallback(GLFWwindow *window, GLdouble xoffset, GLdouble yoffset){
    camera.ProcessMouseScroll(yoffset);
}

void BackToStatus(){
    camera.Position = glm::vec3(1.0f,1.0f,1.0f); // reset the camera position
    camera.Zoom = 45.0f; // reset the zoom value
}

void DoMovement(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        BackToStatus();
}
