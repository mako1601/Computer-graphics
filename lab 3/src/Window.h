#ifndef _Window_h_
#define _Window_h_

#include <string>
#include <vector>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Camera.h"

class Window {
public:
    GLsizei Width;
    GLsizei Height;
    std::string Title;

    float BackgroundColor[3];

    bool FirstMouse;
    bool EnableCursor;
    float LastX;
    float LastY;

    float DeltaTime;
    float LastFrame;

    Camera Camera;

    GLFWwindow* pWindow;

    Window(GLsizei _width, GLsizei _height, const std::string& title);

    void timing();
    void keyCallback(GLFWwindow* window);
    void renderBackgroundColor();

    void frameBuffersizeCallback(int width, int height);
    void cursorPosCallback(double xpos, double ypos);
    void mouseButtonCallback(int button, int action, int mods);
    void scrollCallback(double xoffset, double yoffset);

    static void staticFrameBuffersizeCallback(GLFWwindow* window, int width, int height);
    static void staticCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void staticMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif