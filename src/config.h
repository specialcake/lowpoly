//
// Created by dydxh on 11/12/18.
//

#ifndef CGFRAME_CONFIG_H
#define CGFRAME_CONFIG_H

//Screen
#define WINDOW_NAME "CGFrame"
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900

//Scene
#define SCENE_LENGTH 27.0f
#define CHUNK_SIZE 5
#define CHUNK_RADIUS 2
#define CHUNK_LENGTH (SCENE_LENGTH / CHUNK_SIZE)
#define MESH_SIZE 5
#define MESH_RADIUS 2
#define MESH_LENGTH (CHUNK_LENGTH / MESH_SIZE)
#define OBJECT_TYPE_NUMBER 2
#define LAND_COLOR glm::vec3(0.543f, 0.836f, 0.25f)
enum Meshtype{
    LAND,
    WATER
};
enum Direction{
    ORIGIN_POS,
    MOVE_XPOS,
    MOVE_XNEG,
    MOVE_ZPOS,
    MOVE_ZNEG
};

//Terrian
#define NOISE_SIZE 29
#define NOISE_WIDTH 8
#define NOISE_HEIGHT 8
#define TERRIAN_WIDTH 32
#define TERRIAN_HEIGHT 32
#define TERRIAN_SPRITE_WIDTH 0.25
#define TERRIAN_SPRITE_HEIGHT 0.25
#define MAX_HEIGHT 2.0f

//Value
#define PI 3.1415926535f

//Camera
#define YAW -90.0f
#define PITCH 0.0f
#define MIN_PITCH -89.5f
#define MAX_PITCH 89.5f
#define SPEED 5.0f
#define YAW_SPEED 2.0f
#define SENSITIVITY 0.1f
#define ZOOM 45.0f
#define MIN_ZOOM 1.0f
#define MAX_ZOOM 45.0f

#define default_position glm::vec3(0.0f, 3.0f, 0.0f)
#define default_up glm::vec3(0.0f, 1.0f, 0.0f)
#define default_front glm::vec3(0.0f, 0.0f, -1.0f)
//#define default_position glm::vec3(0.0f, -3.0f, 1.0f)
//#define default_up glm::vec3(0.0f, 0.0f, 1.0f)
//#define default_front glm::vec3(0.0f, 1.0f, 0.0f)

//Material
#define MODEL_DIFFUSE_NAME "texture_diffuse"
#define MODEL_SPECULAR_NAME "texture_specular"

//Particle
#define MINSPEED 0.15f
#define MAXSPEED 0.35f;
#define MINLIFENUMBER 0.7f;
#define MAXLIFENUMBER 1.3f;
#define DEFAULT_ACCELERATION glm::vec3(0.0f, 1.0f, 0.0f)

#endif //CGFRAME_CONFIG_H
