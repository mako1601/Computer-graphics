#ifndef _DrawFunctions_h_
#define _DrawFunctions_h_

#include <iostream>
#include <vector>
#include <map>
#include <functional>

#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#define M_PI 3.14159265f

// text
GLuint textVAO, textVBO;
struct Character {
    unsigned int textureID;
    glm::ivec2   size;
    glm::ivec2   bearing;
    unsigned int advance;
};
std::map<GLchar, Character> characters;

float f(float x) {
    return cos(x);
}

void bind(GLuint& VBO, GLuint& VAO, GLsizeiptr size, const void* data) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
}

std::vector<GLfloat> graphVertex(std::function<float(float)> f) {
    std::vector<GLfloat> vertex;
    for (float x = -10.0f; x < 10.01f; x += 0.01f) {
        vertex.push_back(x);
        vertex.push_back(f(x));
    }
    return vertex;
}

std::vector<GLfloat> gridVertex() {
    std::vector<GLfloat> vertex;
    for (float x = -10.0f; x < 11.0f; x++) {
        vertex.push_back(x);
        vertex.push_back(-10.0f);
        vertex.push_back(x);
        vertex.push_back(10.0f);
    }
    for (float y = -10.0f; y < 11.0f; y++) {
        vertex.push_back(-10.0f);
        vertex.push_back(y);
        vertex.push_back(10.0f);
        vertex.push_back(y);
    }
    return vertex;
}

std::vector<GLfloat> axesVertex() {
    return {
        -10.0f,   0.0f,
         10.0f,   0.0f,
          0.0f, -10.0f,
          0.0f,  10.0f
    };
}

std::vector<GLfloat> arrowOxVertex() {
    return {
         9.8f, -0.05f,
        10.0f,  0.0f,
         9.8f,  0.05f
    };
}

std::vector<GLfloat> arrowOyVertex() {
    return {
        -0.05f,  9.8f,
         0.05f,  9.8f,
         0.0f,  10.0f
    };
}

struct OpenGLObject {
    GLuint VBO{};
    GLuint VAO{};
    std::function<std::vector<GLfloat>()> array;
    GLenum mode;
    GLsizei count;

    OpenGLObject(std::function<std::vector<GLfloat>()> _array, GLenum _mode, GLsizei _count) : array(_array), mode(_mode), count(_count) {}
};

OpenGLObject graph([&]() { return graphVertex(f); }, GL_LINE_STRIP, 2001);

OpenGLObject grid(gridVertex, GL_LINES, 84);
OpenGLObject axes(axesVertex, GL_LINES, 4);
OpenGLObject arrowOx(arrowOxVertex, GL_TRIANGLES, 3);
OpenGLObject arrowOy(arrowOyVertex, GL_TRIANGLES, 3);

void drawVertex(OpenGLObject& object) {
    std::vector<GLfloat> vertex = object.array();
    bind(object.VBO, object.VAO, sizeof(GLfloat) * vertex.size(), vertex.data());
}

void updateVertex(OpenGLObject& object) {
    glBindVertexArray(object.VAO);

    std::vector<GLfloat> vertex = object.array();

    glBindBuffer(GL_ARRAY_BUFFER, object.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * vertex.size(), vertex.data());
    glBindVertexArray(0);
}

void drawArray(OpenGLObject& object) {
    glBindVertexArray(object.VAO);
    glDrawArrays(object.mode, 0, object.count);
    glBindVertexArray(0);
}

void drawCartesian() {
    drawVertex(grid);
    drawVertex(axes);
    drawVertex(arrowOx);
    drawVertex(arrowOy);
}

void updateCartesian() {
    updateVertex(grid);
    updateVertex(axes);
}

