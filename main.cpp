#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <cstdio>
#include <cstdlib>

int main() {
    int xpos, ypos, height;
    const char *description;
    GLFWwindow *windows[4];

    if (!glfwInit()) {
        glfwGetError(&description);
        printf("Error: %s\n", description);
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &xpos, &ypos, NULL, &height);

    for (int i = 0; i < 4; i++) {
        const int size = height / 5;
        const struct {
            float r, g, b;
        } colors[] =
                {
                    {0.95f, 0.32f, 0.11f},
                    {0.50f, 0.80f, 0.16f},
                    {0.f, 0.68f, 0.94f},
                    {0.98f, 0.74f, 0.04f}
                };

        if (i > 0)
            glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_FALSE);

        glfwWindowHint(GLFW_POSITION_X, xpos + size * (1 + (i & 1)));
        glfwWindowHint(GLFW_POSITION_Y, ypos + size * (1 + (i >> 1)));

        windows[i] = glfwCreateWindow(size, size, "Multi-Window Example", NULL, NULL);
        if (!windows[i]) {
            glfwGetError(&description);
            printf("Error: %s\n", description);
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwSetInputMode(windows[i], GLFW_STICKY_KEYS, GLFW_TRUE);

        glfwMakeContextCurrent(windows[i]);
        gladLoadGL(glfwGetProcAddress);
        glClearColor(colors[i].r, colors[i].g, colors[i].b, 1.f);
    }

    for (;;) {
        for (auto & window : windows) {
            glfwMakeContextCurrent(window);
            glClear(GL_COLOR_BUFFER_BIT);
            glfwSwapBuffers(window);

            if (glfwWindowShouldClose(window) ||
                glfwGetKey(window, GLFW_KEY_ESCAPE)) {
                glfwTerminate();
                exit(EXIT_SUCCESS);
            }
        }

        glfwWaitEvents();
    }
}
