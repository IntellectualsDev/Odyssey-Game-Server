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
    float dt{}; // TODO
    Vector3 separationVector{}; //TODO
    bool topCollision;
//TODO: make these constants
//    float Gravity = -0.05;
//    float Jump = 0.8f;
    bool grounded = false;
    bool space; // TODO: determine if needed
    BoundingBox playerBox{}; //
    Vector3 hitBox{};
    float coolDown;
    vector<FPSEntityState> entities = {}; // TODO
    Camera3D camera;
    Vector3 position;
    Vector3 velocity;
    bool alive = true;
    int cameraMode = CAMERA_FIRST_PERSON;
};


class FPSRules {

};

#endif //ODYSSEY_GAME_SERVER_FPSRULES_H


