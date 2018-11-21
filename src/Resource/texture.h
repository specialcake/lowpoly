//
// Created by dydxh on 10/23/18.
//

#ifndef BREAKOUT_TEXTURE_H
#define BREAKOUT_TEXTURE_H

#include <glad/glad.h>

class Texture2D {
public:
    GLuint ID;
    GLuint Width, Height;

    GLuint Internal_Format;
    GLuint Image_Format;

    GLuint Wrap_S;
    GLuint Wrap_T;
    GLuint Filter_Min;
    GLuint Filter_Max;

    Texture2D();
//    Texture2D(const GLchar *filepath);
    void Generate(GLuint width, GLuint height, unsigned char* data);
    void Bind() const;
};


#endif //BREAKOUT_TEXTURE_H
