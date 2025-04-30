#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>

// 全局变量
bool mousePressed = false;
double lastX = 0.0, lastY = 0.0;
float yaw = 45.0f;    // 初始水平角度
float pitch = 30.0f;  // 初始垂直角度
float radius = 5.0f;  // 相机离原点距离（轨迹球半径）

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            mousePressed = true;
            glfwGetCursorPos(window, &lastX, &lastY);
        } else if (action == GLFW_RELEASE) {
            mousePressed = false;
        }
    }
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    if (mousePressed) {
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.3f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // 限制pitch角，防止翻转
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }
}

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

    GLFWwindow* window = glfwCreateWindow(800, 600, "Gyro Viewer with Mouse Control", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window!" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);

    // 注册鼠标回调
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float timeNow = static_cast<float>(glfwGetTime());

        // 模型旋转（Z轴自旋）
        float angle = glm::radians(30.0f) * timeNow;
        glm::quat rotation = glm::angleAxis(angle, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 model = glm::toMat4(rotation);

        // 根据yaw/pitch算出camera位置
        float camX = radius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        float camY = radius * sin(glm::radians(pitch));
        float camZ = radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        glm::vec3 cameraPos = glm::vec3(camX, camY, camZ);

        glm::mat4 view = glm::lookAt(
            cameraPos,            // 相机位置
            glm::vec3(0.0f, 0.0f, 0.0f), // 目标原点
            glm::vec3(0.0f, 1.0f, 0.0f)  // 世界上方向
        );

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        float aspect = width / (float)height;

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(glm::value_ptr(projection));

        glMatrixMode(GL_MODELVIEW);
        glm::mat4 modelview = view * model;
        glLoadMatrixf(glm::value_ptr(modelview));

        drawAxis();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
