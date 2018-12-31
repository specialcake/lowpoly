//
// Created by dydxh on 10/24/18.
//

#include "particlegenerator.h"
#include "../config.h"
#include "../utils/tools.h"

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, GLuint amount)
        : shader(shader), texture(texture), amount(amount) {
    this->init();
}

void ParticleGenerator::Update(GLfloat dt, GLuint newParticles) {
    for (GLuint i = 0; i < newParticles; ++i) {
        int unusedParticle = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unusedParticle]);
    }

    for (GLuint i = 0; i < this->amount; ++i) {
        Particle &p = this->particles[i];
        p.Life -= dt;
        if (p.Life > 0.0f) {
            p.Position += p.Velocity * dt;
//            p.Color.a -= dt * 2.5;
            p.Velocity += origin.Acceleration * dt * 0.1f;
            float factor = 1.0f / ((p.Life / 1000.0f - p.MaxLife / 2000.0f) * (p.Life / 1000.0f - p.MaxLife / 2000.0f) + 1);
            p.alpha = factor;
            p.size = 0.17f * factor;
        }
    }
}

void ParticleGenerator::Draw() {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->shader.use();
    for (Particle particle : this->particles) {
        if (particle.Life > 0.0f) {
            this->shader.setVec3("offset", particle.Position);
            this->shader.setFloat("alpha", particle.alpha);
            this->shader.setFloat("scale", particle.size);
            this->texture.Bind();
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle() {
    for (GLuint i = lastUsedParticle; i < this->amount; ++i){
        if (this->particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    for (GLuint i = 0; i < lastUsedParticle; ++i){
        if (this->particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::respawnParticle(Particle &particle) {
    particle = newParticle();
}

Particle ParticleGenerator::newParticle() {
    Particle ret;
    GLfloat randomx = Tools::random(-0.1f, 0.1f);
    GLfloat randomz = Tools::random(-0.1f, 0.1f);

    ret.Position = origin.Position + glm::vec3(randomx, 0.0, randomz);
    ret.MaxLife = ret.Life = Tools::random(origin.MinLifeNumber, origin.MaxLifeNumber);
    ret.Velocity = glm::vec3(0.0f, Tools::random(origin.MinSpeed, origin.MaxSpeed), 0.0f);
    if(Tools::distance(ret.Position, glm::vec3(0.0f, 0.0f, 0.0f)) < origin.r)
        ret.MaxLife = ret.Life = ret.Life * 1.3f;
    return ret;
}

void ParticleGenerator::init() {
    // Set up mesh and attribute properties
    GLuint VBO;
    GLfloat particle_quad[] = {
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);


    GLfloat randomx = Tools::random(-0.1f, 0.1f);
    GLfloat randomz = Tools::random(-0.1f, 0.1f);
    origin.r = 1.0f;
    origin.MinSpeed = MINSPEED;
    origin.MaxSpeed = MAXSPEED;
    origin.MinLifeNumber = MINLIFENUMBER;
    origin.MaxLifeNumber = MAXLIFENUMBER;
    origin.Position = glm::vec3(randomx, 0.01f, randomz);
    origin.Acceleration = DEFAULT_ACCELERATION;
    // Create this->amount default particle instances
    for (GLuint i = 0; i < this->amount; ++i)
        this->particles.push_back(newParticle());
}