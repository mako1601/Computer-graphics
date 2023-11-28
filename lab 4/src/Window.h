#ifndef _Window_h_
#define _Window_h_

#include <string>
#include <vector>
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
    std::vector<glm::vec3> Vertices;
    glm::vec3 SelectedFace[3];

    GLFWwindow* pWindow;

    Window(GLsizei _width, GLsizei _height, const std::string& title);

    void timing();
    void keyCallback(GLFWwindow* window);
    void renderBackgroundColor();
    glm::vec3 translate(const float distance);
    glm::vec3 scale(const float scaleFactor, float(&point1)[3], float(&point2)[3], float(&point3)[3]);
    glm::mat4 reflect();
    glm::mat4 rotate(const float angle, float(&point1)[3], float(&point2)[3]);

    static void staticFrameBuffersizeCallback(GLFWwindow* window, int width, int height);
    static void staticCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void staticMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
    void frameBuffersizeCallback(int width, int height);
    void cursorPosCallback(double xpos, double ypos);
    void mouseButtonCallback(int button, int action, int mods);
    void scrollCallback(double xoffset, double yoffset);

    float Window::distanceToIntersection(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);
};

#endif