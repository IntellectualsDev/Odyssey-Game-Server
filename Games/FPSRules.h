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

//TODO: repurpose this to generate an FPSClientState object

/*
void UpdateClient(bool w, bool a, bool s, bool d,Vector2 mouseDelta,bool shoot,bool space,float dt, Vector3 prevPosition, vector<BoundingBox> &terrainList,vector<BoundingBox> &topBoxVector,bool sprint,bool crouch, FPSClientState client, FPSEntityState entity) {


    if(CheckCollisionBoxes(client.playerBox,terrainList[3])){
        setGrounded(true);
    }
    else if(topCollision){
        grounded = true;
        topCollision = false;
    }else{
        grounded = false;
        space = false;
    }
    if(space && grounded){
        grounded = false;
        velocity = (Vector3){(w)*0.15f -(s)*0.15f,((space)*Jump),(d)*0.15f -(a)*0.15f};
    }else if (!grounded){
        velocity = (Vector3){(w)*0.15f -(s)*0.15f,velocity.y + Gravity,(d)*0.15f -(a)*0.15f};
    }else{
        velocity = (Vector3){(w)*0.15f -(s)*0.15f,0,(d)*0.15f -(a)*0.15f};
    }

    //TODO check for case to set grounded == true
    //TODO implement grounded/jumping movement
    UpdateCameraPro(&camera,
                    Vector3Multiply((Vector3){velocity.x,velocity.z,velocity.y},(Vector3){(sprint+1.0f), (sprint+1.0f), 1.0f}),
                    (Vector3){
                            mouseDelta.x*0.1f,                            // Rotation: yaw
                            mouseDelta.y*0.1f,                            // Rotation: pitch
                            0.0f                                                // Rotation: roll
                    },
                    GetMouseWheelMove()*2.0f);
    position = camera.position;




    if(shoot && coolDown <= 0.0f){
        coolDown = 0.3;
        cout << "SHOOT!" << endl;
        Bullet temp(Vector3Add(camera.position, Vector3Scale(camera_direction(&camera),0.7f)), Vector3Scale(camera_direction(&camera),5.0f),(Vector3){0.1f,0.1f,0.1f},
                    true);
        //TODO look into ray casting
//        temp.getBulletModel().transform =  MatrixRotateXYZ((Vector3){ DEG2RAD*temp.getVelocity().x, DEG2RAD*temp.getVelocity().y, DEG2RAD*temp.getVelocity().z});
        entities.push_back(temp);
        //TODO deque object instead of vector
    }

    for(int i = 0;i < terrainList.size();i++){
        if(i <3){
            //TODO fix this
            if(CheckCollisionBoxes(playerBox,topBoxVector[i])&&!space){
                position.y = 2+topBoxVector[i].max.y;//bad code
                camera.position.y = position.y;
                topCollision = true;
            }else if(CheckCollision(playerBox,terrainList[i],separationVector)){
                position = Vector3Add(position,separationVector);
                camera.position = position;
                camera.target = Vector3Add(camera.target,separationVector);
            }
        }


    }
    playerBox.min = (Vector3){position.x - hitbox.x/2,
                              position.y - hitbox.y/2-1.0f,
                              position.z - hitbox.z/2};
    playerBox.max = (Vector3){position.x + hitbox.x/2,
                              position.y + hitbox.y/2-0.5f,
                              position.z + hitbox.z/2};
}
 */

// TODO: Repurpose this to return a FPSEntityState struct

/*
void updateEntities(float dt) {
    for (int i = 0; i < entities.size(); i++) {
        //Vector3Subtract(entities[i].getPosition(),this->position)
        if(entities[i].getAlive()){
            Vector3 temp = Vector3Add(
                    entities[i].getPosition(),
                    Vector3Scale(entities[i].getVelocity(), dt*10));
            entities[i].UpdatePosition(temp.x,temp.y,temp.z) ;
        }else{
            entities.erase(entities.begin()+i);


        }
        BoundingBox tempBoundingBox = (BoundingBox){(Vector3){entities[i].getPosition().x - entities[i].getHitbox().x/2,
                                                              entities[i].getPosition().y - entities[i].getHitbox().y/2,
                                                              entities[i].getPosition().z - entities[i].getHitbox().z/2},
                                                    (Vector3){entities[i].getPosition().x + entities[i].getHitbox().x/2,
                                                              entities[i].getPosition().y+ entities[i].getHitbox().y/2,
                                                              entities[i].getPosition().z + entities[i].getHitbox().z/2}};
        entities[i].setBulletBox(tempBoundingBox);

    }
    coolDown -= dt*2;

}
*/

#endif //ODYSSEY_GAME_SERVER_FPSRULES_H
