#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>           // GLEW一定要在最前面
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>    // 四元数
#include <glm/gtx/quaternion.hpp>    // 四元数转矩阵
#include <iostream>

void drawAxis() {
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    // X轴 - 红色
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);

    // Y轴 - 绿色
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    // Z轴 - 蓝色
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glEnd();
}

int main() {
    // 初始化GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(800, 600, "Gyro Viewer", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window!" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);

    // 初始化GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // 主循环
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 计算时间
        float timeNow = static_cast<float>(glfwGetTime());

        // 每秒旋转30度，绕Z轴
        float angle = glm::radians(30.0f) * timeNow;
        glm::quat rotation = glm::angleAxis(angle, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 rotationMatrix = glm::toMat4(rotation);
        glm::mat4 model = rotationMatrix;

        // 投影矩阵
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        float aspect = width / (float)height;
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

        // 观察矩阵
        glm::mat4 view = glm::lookAt(
            glm::vec3(2.0f, 2.0f, 2.0f),  // 相机位置
            glm::vec3(0.0f, 0.0f, 0.0f),  // 观察目标
            glm::vec3(0.0f, 1.0f, 0.0f)   // 上方向
        );

        // 把矩阵传给OpenGL
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(glm::value_ptr(projection));

        glMatrixMode(GL_MODELVIEW);
        glm::mat4 modelview = view * model;
        glLoadMatrixf(glm::value_ptr(modelview));

        // 绘制三维坐标轴
        drawAxis();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 清理资源
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
