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

    int cameraState = 1;
    float distance = 0.0f;
    glm::vec3 transferVec = glm::vec3(0.0f, 0.0f, 0.0f);
    float pointOfPlane1[3] = { 0.0f, 0.0f, 0.0f };
    float pointOfPlane2[3] = { 0.0f, 0.0f, 0.0f };
    float pointOfPlane3[3] = { 0.0f, 0.0f, 0.0f };
    float scaleFactor = 0.0f;
    glm::vec3 scaleVec = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::mat4 reflection = glm::mat4(1.0f);
    float pointOfLine1[3] = { 0.0f, 0.0f, 0.0f };
    float pointOfLine2[3] = { 0.0f, 0.0f, 0.0f };
    glm::mat4 rotation = glm::mat4(1.0f);
    float angle = 0.0f;

    glPointSize(2.0f);
    //glPolygonMode(GL_FRONT_AND_BACK , GL_LINE);

    // render loop
    while (!glfwWindowShouldClose(window.pWindow)) {
        // per-frame time logic
        window.timing();

        // input
        window.keyCallback(window.pWindow);

        /* Render */
        window.renderBackgroundColor();

        GLfloat crosshair[] = {
            window.Camera.Position.x + 0.01f * window.Camera.Front.x,
            window.Camera.Position.y + 0.01f * window.Camera.Front.y,
            window.Camera.Position.z + 0.01f * window.Camera.Front.z,
            0.0f, 0.0f, 0.0f
        };

        GLuint crosshairVBO, crosshairVAO;
        glGenVertexArrays(1, &crosshairVAO);
        glGenBuffers(1, &crosshairVBO);

        glBindVertexArray(crosshairVAO);

        glBindBuffer(GL_ARRAY_BUFFER, crosshairVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(crosshair), crosshair, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glm::mat4 projection;
        glm::mat4 view         = glm::lookAt(window.Camera.Position, window.Camera.Position + window.Camera.Front, window.Camera.Up);
        glm::mat4 model        = glm::mat4(1.0f);
        glm::mat4 pyramidModel = glm::translate(model, glm::vec3(0.75f, 0.0f, 0.75f));

        float ratio = static_cast<float>(window.Width) / static_cast<float>(window.Height);

        switch (cameraState) {
            case 1:
                projection = glm::perspective(glm::radians(window.Camera.Zoom), ratio, 0.001f, 100.0f); break;
            case 2:
                projection = glm::ortho(-1.0f * ratio, 1.0f * ratio, -1.0f, 1.0f, 0.001f, 100.0f); break;
        }

        pyramidModel = glm::translate(pyramidModel, transferVec);
        pyramidModel = glm::scale(pyramidModel, scaleVec);
        pyramidModel = pyramidModel * reflection;
        pyramidModel = rotation * pyramidModel;

        window.Vertices.clear();
        for (size_t i = 0; i < sizeof(pyramidVertices) / sizeof(GLfloat); i += 6) {
            window.Vertices.push_back(glm::vec3(pyramidVertices[i], pyramidVertices[i + 1], pyramidVertices[i + 2]));
            window.Vertices.back() = pyramidModel * glm::vec4(window.Vertices.back(), 1.0f);
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
        glBindVertexArray(crosshairVAO);
        glDrawArrays(GL_POINTS, 0, 1);

        renderAllText(text, projection, view);
        /*        */

        /* Interface */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Menu");
        ImGui::Text("Camera Mode");
        ImGui::RadioButton("Perspective", &cameraState, 1);
        ImGui::RadioButton("Orthographic", &cameraState, 2);
        ImGui::Separator();
        ImGui::Text("Selected Face");
        float A[3] = { window.SelectedFace[0].x, window.SelectedFace[0].y, window.SelectedFace[0].z };
        float B[3] = { window.SelectedFace[1].x, window.SelectedFace[1].y, window.SelectedFace[1].z };
        float C[3] = { window.SelectedFace[2].x, window.SelectedFace[2].y, window.SelectedFace[2].z };
        ImGui::InputFloat3("First Point", A, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat3("Second Point", B, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat3("Third Point", C, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::Separator();
        ImGui::Text("Translation");
        ImGui::InputFloat("Distance", &distance);
        if (ImGui::Button("Translate")) {
            transferVec += window.translate(distance);
        }
        ImGui::Separator();
        ImGui::Text("Scaling");
        ImGui::InputFloat3("A", pointOfPlane1);
        ImGui::InputFloat3("B", pointOfPlane2);
        ImGui::InputFloat3("C", pointOfPlane3);
        ImGui::InputFloat("Scale Factor", &scaleFactor);
        if (ImGui::Button("Scale")) {
            scaleVec = window.scale(scaleFactor, pointOfPlane1, pointOfPlane2, pointOfPlane3);
        }
        ImGui::Separator();
        ImGui::Text("Reflection");
        if (ImGui::Button("Reflect")) {
            reflection = window.reflect();
        }
        ImGui::Separator();
        ImGui::Text("Rotation");
        ImGui::InputFloat3("a", pointOfLine1);
        ImGui::InputFloat3("b", pointOfLine2);
        ImGui::SliderFloat("Angle", &angle, -360.0f, 360.0f);
        if (ImGui::Button("Rotate")) {
            rotation = window.rotate(angle, pointOfLine1, pointOfLine2);
        }
        ImGui::NewLine(); ImGui::NewLine(); ImGui::NewLine();
        ImGui::ColorEdit3("Background Color", window.BackgroundColor);
        ImGui::Separator();
        float cameraFront[3] = { window.Camera.Front.x, window.Camera.Front.y, window.Camera.Front.z };
        ImGui::InputFloat3("Front", cameraFront, "%.3f", ImGuiInputTextFlags_ReadOnly);
        float cameraPos[3] = { window.Camera.Position.x, window.Camera.Position.y, window.Camera.Position.z };
        ImGui::InputFloat3("Camera Position", cameraPos, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat("FOV", &window.Camera.Zoom, 40.0f, 140.0f, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::Text("Pitch"); ImGui::SameLine();
        ImGui::Text(std::to_string(window.Camera.Pitch).c_str());
        ImGui::Text("Yaw"); ImGui::SameLine();
        ImGui::Text(std::to_string(window.Camera.Yaw).c_str());
        ImGui::Separator();
        ImGui::Text("FPS:"); ImGui::SameLine();
        ImGui::Text(std::to_string(1.0f / window.DeltaTime).c_str());
        ImGui::End();

        ImGui::Begin("Vertices");
        float green[3]   = { window.Vertices.at(0).x, window.Vertices.at(0).y, window.Vertices.at(0).z };
        float blue[3]    = { window.Vertices.at(1).x, window.Vertices.at(1).y, window.Vertices.at(1).z };
        float red[3]     = { window.Vertices.at(2).x, window.Vertices.at(2).y, window.Vertices.at(2).z };
        float yellow[3]  = { window.Vertices.at(4).x, window.Vertices.at(4).y, window.Vertices.at(4).z };
        float magenta[3] = { window.Vertices.at(8).x, window.Vertices.at(8).y, window.Vertices.at(8).z };
        ImGui::InputFloat3("Green", green, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat3("Blue", blue, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat3("Red", red, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat3("Yellow", yellow, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat3("Magenta", magenta, "%.3f", ImGuiInputTextFlags_ReadOnly);
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

        glDeleteVertexArrays(1, &crosshairVAO);
        glDeleteBuffers(1, &crosshairVBO);

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