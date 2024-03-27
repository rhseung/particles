#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <cstdio>
#include <cstdlib>
#include <string>

typedef struct rgb {
    float r, g, b;
} rgb;

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }
}

int main() {
    int xpos, ypos, height;
    const char* description;

    if (!glfwInit()) {
        glfwGetError(&description);
        printf("Error: %s\n", description);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &xpos, &ypos, nullptr, &height);

    const int size = height / 4;
    const rgb color = { 0.95f, 0.32f, 0.11f };
    const float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_FALSE);
    glfwWindowHint(GLFW_POSITION_X, xpos + size);
    glfwWindowHint(GLFW_POSITION_Y, ypos + size);

    GLFWwindow *window = glfwCreateWindow(size, size, "example", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if (!window) {
        glfwGetError(&description);
        printf("Error: %s\n", description);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(color.r, color.g, color.b, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    glfwTerminate();
    return 0;
}