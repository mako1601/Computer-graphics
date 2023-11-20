#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "ShaderProgram.h"
#include "TextRenderer.h"
#include "Window.h"
#include "Camera.h"

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

    GLfloat Ox[] = {
        0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
        2.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f
    };

    GLfloat Oy[] = {
        0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        0.0f, 2.0f, 0.0f,  0.0f, 1.0f, 0.0f
    };

    GLfloat Oz[] = {
        0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 2.0f,  0.0f, 0.0f, 1.0f
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

    GLuint xVBO, xVAO;
    glGenVertexArrays(1, &xVAO);
    glGenBuffers(1, &xVBO);

    glBindVertexArray(xVAO);

    glBindBuffer(GL_ARRAY_BUFFER, xVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Ox), Ox, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    GLuint yVBO, yVAO;
    glGenVertexArrays(1, &yVAO);
    glGenBuffers(1, &yVBO);

    glBindVertexArray(yVAO);

    glBindBuffer(GL_ARRAY_BUFFER, yVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Oy), Oy, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    GLuint zVBO, zVAO;
    glGenVertexArrays(1, &zVAO);
    glGenBuffers(1, &zVBO);

    glBindVertexArray(zVAO);

    glBindBuffer(GL_ARRAY_BUFFER, zVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Oz), Oz, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    pyramidShader.use();

    ShaderProgram text("..\\resources\\text.vs", "..\\resources\\text.fs");
    text.use();
    initFreeType();

    //glPolygonMode(GL_FRONT_AND_BACK , GL_LINE);

    int cameraState = 1;
    bool firstPerspective = true;
    glm::vec3 cameraPos;
    float yam;
    float pitch;
    float angle = 0.0f;

    // render loop
    while (!glfwWindowShouldClose(window.pWindow)) {
        // per-frame time logic
        window.timing();

        // input
        window.keyCallback(window.pWindow);

        /* Render */
        window.renderBackgroundColor();

        glm::mat4 projection;
        glm::mat4 view         = glm::lookAt(window.Camera.Position, window.Camera.Position + window.Camera.Front, window.Camera.Up);
        glm::mat4 model        = glm::mat4(1.0f);
        glm::mat4 pyramidModel = glm::translate(model, glm::vec3(0.75f, 0.0f, 0.75f));
        //pyramidModel           = glm::rotate(pyramidModel, static_cast<float>(glfwGetTime()) / 5.0f, glm::vec3(0.0f, 1.0f, 0.0f));

        float ratio = static_cast<float>(window.Width) / static_cast<float>(window.Height);

        switch (cameraState) {
            case 1: // perspective
                if (!firstPerspective) {
                    window.Camera.Position = cameraPos;
                    window.Camera.Yaw = yam;
                    window.Camera.Pitch = pitch;
                    window.Camera.updateCameraVectors();
                    firstPerspective = true;
                }
                cameraPos = window.Camera.Position;
                yam = window.Camera.Yaw;
                pitch = window.Camera.Pitch;
                projection = glm::perspective(glm::radians(window.Camera.Zoom), ratio, 0.001f, 100.0f);
                renderAllTextPerspective(text, projection, view);
                break;
            case 2: // front (ortho)
                if (firstPerspective) {
                    cameraPos = window.Camera.Position;
                    yam = window.Camera.Yaw;
                    pitch = window.Camera.Pitch;
                    firstPerspective = false;
                }
                projection = glm::ortho(-1.0f * ratio, 1.0f * ratio, -1.0f, 1.0f, 0.001f, 100.0f);
                window.Camera.Position = glm::vec3(0.75f, 0.5f, 10.0f);
                window.Camera.Yaw = -90.0f;
                window.Camera.Pitch = 0.0f;
                window.Camera.updateCameraVectors();
                renderAllTextFront(text, projection, view);
                break;
            case 3: // up (oblique)
                if (firstPerspective) {
                    cameraPos = window.Camera.Position;
                    yam = window.Camera.Yaw;
                    pitch = window.Camera.Pitch;
                    firstPerspective = false;
                }
                projection = glm::ortho(-1.0f * ratio, 1.0f * ratio, -1.0f, 1.0f, 0.001f, 100.0f);
                pyramidModel = glm::rotate(pyramidModel, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
                window.Camera.Position = glm::vec3(0.75f, 10.0f, 0.75f);
                window.Camera.Yaw = 0.0f;
                window.Camera.Pitch = -89.999f;
                window.Camera.updateCameraVectors();
                renderAllTextUp(text, projection, view);
                break;
            case 4: // side (ortho)
                if (firstPerspective) {
                    cameraPos = window.Camera.Position;
                    yam = window.Camera.Yaw;
                    pitch = window.Camera.Pitch;
                    firstPerspective = false;
                }
                projection = glm::ortho(-1.0f * ratio, 1.0f * ratio, -1.0f, 1.0f, 0.001f, 100.0f);
                window.Camera.Position = glm::vec3(10.0f, 0.5f, 0.75f);
                window.Camera.Yaw = -180.0f;
                window.Camera.Pitch = 0.0f;
                window.Camera.updateCameraVectors();
                renderAllTextSide(text, projection, view);
                break;
        }

        pyramidShader.use();
        pyramidShader.setMat4("projection", projection);
        pyramidShader.setMat4("view", view);
        pyramidShader.setMat4("model", pyramidModel);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 18);

        pyramidShader.setMat4("model", model);

        glBindVertexArray(xVAO);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(yVAO);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(zVAO);
        glDrawArrays(GL_LINES, 0, 2);
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
        ImGui::RadioButton("Perspective", &cameraState, 1);
        ImGui::RadioButton("Front", &cameraState, 2);
        ImGui::RadioButton("Up", &cameraState, 3); ImGui::SameLine();
        ImGui::SliderFloat("Angle", &angle, -180.0f, 180.0f);
        ImGui::RadioButton("Side", &cameraState, 4);
        ImGui::Separator();
        ImGui::Text("Pitch"); ImGui::SameLine();
        ImGui::Text(std::to_string(window.Camera.Pitch).c_str());
        ImGui::Text("Yaw"); ImGui::SameLine();
        ImGui::Text(std::to_string(window.Camera.Yaw).c_str());
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
    glDeleteVertexArrays(1, &xVAO);
    glDeleteBuffers(1, &xVBO);
    glDeleteVertexArrays(1, &yVAO);
    glDeleteBuffers(1, &yVBO);
    glDeleteVertexArrays(1, &zVAO);
    glDeleteBuffers(1, &zVBO);
    glDeleteProgram(pyramidShader.ID);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}