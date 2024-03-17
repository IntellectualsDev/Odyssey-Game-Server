//
// Created by josep on 2/8/2024.
//

#include "Player.h"
#include <raymath.h>
#include <algorithm>

void Player::UpdatePlayer(FPSClientState& previousState, FPSClientState& currentState, bool w, bool a, bool s, bool d,Vector2 mouseDelta,bool shoot,bool space,float dt, vector<BoundingBox> &terrainList,vector<BoundingBox> &topBoxVector,bool sprint,bool crouch) {
//Continious collision detection.

    if(CheckCollisionBoxes(playerBox,terrainList[3])){
//        setGrounded(true);
        currentState.grounded = true;
    }
    else if(previousState.topCollision){
//        grounded = true;
//        topCollision = false;
        currentState.grounded = true;
        currentState.topCollision = false;
    }else{
//        grounded = false;
//        space = false;
        currentState.grounded = false;
        currentState.space = false;
    }
    if(space && grounded){
//        grounded = false;
//        velocity = (Vector3){(w)* dt*4  -(s)*dt*4 ,((space)*Jump*10*dt),(d)*dt*4 -(a)*dt*4 };
        currentState.grounded = false;
        currentState.velocity = (Vector3){(w)* dt*4 -(s)*dt*4 ,((space)*Jump*10*dt),(d)*dt*4 -(a)*dt*4 };
    }else if (!grounded){
//        velocity = (Vector3){(w)*dt*4 -(s)*dt*4 ,velocity.y + Gravity*dt*10,(d)*dt*4  -(a)*dt*4 };
        currentState.velocity = (Vector3){(w)*dt*4 -(s)*dt*4 ,previousState.velocity.y + Gravity*dt*10,(d)*dt*4  -(a)*dt*4 };
    }else{
//        velocity = (Vector3){(w)*dt*4  -(s)*dt*4 ,0,(d)*dt*4  -(a)*dt*4 };
        currentState.velocity = (Vector3){(w)*dt*4  -(s)*dt*4 ,0,(d)*dt*4  -(a)*dt*4 };
    }

    //TODO check for case to set grounded == true
    //TODO implement grounded/jumping movement
    UpdateCameraPro(&previousState.camera,
    Vector3Multiply((Vector3){velocity.x,velocity.z,velocity.y},(Vector3){(sprint+1.0f), (sprint+1.0f), 1.0f}),//SHIT
    (Vector3){
        mouseDelta.x*dt*2.0f ,                            // Rotation: yaw
        mouseDelta.y*dt*2.0f,                            // Rotation: pitch
        0.0f                                                // Rotation: roll
        },
        GetMouseWheelMove()*dt *2.0f);
    currentState.position = previousState.camera.position;
    currentState.camera = previousState.camera;
//    position = camera.position;

    if(shoot && previousState.coolDown <= 0.0f){
//        coolDown = 0.3;
        currentState.coolDown = 0.3;
//        cout << "SHOOT!" << endl;
//        Bullet temp(Vector3Add(currentState.camera.position, Vector3Scale(camera_direction(&currentState.camera),0.7f)), Vector3Scale(camera_direction(&currentState.camera),5.0f),(Vector3){0.1f,0.1f,0.1f},
//                    true);
        struct FPSEntityState temp;
        temp.position = Vector3Add(currentState.camera.position, Vector3Scale(camera_direction(&currentState.camera),0.7f));
        temp.velocity = Vector3Scale(camera_direction(&currentState.camera),5.0f);
        temp.hitbox = (Vector3){0.1f,0.1f,0.1f};
        temp.alive = true;
        //TODO look into ray casting
        currentState.entities.push_back(temp);
        //TODO deque object instead of vector
    }
    //TODO
    //store vector terrainList on init sequence and share with clients
    //create a camera object for each joining client
    for(int i = 0;i < terrainList.size();i++){
        if(i <3){
            //TODO fix this
            if(CheckCollisionBoxes(previousState.playerBox,topBoxVector[i])&&!space){
                currentState.position.y = 2+topBoxVector[i].max.y;//bad code
                currentState.camera.position.y = currentState.position.y;
                currentState.topCollision = true;
            }else if(CheckCollision(previousState.playerBox,terrainList[i],currentState.separationVector)){ // check coliision modies passed seperationVector
                currentState.position = Vector3Add(currentState.position,currentState.separationVector);
                currentState.camera.position = currentState.position;
                currentState.camera.target = Vector3Add(currentState.camera.target,currentState.separationVector);
            }
        }


    }
    //TODO: statically define hitbox (size of player) on init
    currentState.playerBox.min = (Vector3){currentState.position.x - hitbox.x/2,
                              currentState.position.y - hitbox.y/2-1.0f,
                              currentState.position.z - hitbox.z/2};
    currentState.playerBox.max = (Vector3){currentState.position.x + hitbox.x/2,
                              currentState.position.y + hitbox.y/2-0.5f,
                              currentState.position.z + hitbox.z/2};
}

void Player::setCameraMode(int temp) {

}


Vector3 Player::camera_direction(Camera *tcamera) {
    return Vector3Normalize(Vector3Subtract(tcamera->target, tcamera->position));
}

