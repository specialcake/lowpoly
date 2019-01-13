//
// Created by dydxh on 11/12/18.
//

#ifndef CGFRAME_TOOLS_H
#define CGFRAME_TOOLS_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Tools {
public:
    static GLfloat random01();
    static GLfloat random(GLfloat l, GLfloat r);
    static GLfloat random_11();
    static GLfloat random_angle();
    static glm::vec2 random_vec2();
    static glm::vec3 random_vec3();
    static GLfloat fmod(GLfloat a, GLfloat m);
    static GLfloat distance(glm::vec2 a, glm::vec2 b);
    static GLfloat distance(glm::vec3 a, glm::vec3 b);
    static GLfloat distance(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c);
    static GLfloat PointToSegment(glm::vec3 p, glm::vec3 a, glm::vec3 b);
    static GLint sign(GLfloat val);
    static bool inTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c);
    static void PrintVec2(glm::vec2 val);
    static void PrintVec3(glm::vec3 val);
    static void PrintVec4(glm::vec4 val);
    static void PrintMat3(glm::mat3 val);
    static void PrintMat4(glm::mat4 val);

    template <class T>
    static void swap(T &x,T &y) {
        T temp=x;
        x=y;
        y=temp;
    }
};


#endif //CGFRAME_TOOLS_H
