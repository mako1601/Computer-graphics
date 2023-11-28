#include "Window.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

Window::Window(GLsizei width, GLsizei height, const std::string& title) {
    Width = width;
    Height = height;
    Title = title;

    BackgroundColor[0] = 1.0f;
    BackgroundColor[1] = 1.0f;
    BackgroundColor[2] = 1.0f;

    FirstMouse = true;
    EnableCursor = true;
    DeltaTime = 0.0f;
    LastFrame = 0.0f;
    LastX = Width / 2.0f;
    LastY = Height / 2.0f;

    SelectedFace[0] = glm::vec3(0.0f);
    SelectedFace[1] = glm::vec3(0.0f);
    SelectedFace[2] = glm::vec3(0.0f);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    pWindow = glfwCreateWindow(Width, Height, Title.c_str(), nullptr, nullptr);
    if (!pWindow) {
        std::cout << "Failed to create GLFW window1" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glEnable(GL_DEPTH_TEST);

    // for render text
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Resolution: " << Width << "x" << Height << std::endl;
}

void Window::timing() {
    float currentFrame = static_cast<float>(glfwGetTime());
    DeltaTime = currentFrame - LastFrame;
    LastFrame = currentFrame;
}

void Window::keyCallback(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (!EnableCursor) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            Camera.ProcessKeyboard(Movement::FORWARD, DeltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            Camera.ProcessKeyboard(Movement::BACKWARD, DeltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            Camera.ProcessKeyboard(Movement::LEFT, DeltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            Camera.ProcessKeyboard(Movement::RIGHT, DeltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            Camera.ProcessKeyboard(Movement::UP, DeltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            Camera.ProcessKeyboard(Movement::DOWN, DeltaTime);
        }
    }
}

void Window::renderBackgroundColor() {
    glClearColor(BackgroundColor[0], BackgroundColor[1], BackgroundColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::frameBuffersizeCallback(int width, int height) {
    glViewport(0, 0, width, height);
    if (width || height) {
        Width = width;
        Height = height;
    }
}

void Window::cursorPosCallback(double xpos_, double ypos_) {
    if (EnableCursor) { return; }

    float xpos = static_cast<float>(xpos_);
    float ypos = static_cast<float>(ypos_);

    if (FirstMouse) {
        LastX = xpos;
        LastY = ypos;
        FirstMouse = false;
    }

    float xoffset = xpos - LastX;
    float yoffset = LastY - ypos;

    LastX = xpos;
    LastY = ypos;

    Camera.ProcessMouseMovement(xoffset, yoffset);
}

void Window::mouseButtonCallback(int button, int action, int mods) {
    // to skip mouseButtonCallback if we clicked on ImGui
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) { return; }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        if (EnableCursor) {
            glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            FirstMouse = true;
            EnableCursor = false;
        }
        else {
            glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            EnableCursor = true;
        }
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !EnableCursor) {
        float closestDist = -1.0f;

        for (size_t i = 0; i < Vertices.size();) {
            glm::vec3 point1 = Vertices.at(i++);
            glm::vec3 point2 = Vertices.at(i++);
            glm::vec3 point3 = Vertices.at(i++);

            float dist = distanceToIntersection(point1, point2, point3);
            if (dist >= 0 && (closestDist < 0 || dist < closestDist)) {
                closestDist = dist;
                SelectedFace[0] = point1;
                SelectedFace[1] = point2;
                SelectedFace[2] = point3;
            }
        }
    }
}

float Window::distanceToIntersection(const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& point3) {
    glm::vec3 edge1 = point2 - point1;
    glm::vec3 edge2 = point3 - point1;
    glm::vec3 h = glm::cross(Camera.Front, edge2);
    float a = glm::dot(edge1, h);

    // ray is parallel to the polygon or lies on it
    if (a > -0.00001f && a < 0.00001f) {
        return -1;
    }

    float f = 1.0f / a;
    glm::vec3 s = Camera.Position - point1;
    float u = f * glm::dot(s, h);

    // intersection point lies outside the edge point1-point2
    if (u < 0.0f || u > 1.0f) {
        return -1;
    }

    glm::vec3 q = glm::cross(s, edge1);
    float v = f * glm::dot(Camera.Front, q);

    // intersection point lies outside the edge point1-point3 or point2-point3
    if (v < 0.0f || u + v > 1.0f) {
        return -1;
    }

    float t = f * glm::dot(edge2, q);

    if (t > 0.00001f) {
        return t; // ray intersects polygon
    }
    else {
        return -1; // ray is directed in the opposite direction or lies on the polygon
    }
}

glm::vec3 Window::translate(const float distance) {
    glm::vec3 edge1 = SelectedFace[1] - SelectedFace[0];
    glm::vec3 edge2 = SelectedFace[2] - SelectedFace[0];
    glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

    return distance * normal;
}

glm::vec3 Window::scale(const float scaleFactor, float(&point1)[3], float(&point2)[3], float(&point3)[3]) {
    glm::vec3 p1 = glm::vec3(point1[0], point1[1], point1[2]);
    glm::vec3 p2 = glm::vec3(point2[0], point2[1], point2[2]);
    glm::vec3 p3 = glm::vec3(point3[0], point3[1], point3[2]);
    glm::vec3 normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor)) - glm::mat4(1.0f);
    glm::vec3 scaledNormal = scale * glm::vec4(normal, 0.0f);

    return glm::vec3(1.0f) + scaledNormal;
}

glm::mat4 Window::reflect() {
    glm::vec3 edge1 = SelectedFace[1] - SelectedFace[0];
    glm::vec3 edge2 = SelectedFace[2] - SelectedFace[0];
    glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

    return glm::mat4(
         1.0f - 2.0f * normal.x * normal.x, -2.0f * normal.x * normal.y,        -2.0f * normal.x * normal.z,        0.0f,
        -2.0f * normal.y * normal.x,         1.0f - 2.0f * normal.y * normal.y, -2.0f * normal.y * normal.z,        0.0f,
        -2.0f * normal.z * normal.x,        -2.0f * normal.z * normal.y,         1.0f - 2.0f * normal.z * normal.z, 0.0f,
         0.0f,                               0.0f,                               0.0f,                              1.0f
    );
}

glm::mat4 Window::rotate(const float angle, float(&point1)[3], float(&point2)[3]) {
    glm::vec3 p1 = glm::vec3(point1[0], point1[1], point1[2]);
    glm::vec3 p2 = glm::vec3(point2[0], point2[1], point2[2]);
    glm::vec3 direction = glm::normalize(p2 - p1);

    return glm::rotate(glm::mat4(1.0f), glm::radians(angle), direction);
}

void Window::scrollCallback(double xoffset, double yoffset) {
    Camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void Window::staticFrameBuffersizeCallback(GLFWwindow* window, int width, int height) {
    Window* windowInstance = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (windowInstance) {
        windowInstance->frameBuffersizeCallback(width, height);
    }
}

void Window::staticCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    Window* windowInstance = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (windowInstance) {
        windowInstance->cursorPosCallback(xpos, ypos);
    }
}

void Window::staticMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Window* windowInstance = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (windowInstance) {
        windowInstance->mouseButtonCallback(button, action, mods);
    }
}

void Window::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Window* windowInstance = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (windowInstance) {
        windowInstance->scrollCallback(xoffset, yoffset);
    }
}