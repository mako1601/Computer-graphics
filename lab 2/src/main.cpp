#include <iomanip>
#include <sstream>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "ShaderProgram.h"
#include "DrawFuntions.h"
#include "Window.h"

int main() {
    // window init
    Window Window1(800, 800, "OpenGL 4.6");

    // callbacks
    glfwSetWindowUserPointer(Window1.window, &Window1);
    glfwSetFramebufferSizeCallback(Window1.window, Window::staticFrameBuffersizeCallback);
    glfwSetScrollCallback(Window1.window, Window::staticScrollCallback);
    glfwSetMouseButtonCallback(Window1.window, Window::staticMouseButtonCallback);

    // imgui init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
    io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_ViewportsEnable;
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(Window1.window, true);

    // shaders
    ShaderProgram pen("..\\resources\\shader.vs", "..\\resources\\shader.fs");
    pen.use();
    drawCartesian();

    ShaderProgram text("..\\resources\\text.vs", "..\\resources\\text.fs");
    text.use();
    initFreeType();

    std::stringstream ss;
    float scaleFactor = 0.0f;
    float angle = 0.0f;
    float pointColor[] = { 0.1f, 0.69f, 0.28f };

    //glPolygonMode(GL_FRONT_AND_BACK , GL_LINE);

    // render loop
    while (!glfwWindowShouldClose(Window1.window)) {
        // per-frame time logic
        Window1.timing();

        // input
        Window1.keyCallback(Window1.window);

        /* Render */
        Window1.renderBackgroundColor();

        glm::mat4 projection = glm::perspective(glm::radians(Window1.camera.worldScale * 45.0f), static_cast<float>(Window1.width) / static_cast<float>(Window1.height), 0.1f, 100.0f);
        glm::mat4 view       = glm::lookAt(Window1.camera.pos, Window1.camera.pos + Window1.camera.front, Window1.camera.up);

        pen.use();
        pen.setMat4("projection", projection);
        pen.setMat4("view", view);
        pen.setVec3("color", 0.6f, 0.6f, 0.6f);
        drawArray(grid);

        pen.setVec3("color", 0.0f, 0.0f, 0.0f);
        drawArray(axes);
        drawArray(arrowOx);
        drawArray(arrowOy);

        // polygon
        pen.setVec3("color", glm::vec3(0.4f));
        for (const auto& i : Window1.points) {
            Window1.render(sizeof(GLfloat) * Window1.pointArray.size(), Window1.pointArray.data(), GL_LINE_LOOP, Window1.points.size());
        }
        // points
        pen.setVec3("color", pointColor[0], pointColor[1], pointColor[2]);
        for (size_t i = 0; i < Window1.points.size(); i++) {
            Window1.render(sizeof(GLfloat) * Window1.points.at(i).size(), Window1.points.at(i).data(), GL_TRIANGLE_FAN, Window1.points.at(i).size() / 2);
        }

        // point P and line
        if (Window1.vectorVertex.size() > 0) {
            pen.setVec3("color", 1.0f, 0.0f, 0.77f);
            Window1.render(sizeof(GLfloat) * Window1.vectorVertex.size(), Window1.vectorVertex.data(), GL_TRIANGLE_FAN, Window1.vectorVertex.size() / 2);

            GLfloat line[4] = { 0.0f, 0.0f, Window1.vectorVertex.at(0), Window1.vectorVertex.at(1) };
            Window1.render(sizeof(line), line, GL_LINES, 2);
        }

        renderAllText(text, projection, view);
        /*        */

        /* Interface */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Menu");
        ImGui::ColorEdit3("Background Color", Window1.backgroundColor);
        ImGui::ColorEdit3("Point Color", pointColor);
        ImGui::SliderFloat("World Scale", &Window1.camera.worldScale, 0.001f, 1.4f);
        ImGui::Separator();
        ImGui::Checkbox("Point Mode", &Window1.pointMode);
        ImGui::Checkbox("Vector Mode", &Window1.vectorMode);
        ImGui::Separator();
        ss.str(std::string());
        if (Window1.vectorVertex.size() > 0) {
            ss << "Vector OP (" << Window1.vectorVertex.at(0) << " " << Window1.vectorVertex.at(1) << ")";
        }
        else {
            ss << "Vector OP (0 0)";
        }
        ImGui::Text(ss.str().c_str()); ImGui::SameLine();
        if (ImGui::Button("Translate")) {
            Window1.translate();
        }
        ImGui::Separator();
        ImGui::InputFloat("Scale Factor", &scaleFactor, 0.1f);
        if (ImGui::Button("Scale")) {
            Window1.scale(scaleFactor);
        }
        ImGui::Separator();
        ss.str(std::string());
        ss << "Selected point: " << Window1.selectedPoint[0] << " x " << Window1.selectedPoint[1];
        ImGui::Text(ss.str().c_str());
        if (ImGui::Button("Reflect")) {
            Window1.reflection(Window1.selectedPoint);
        }
        ImGui::Separator();
        ImGui::InputFloat("Angle", &angle, 0.1f);
        if (ImGui::Button("Rotate")) {
            Window1.rotation(Window1.selectedPoint, angle);
        }
        ImGui::Separator();
        for (size_t i = 0; i < Window1.pointArray.size(); i += 2) {
            float arr[] = { Window1.pointArray.at(i), Window1.pointArray.at(i + 1) };
            ImGui::InputFloat2("point", arr);
        }
        ImGui::Separator();
        ss.str(std::string());
        ss << "Resolution: " << Window1.width << "x" << Window1.height;
        ImGui::Text(ss.str().c_str());
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

        glfwSwapBuffers(Window1.window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}