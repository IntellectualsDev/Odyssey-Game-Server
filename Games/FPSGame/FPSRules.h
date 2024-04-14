//
// Created by Anshul Gowda on 3/16/24.
//

#ifndef ODYSSEY_GAME_SERVER_FPSRULES_H
#define ODYSSEY_GAME_SERVER_FPSRULES_H

#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <iostream>
#include <set>

using namespace std;

//TODO: Instead of pulling out the parts of the function I want, pull the pull contents of Player.cpp and Bullet.cpp. And within their
// updateClient and updateEntity methods return filled FPSClientState structs and FPSEntityState structs

struct FPSEntityState {
    Model bulletModel;
    bool alive;
    bool claimed;
    BoundingBox bulletBox{};
    Vector3 position{};
    Vector3 velocity{};
    Vector3 hitbox{}; // TODO: This is statically defined. No need to send

    // Default constructor
    FPSEntityState() = default; // Explicitly defaulted

    FPSEntityState(FPSEntityState&& other) noexcept
    : bulletModel(std::move(other.bulletModel)),
      alive(other.alive),
      claimed(other.claimed),
      bulletBox(std::move(other.bulletBox)),
      position(std::move(other.position)),
      velocity(std::move(other.velocity)),
      hitbox(std::move(other.hitbox)){
        // Optionally clear or reset the state of `other` if needed
    }

    // Move assignment operator
    FPSEntityState& operator=(FPSEntityState&& other) noexcept {
        if (this != &other) {
            bulletModel = std::move(other.bulletModel); // Assuming Model supports move semantics
            alive = other.alive;
            claimed = other.claimed;
            bulletBox = std::move(other.bulletBox);
            position = std::move(other.position);
            velocity = std::move(other.velocity);
            hitbox = std::move(other.hitbox);
        }
        return *this;
    }

//    // Method to clone the object
//    FPSEntityState clone() const {
//        FPSEntityState cloned;
//        cloned.bulletModel = this->bulletModel; // Assuming Model supports move semantics
//        cloned.alive = this->alive;
//        cloned.claimed = this->claimed;
//        cloned.position = this->position;
//        cloned.velocity = this->velocity;
//        cloned.hitbox = this->hitbox;
//
//        return cloned;
//    }
};

struct FPSClientState {
    float dt{}; // TODO
    Vector3 separationVector{};
    bool topCollision;
//TODO: make these constants
//    float Gravity = -0.05;
//    float Jump = 0.8f;
    bool grounded = false;
    bool space;
    bool sprint = false;
    BoundingBox playerBox{}; //
//    Vector3 hitBox{}; // don't need to send this
    float coolDown;
    vector<FPSEntityState> entities = {};
    set<size_t> freeEntities = {};
    Camera3D camera;
    Vector3 position;
    Vector3 velocity;
    bool alive = true;
    int cameraMode = CAMERA_FIRST_PERSON;

    // Default constructor
    FPSClientState() = default; // Explicitly defaulted

    FPSClientState(FPSClientState&& other) noexcept
            : dt(other.dt),
              separationVector(std::move(other.separationVector)),
              topCollision(other.topCollision),
              grounded(other.grounded),
              space(other.space),
              sprint(other.sprint),
              playerBox(std::move(other.playerBox)),
              coolDown(other.coolDown),
              entities(std::move(other.entities)),
              freeEntities(std::move(other.freeEntities)),
              camera(std::move(other.camera)),
              position(std::move(other.position)),
              velocity(std::move(other.velocity)),
              alive(other.alive),
              cameraMode(other.cameraMode) {
        // Optionally clear or reset the state of `other` if needed
    }

    FPSClientState& operator=(FPSClientState&& other) noexcept {
        if (this != &other) {
            dt = other.dt;
            separationVector = std::move(other.separationVector);
            topCollision = other.topCollision;
            grounded = other.grounded;
            space = other.space;
            sprint = other.sprint;
            playerBox = std::move(other.playerBox);
            coolDown = other.coolDown;
            entities = std::move(other.entities);
            freeEntities = std::move(other.freeEntities);
            camera = std::move(other.camera);
            position = std::move(other.position);
            velocity = std::move(other.velocity);
            alive = other.alive;
            cameraMode = other.cameraMode;
            // Optionally clear or reset the state of `other` if needed
        }
        return *this;
    }

//    // Method to clone the object
//    std::unique_ptr<FPSClientState> clone() const {
//        auto cloned = std::make_unique<FPSClientState>();
//        cloned->dt = this->dt;
//        cloned->separationVector = this->separationVector;
//        cloned->topCollision = this->topCollision;
//        cloned->grounded = this->grounded;
//        cloned->space = this->space;
//        cloned->sprint = this->sprint;
//        cloned->playerBox = this->playerBox;  // Assuming this can be copied directly
//        cloned->coolDown = this->coolDown;
//        cloned->camera = this->camera;  // Assuming Camera3D can be copied directly
//        cloned->position = this->position;
//        cloned->velocity = this->velocity;
//        cloned->alive = this->alive;
//        cloned->cameraMode = this->cameraMode;
//
//        // Handle entities and other complex or non-copyable members manually
//        for (const auto& entity : this->entities) {
//            cloned->entities.push_back(std::move(entity.clone()));
////            cloned->entities.push_back(entity.clone());  // Assuming FPSEntityState has a clone method
//        }
//
//        return cloned;
//    };
};


class FPSRules {

};

#endif //ODYSSEY_GAME_SERVER_FPSRULES_H


