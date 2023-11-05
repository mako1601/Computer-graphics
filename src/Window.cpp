#include "Window.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#define M_PI 3.14159265f

Window::Window(GLsizei _width, GLsizei _height, const std::string& _title) {
    width = _width;
    height = _height;
    title = _title;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window1" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    // for render text
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Resolution: " << width << "x" << height << std::endl;
}

void Window::timing() {
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void Window::keyCallback(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    float cameraSpeed = 4.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.pos += cameraSpeed * camera.up;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.pos -= cameraSpeed * camera.up;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.pos -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.pos += glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    }
}

void Window::renderBackgroundColor() {
    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

std::vector<GLfloat> Window::pointVertex(float x, float y) {
    std::vector<GLfloat> vertex;

    vertex.push_back(x);
    vertex.push_back(y);

    for (int i = 0; i <= 20; i++) {
        vertex.push_back(x + 0.025f * cos(2.0f * M_PI * i / 20.0f));
        vertex.push_back(y + 0.025f * sin(2.0f * M_PI * i / 20.0f));
    }
    return vertex;
}

void Window::render(GLsizeiptr size, const void* data, GLenum mode, GLsizei count) {
    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glDrawArrays(mode, 0, count);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Window::translate() {
    for (size_t i = 0, j = 0; i < points.size(); i++, j += 2) {
        points.at(i) = pointVertex(pointArray.at(j) + vectorVertex[0], pointArray.at(j + 1) + vectorVertex[1]);

        pointArray.at(j)     += vectorVertex[0];
        pointArray.at(j + 1) += vectorVertex[1];
    }
}

void Window::scale(float scale) {
    glm::vec2 normalizedVector(glm::normalize(glm::vec2(vectorVertex[0], vectorVertex[1])));
    glm::mat2 transformationMatrix = {
        scale * normalizedVector.x, 0.0f,
        0.0f, scale * normalizedVector.y
    };

    for (size_t i = 0, j = 0; i < points.size(); i++, j += 2) {
        glm::vec2 vector(pointArray.at(j), pointArray.at(j + 1));
        glm::vec2 result = transformationMatrix * vector;

        points.at(i) = pointVertex(result.x, result.y);

        pointArray.at(j)     = result.x;
        pointArray.at(j + 1) = result.y;
    }
}

void Window::reflection(float(&point)[2]) {
    glm::mat2 reflectionMatrix(-1.0f);

    for (size_t i = 0, j = 0; i < points.size(); i++, j += 2) {
        if (pointArray.at(j) != point[0] && pointArray.at(j + 1) != point[1]) {
            glm::vec2 vector(pointArray.at(j) - point[0], pointArray.at(j + 1) - point[1]);
            glm::vec2 result = reflectionMatrix * vector;

            points.at(i) = pointVertex(result.x + point[0], result.y + point[1]);

            pointArray.at(j) = result.x + point[0];
            pointArray.at(j + 1) = result.y + point[1];
        }
    }
}

void Window::rotation(float(&point)[2], float angle) {
    glm::mat2 rotationMatrix = {
        cos(glm::radians(angle)), -sin(glm::radians(angle)),
        sin(glm::radians(angle)),  cos(glm::radians(angle))
    };

    for (size_t i = 0, j = 0; i < points.size(); i++, j += 2) {
        if (pointArray.at(j) != point[0] && pointArray.at(j + 1) != point[1]) {
            glm::vec2 vector(pointArray.at(j) - point[0], pointArray.at(j + 1) - point[1]);
            glm::vec2 result = rotationMatrix * vector;

            points.at(i) = pointVertex(result.x + point[0], result.y + point[1]);
            
            pointArray.at(j)     = result.x + point[0];
            pointArray.at(j + 1) = result.y + point[1];
        }
    }
}

void Window::frameBuffersizeCallback(int width, int height) {
    glViewport(0, 0, width, height);
    if (width || height) {
        this->width = width;
        this->height = height;
    }
}

void Window::scrollCallback(double xoffset, double yoffset) {
    camera.worldScale -= static_cast<float>(yoffset) * 0.03f;
    if (camera.worldScale < 0.03f)
        camera.worldScale = 0.03f;
    if (camera.worldScale > 1.4f)
        camera.worldScale = 1.4f;
}

void Window::mouseButtonCallback(int button, int action, int mods) {
    // to skip mouseButtonCallback if we clicked on ImGui
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) { return; }
    
    double xpos;
    double ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        // normalize * ratio * world scale (* aspect) + camera pos
        // BUG: if worldScale != 1 the farther the cursor is from the center of the window, the more the point moves away from the cursor
        camera.cursor.x = ((xpos / width) * 2.0f - 1.0f)  * 10.0f * camera.worldScale * (static_cast<float>(width) / static_cast<float>(height)) + camera.pos.x;
        camera.cursor.y = (1.0f - (ypos / height) * 2.0f) * 10.0f * camera.worldScale + camera.pos.y;
        if (pointMode) {
            pointArray.push_back(camera.cursor.x);
            pointArray.push_back(camera.cursor.y);
            points.push_back(pointVertex(camera.cursor.x, camera.cursor.y));
        }
        else if (vectorMode) {
            vectorVertex = pointVertex(camera.cursor.x, camera.cursor.y);
        }
        else {
            for (size_t i = 0; i < pointArray.size(); i += 2) {
                if (camera.cursor.x - 0.075f < pointArray.at(i)     && camera.cursor.x + 0.075f > pointArray.at(i) &&
                    camera.cursor.y - 0.075f < pointArray.at(i + 1) && camera.cursor.y + 0.075f > pointArray.at(i + 1)) {
                    selectedPoint[0] = pointArray.at(i);
                    selectedPoint[1] = pointArray.at(i + 1);
                    break;
                }
            }
        }

    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE && pointArray.size() > 0) {
        if (pointMode) {
            pointArray.pop_back();
            pointArray.pop_back();
            points.pop_back();
        }
    }
}

void Window::staticFrameBuffersizeCallback(GLFWwindow* window, int width, int height) {
    Window* windowInstance = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (windowInstance) {
        windowInstance->frameBuffersizeCallback(width, height);
    }
}

void Window::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Window* windowInstance = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (windowInstance) {
        windowInstance->scrollCallback(xoffset, yoffset);
    }
}

void Window::staticMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Window* windowInstance = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (windowInstance) {
        windowInstance->mouseButtonCallback(button, action, mods);
    }
}