#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "ShaderProgram.h"
#include "Window.h"

int main() {
    // window init
    Window window(800, 800, "OpenGL 4.6");

    // callbacks
    glfwSetWindowUserPointer(window.pWindow, &window);
    glfwSetFramebufferSizeCallback(window.pWindow, Window::staticFrameBuffersizeCallback);
    glfwSetCursorPosCallback(window.pWindow, Window::staticCursorPosCallback);
    glfwSetMouseButtonCallback(window.pWindow, Window::staticMouseButtonCallback);
    glfwSetScrollCallback(window.pWindow, Window::staticScrollCallback);

    // imgui init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
    io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_ViewportsEnable;
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(window.pWindow, true);

    // shaders
    ShaderProgram pyramidShader("..\\resources\\shader.vs", "..\\resources\\shader.fs");

    GLfloat pyramidVertices[] = {
        // positions         // colors
        // base
         0.5f, 0.0f,  0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, 0.0f, -0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, 0.0f, -0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f, 0.0f, -0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f, 0.0f,  0.5f,  1.0f, 1.0f, 0.0f,
         0.5f, 0.0f,  0.5f,  0.0f, 1.0f, 0.0f,

        // side
         0.5f, 0.0f, -0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, 0.0f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.0f, 1.0f,  0.0f,  1.0f, 0.0f, 1.0f,

        -0.5f, 0.0f, -0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f, 0.0f,  0.5f,  1.0f, 1.0f, 0.0f,
         0.0f, 1.0f,  0.0f,  1.0f, 0.0f, 1.0f,

        -0.5f, 0.0f,  0.5f,  1.0f, 1.0f, 0.0f,
         0.5f, 0.0f,  0.5f,  0.0f, 1.0f, 0.0f,
         0.0f, 1.0f,  0.0f,  1.0f, 0.0f, 1.0f,

         0.5f, 0.0f,  0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, 0.0f, -0.5f,  0.0f, 0.0f, 1.0f,
         0.0f, 1.0f,  0.0f,  1.0f, 0.0f, 1.0f,
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    pyramidShader.use();

    //glPolygonMode(GL_FRONT_AND_BACK , GL_LINE);

    // render loop
    while (!glfwWindowShouldClose(window.pWindow)) {
        // per-frame time logic
        window.timing();

        // input
        window.keyCallback(window.pWindow);

        /* Render */
        window.renderBackgroundColor();

        glm::mat4 projection = glm::perspective(glm::radians(window.Camera.Zoom), static_cast<float>(window.Width) / static_cast<float>(window.Height), 0.001f, 100.0f);
        glm::mat4 view       = glm::lookAt(window.Camera.Position, window.Camera.Position + window.Camera.Front, window.Camera.Up);
        glm::mat4 model      = glm::mat4(1.0f);
        model = glm::rotate(model, static_cast<float>(glfwGetTime()) / 5.0f, glm::vec3(0.0f, 0.5f, 0.0f));

        pyramidShader.use();
        pyramidShader.setMat4("projection", projection);
        pyramidShader.setMat4("view", view);
        pyramidShader.setMat4("model", model);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 18);
        /*        */

        /* Interface */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Menu");
        ImGui::ColorEdit3("Background Color", window.BackgroundColor);
        ImGui::Separator();
        float cameraPos[3] = { window.Camera.Position.x, window.Camera.Position.y, window.Camera.Position.z };
        ImGui::InputFloat3("Camera Position", cameraPos, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat("FOV", &window.Camera.Zoom, 40.0f, 140.0f, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::Separator();
        ImGui::Text("FPS:"); ImGui::SameLine();
        ImGui::Text(std::to_string(1.0f / window.DeltaTime).c_str());
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        /* --------- */

        glfwSwapBuffers(window.pWindow);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(pyramidShader.ID);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}