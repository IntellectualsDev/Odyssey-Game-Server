//
// Created by Anshul Gowda on 3/16/24.
//

#ifndef ODYSSEY_GAME_SERVER_FPSRULES_H
#define ODYSSEY_GAME_SERVER_FPSRULES_H

#include <raylib.h>
#include <raymath.h>
#include <vector>

using namespace std;

//TODO: Instead of pulling out the parts of the function I want, pull the pull contents of Player.cpp and Bullet.cpp. And within their
// updateClient and updateEntity methods return filled FPSClientState structs and FPSEntityState structs

struct FPSEntityState {
    Model bulletModel;
    bool alive;
    BoundingBox bulletBox{};
    Vector3 position{};
    Vector3 velocity{};
    Vector3 hitbox{};
};

struct FPSClientState {
    Quaternion cameraRotation{};
    Vector3 separationVector{};
    bool topCollision = false;
//    bool colliding{};
    float Gravity = -0.05;
    float Jump = 0.8f;
    bool grounded = false;
    BoundingBox playerBox{};
    float coolDown = 0;
    vector<FPSEntityState> entities = {};
    Camera3D camera = {0};
    Vector3 position = (Vector3){0,2,1};
    Vector3 velocity = (Vector3){0,0,0};
    Vector3 hitbox = (Vector3){1.0f,2.0f,1.0f};
    bool alive = true;
    int cameraMode = CAMERA_FIRST_PERSON;
    float JumpTimer = 0.0f;
};


#endif //ODYSSEY_GAME_SERVER_FPSRULES_H
