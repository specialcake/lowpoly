//
// Created by dydxh on 10/23/18.
//

#include "texture.h"
#include <iostream>

Texture2D::Texture2D()
        : Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_LINEAR), Wrap_T(GL_LINEAR), Filter_Min(GL_NEAREST), Filter_Max(GL_NEAREST) {
    glGenTextures(1, &this->ID);
}

void Texture2D::Generate(GLuint width, GLuint height, unsigned char* data) {
    this->Width = width;
    this->Height = height;

    glBindTexture(GL_TEXTURE_2D, this->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Generatefloat(GLuint width, GLuint height, GLfloat *data) {
    this->Width = width;
    this->Height = height;

    glBindTexture(GL_TEXTURE_2D, this->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_FLOAT, data);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_FLOAT, width, height, 0, GL_FLOAT, GL_FLOAT, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);

    glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture2D::Generatevec3(GLuint width, GLuint height, glm::vec3* data) {
    this->Width = width;
    this->Height = height;

    glBindTexture(GL_TEXTURE_2D, this->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_FLOAT, data);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_FLOAT, width, height, 0, GL_FLOAT, GL_FLOAT, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);

    glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture2D::GenerateShadowMap(GLuint width, GLuint height) {
    this->Width = width, this->Height = height;
    this->Internal_Format = this->Image_Format = GL_DEPTH_COMPONENT;

    glBindTexture(GL_TEXTURE_2D, this->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const {
    glBindTexture(GL_TEXTURE_2D, this->ID);
}