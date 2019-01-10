//
// Created by dydxh on 11/12/18.
//

#ifndef CGFRAME_CONFIG_H
#define CGFRAME_CONFIG_H

//#define lightview
//#define ANTIALIASING

//Screen
#define WINDOW_NAME "CGFrame"
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900

#define NEAR_PLANE 0.1f
#define FAR_PLANE 200.0f

#define SHADOW_WIDTH 1200
#define SHADOW_HEIGHT 900

//Scene
#define SCENE_LENGTH 53.0f
#define CHUNK_SIZE 5
#define CHUNK_RADIUS 2
#define CHUNK_LENGTH (SCENE_LENGTH / CHUNK_SIZE)
#define MESH_SIZE 21
#define MESH_RADIUS 10
#define MESH_LENGTH (CHUNK_LENGTH / MESH_SIZE)
#define OBJECT_TYPE_NUMBER 2
#define SEA_COLOR glm::vec3(0.106f, 0.863f, 1.0f)
#define LOWER_COLOR glm::vec3(0.75686f, 1.0f, 0.91373f)
#define LAND_COLOR glm::vec3(0.48627f, 0.97647f, 0.65882f)
#define ROCK_COLOR glm::vec3(0.522f, 0.522f, 0.522f)

#define MAX_HEIGHT 4.0f
#define NOISE_SIZE 29

#define TREEPLACER 4
#define TREENUMBER 3

#define BLOOMTHRESHOLD glm::vec3(0.2126, 0.7152, 0.0722)

#define CLOUDDIR glm::vec3(0.70711f, 0.0f, 0.70711f)

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

//Water
#define NUMOFWATER 4
#define SEA_LEVEL 0.10f
#define CONSTQ 0.5f

//GaussBlur
#define GAUSSBLUR_ROUND 10

//Light
//#define PARLIGHT_DIR glm::vec3(27.0f, -13.0f, -27.0f)
#define PARLIGHT_DIR glm::vec3(0.0f, -13.0f, 27.0f)
#define PARLIGHT_POSITION glm::vec3(0.0f, 13.0f, -27.0f)
//#define PARLIGHT_POSITION glm::vec3(-27.0f, 13.0f, 27.0f)
#define PARLIGHT_AMBIENT glm::vec3(0.3f)
#define PARLIGHT_DIFFUSE glm::vec3(0.9f)
#define PARLIGHT_SPECULAR glm::vec3(0.5f)

#define POINTLIGHT0_POS glm::vec3(0.0f, 1.0f, -14.0f)
#define POINTLIGHT0_AMBIENT glm::vec3(0.4f)
#define POINTLIGHT0_DIFFUSE glm::vec3(0.5f)
#define POINTLIGHT0_SPECULAR glm::vec3(0.0f)
#define POINTLIGHT0_CONSTANT 1.0f
#define POINTLIGHT0_LINEAR 0.045f
#define POINTLIGHT0_QUADRATIC 0.0075f

//Value
#define PI 3.1415926535f
#define eps 0.0000001

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

#ifndef lightview
#define default_position glm::vec3(0.0f, 3.0f, 3.0f)
#else //lightview
#define default_position glm::vec3(0.0f, 13.0f, -27.0f)
#endif //lightview
#define default_up glm::vec3(0.0f, 1.0f, 0.0f)
#define default_front glm::vec3(0.0f, -13.0f, 27.0f)
//#define default_front glm::vec3(0.0f, 0.0f, -1.0f)
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
