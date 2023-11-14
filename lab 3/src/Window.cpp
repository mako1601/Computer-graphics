#include "Window.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
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

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
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