#ifndef _Window_h_
#define _Window_h_

#include <string>
#include <vector>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct Camera {
    glm::vec3 pos   { 0.0f, 0.0f, 24.2f };
    glm::vec3 front { 0.0f, 0.0f, -1.0f };
    glm::vec3 up    { 0.0f, 1.0f,  0.0f };

    float worldScale = 1.0f;

    glm::vec3 cursor{ 0.0f, 0.0f,  0.0f };
};

class Window {
public:
    GLsizei width{};
    GLsizei height{};
    std::string title{};

    float deltaTime{ 0.0f };
    float lastFrame{ 0.0f };

    Camera camera{};

    float backgroundColor[3]{ 1.0f, 1.0f, 1.0f };

    GLFWwindow* window{};

    bool pointMode{ true };
    bool vectorMode{ false };
    std::vector<GLfloat> pointArray{}; // array points (x, y)
    std::vector<std::vector<GLfloat>> points{}; // array for drawing points
    std::vector<GLfloat> vectorVertex{}; // array for drawing vector point (vectorVertex[0] and vectorVertex[1] - center of point)
    GLfloat selectedPoint[2]{ 0.0f, 0.0f };

    Window(GLsizei _width, GLsizei _height, const std::string& title);

    void timing();
    void keyCallback(GLFWwindow* window);
    void renderBackgroundColor();
    void drawPoint(float x, float y) {}
    std::vector<GLfloat> pointVertex(float x, float y);
    void render(GLsizeiptr size, const void* data, GLenum mode, GLsizei count);
    void translate();
    void scale(float scale);
    void reflection(float(&point)[2]);
    void rotation(float(&point)[2], float angle);

    void frameBuffersizeCallback(int width, int height);
    void scrollCallback(double xoffset, double yoffset);
    void mouseButtonCallback(int button, int action, int mods);

    static void staticFrameBuffersizeCallback(GLFWwindow* window, int width, int height);
    static void staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void staticMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};

#endif