int initFreeType() {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -3;
    }
    FT_Face face;
    if (FT_New_Face(ft, "..\\resources\\arial.ttf", 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -4;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        characters.insert(std::pair<char, Character>(c, character));
    }

    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void renderText(ShaderProgram& shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::mat4 projection, glm::mat4 view) {
    shader.use();
    shader.setVec3("textColor", 0.0f, 0.0f, 0.0f);
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(textVAO);

    for (const char& c : text) {
        Character ch = characters[c];

        glm::vec4 pos = glm::vec4(x + ch.bearing.x * scale, y - (ch.size.y - ch.bearing.y) * scale, 0.0, 1.0);

        GLfloat xpos = pos.x;
        GLfloat ypos = pos.y;
        GLfloat w = ch.size.x * scale;
        GLfloat h = ch.size.y * scale;

        float vertices[6][4] = {
            { xpos,     ypos + h, 0.0f, 0.0f },
            { xpos,     ypos,     0.0f, 1.0f },
            { xpos + w, ypos,     1.0f, 1.0f },

            { xpos,     ypos + h, 0.0f, 0.0f },
            { xpos + w, ypos,     1.0f, 1.0f },
            { xpos + w, ypos + h, 1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void renderAllText(ShaderProgram& shader, glm::mat4 projection, glm::mat4 view) {
    // Ox
    renderText(shader, "-10", -9.95f, -0.2f,  0.005f, projection, view);
    renderText(shader,  "-9", -9.25f, -0.2f,  0.005f, projection, view);
    renderText(shader,  "-8", -8.25f, -0.2f,  0.005f, projection, view);
    renderText(shader,  "-7", -7.25f, -0.2f,  0.005f, projection, view);
    renderText(shader,  "-6", -6.25f, -0.2f,  0.005f, projection, view);
    renderText(shader,  "-5", -5.25f, -0.2f,  0.005f, projection, view);
    renderText(shader,  "-4", -4.25f, -0.2f,  0.005f, projection, view);
    renderText(shader,  "-3", -3.25f, -0.2f,  0.005f, projection, view);
    renderText(shader,  "-2", -2.25f, -0.2f,  0.005f, projection, view);
    renderText(shader,  "-1", -1.25f, -0.2f,  0.005f, projection, view);
    renderText(shader,   "0", -0.15f, -0.2f,  0.005f, projection, view);
    renderText(shader,   "1",  0.85f, -0.2f,  0.005f, projection, view);
    renderText(shader,   "2",  1.85f, -0.2f,  0.005f, projection, view);
    renderText(shader,   "3",  2.85f, -0.2f,  0.005f, projection, view);
    renderText(shader,   "4",  3.85f, -0.2f,  0.005f, projection, view);
    renderText(shader,   "5",  4.85f, -0.2f,  0.005f, projection, view);
    renderText(shader,   "6",  5.85f, -0.2f,  0.005f, projection, view);
    renderText(shader,   "7",  6.85f, -0.2f,  0.005f, projection, view);
    renderText(shader,   "8",  7.85f, -0.2f,  0.005f, projection, view);
    renderText(shader,   "9",  8.85f, -0.2f,  0.005f, projection, view);
    renderText(shader,  "10",  9.7f,  -0.25f, 0.005f, projection, view);

    // Oy
    renderText(shader, "-10", -0.4f,  -9.95f, 0.005f, projection, view);
    renderText(shader,  "-9", -0.25f, -9.2f,  0.005f, projection, view);
    renderText(shader,  "-8", -0.25f, -8.2f,  0.005f, projection, view);
    renderText(shader,  "-7", -0.25f, -7.2f,  0.005f, projection, view);
    renderText(shader,  "-6", -0.25f, -6.2f,  0.005f, projection, view);
    renderText(shader,  "-5", -0.25f, -5.2f,  0.005f, projection, view);
    renderText(shader,  "-4", -0.25f, -4.2f,  0.005f, projection, view);
    renderText(shader,  "-3", -0.25f, -3.2f,  0.005f, projection, view);
    renderText(shader,  "-2", -0.25f, -2.2f,  0.005f, projection, view);
    renderText(shader,  "-1", -0.25f, -1.2f,  0.005f, projection, view);
    renderText(shader,   "1", -0.15f,  0.8f,  0.005f, projection, view);
    renderText(shader,   "2", -0.15f,  1.8f,  0.005f, projection, view);
    renderText(shader,   "3", -0.15f,  2.8f,  0.005f, projection, view);
    renderText(shader,   "4", -0.15f,  3.8f,  0.005f, projection, view);
    renderText(shader,   "5", -0.15f,  4.8f,  0.005f, projection, view);
    renderText(shader,   "6", -0.15f,  5.8f,  0.005f, projection, view);
    renderText(shader,   "7", -0.15f,  6.8f,  0.005f, projection, view);
    renderText(shader,   "8", -0.15f,  7.8f,  0.005f, projection, view);
    renderText(shader,   "9", -0.15f,  8.8f,  0.005f, projection, view);
    renderText(shader,  "10", -0.35f,  9.8f,  0.005f, projection, view);

    renderText(shader, "x", 9.8f, 0.1f, 0.006f, projection, view);
    renderText(shader, "y", 0.07f, 9.82f, 0.006f, projection, view);
}

#endif