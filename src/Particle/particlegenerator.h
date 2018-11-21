//
// Created by dydxh on 10/24/18.
//

#ifndef BREAKOUT_PARTICLEGENERATOR_H
#define BREAKOUT_PARTICLEGENERATOR_H

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../Resource/shader.h"
#include "../Resource/texture.h"

// Represents a single particle and its state
struct Particle {
    glm::vec3 Position, Velocity;
    GLfloat Life, alpha, size, MaxLife;

    Particle() : Position(glm::vec3(0.0f)), Velocity(glm::vec3(0.0f)), Life(0.0f) { }
};
struct Generator {
    GLfloat MinSpeed, MaxSpeed, r;
    GLfloat MinLifeNumber, MaxLifeNumber;
    glm::vec3 Position;
    glm::vec3 Acceleration;
};

class ParticleGenerator {
public:
    Shader shader;
    Generator origin;
    ParticleGenerator(Shader shader, Texture2D texture, GLuint amount);
    void Update(GLfloat dt, GLuint newParticles);
    void Draw();
private:
    std::vector<Particle> particles;
    GLuint amount;
    Texture2D texture;
    GLuint VAO;
    void init();
    Particle newParticle();
    GLuint firstUnusedParticle();
    void respawnParticle(Particle &particle);
};


#endif //BREAKOUT_PARTICLEGENERATOR_H
