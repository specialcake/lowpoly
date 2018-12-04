//
// Created by dydxh on 12/4/18.
//

#ifndef TMPER_FONTDISPLAY_H
#define TMPER_FONTDISPLAY_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "shader.h"

#include <string>
#include <map>

struct Character {
    GLuint TextureID;
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph
};

class Fontdisplay {
public:
    GLuint VAO, VBO;
    Shader shader;
    FT_Library ft;
    FT_Face face;
    std::map<GLchar, Character> Characters;
    void Initialize(Shader s);
    void RenderText(std::string text, glm::vec3 pos, glm::vec3 scale, glm::vec3 color);
};


#endif //TMPER_FONTDISPLAY_H