void Player::updateEntities(FPSClientState& currentState, float dt) {
    for (int i = 0; i < currentState.entities.size(); i++) {
        //Vector3Subtract(entities[i].getPosition(),this->position)
        if(currentState.entities[i].alive){
            Vector3 temp = Vector3Add(
                    currentState.entities[i].position,
                    Vector3Scale(currentState.entities[i].velocity, dt*10));
//            entities[i].UpdatePosition(temp.x,temp.y,temp.z) ;
            currentState.entities[i].position = (Vector3){temp.x,temp.y,temp.z};
        }else{
//            entities.erase(entities.begin()+i);
            currentState.entities.erase(currentState.entities.begin()+i);


        }
        BoundingBox tempBoundingBox = (BoundingBox){(Vector3){currentState.entities[i].position.x - currentState.entities[i].hitbox.x/2,
                                                              currentState.entities[i].position.y - currentState.entities[i].hitbox.y/2,
                                                              currentState.entities[i].position.z - currentState.entities[i].hitbox.z/2},
                                  (Vector3){currentState.entities[i].position.x + currentState.entities[i].hitbox.x/2,
                                            currentState.entities[i].position.y+ currentState.entities[i].hitbox.y/2,
                                            currentState.entities[i].position.z + currentState.entities[i].hitbox.z/2}};
//        entities[i].setBulletBox(tempBoundingBox);
        currentState.entities[i].bulletBox = tempBoundingBox;

    }
//    coolDown -= dt*2;
    currentState.coolDown -= dt*2;
}



bool Player::CheckCollision(BoundingBox playerBB, BoundingBox wallBB, Vector3& separationVector) {
    // Check if the player's bounding box is not entirely to the left of the wall
    bool notLeftOfWall = playerBB.max.x >= wallBB.min.x;
    float leftSeparation = wallBB.min.x - playerBB.max.x;

    // Check if the player's bounding box is not entirely to the right of the wall
    bool notRightOfWall = playerBB.min.x <= wallBB.max.x;
    float rightSeparation =  - wallBB.max.x + playerBB.min.x;

    // Check if the player's bounding box is not entirely above the wall
    bool notAboveWall = playerBB.max.y >= wallBB.min.y;
    float aboveSeparation = wallBB.min.y - playerBB.max.y;

    // Check if the player's bounding box is not entirely below the wall
    bool notBelowWall = playerBB.min.y <= wallBB.max.y;
    float belowSeparation = wallBB.max.y - playerBB.min.y;

    // Check if the player's bounding box is not entirely in front of the wall
    bool notInFrontOfWall = playerBB.max.z >= wallBB.min.z;
    float frontSeparation = wallBB.min.z - playerBB.max.z;

    // Check if the player's bounding box is not entirely behind the wall
    bool notBehindWall = playerBB.min.z <= wallBB.max.z;
    float behindSeparation =  - wallBB.max.z + playerBB.min.z;

    // If all conditions are true, then there is a collision
    if (notLeftOfWall && notRightOfWall && notAboveWall && notBelowWall && notInFrontOfWall && notBehindWall) {
        // Calculate the minimum separation vector
        float minSeparation = std::min({ abs(leftSeparation), abs(rightSeparation), abs(aboveSeparation), abs(belowSeparation), abs(frontSeparation), abs(behindSeparation)});

        if (minSeparation == abs(leftSeparation)) {
            separationVector = { leftSeparation, 0.0f, 0.0f };
        } else if (minSeparation == abs(rightSeparation)) {
            separationVector = { -rightSeparation, 0.0f, 0.0f };
        } else if (minSeparation == abs(aboveSeparation)) {
            separationVector = { 0.0f, aboveSeparation, 0.0f };
        } else if (minSeparation == abs(belowSeparation)) {
            separationVector = { 0.0f, -belowSeparation, 0.0f };
        } else if (minSeparation == abs(frontSeparation)) {
            separationVector = { 0.0f, 0.0f, frontSeparation };
        } else {
            separationVector = { 0.0f, 0.0f, -behindSeparation };
        }

        return true;
    }

    return false;
}


/*
 *
//TODO IMPLEMENT CIRCULAR BUFFER OF HISTORY OF PREDICTIONS
//make a class that implements this so its easier to genralize
//one buffer includes history of predictions from the last ack'd packet(updated by server from input buffer)
//one buffer is for the out buffer handled by the output thread and size is detrmined based on tick rate
//TODO IMPLEMENT OUTPUT BUFFER FOR PACKETS
//render player locally same as before
//send a packet every (MATH) or if a collision/shoot has been encountered
//put the packet into the output buffer (queue)
//output thread handles sending
//packet format includes previous inputs inorder to mitigate packet loss
//send tick number and dt and tick (on join most likely)
//TODO IMPLEMENT INPUT BUFFER FOR PACKETS
//on reception of a packet read the header (idk)
//on SNAPSHOT:
    //parse the packet and place the other player packet into its input buffer
    //parse the packet and find the part relating to the local player
    //check the tick number of the packet and (MATH) determine which local tick that is with the dt parameter
    //check the packet you locally predicted against the received packet and if they are the same then set the tail of the buffer to that packet
    //if the states are different then you must revert to that state
//TODO INTERPOLATE OTHER PLAYER
//make a class inheriting entity and define it to only be updated according to packest recieved from the server
//it should have a buffer (queue) of snapshot packets from the server and render a (MATH) amount of time behind the most recently recieved packet
//it should have x y z positions and direction x y z
//it also has a vector of bullet entities that will be rendered in the main loop
//once you pull out a packet look into the queue and see where the next position should be and interpolate to that position
    //this part requires the packet to contain the other clients dt in order to ensure you interpolate between snapshots correctly

 */