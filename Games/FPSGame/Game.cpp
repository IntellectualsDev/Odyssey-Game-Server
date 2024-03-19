
#include "Game.h"

std::vector<BoundingBox> Game::terrainVector = {
        {(Vector3){-16.0f - Game::wallWidth/2, 2.5f - Game::wallHeight/2, 0.0f - Game::wallLength/2},
                (Vector3){-16.0f + Game::wallWidth/2, 2.5f + Game::wallHeight/2, 0.0f + Game::wallLength/2}},

        {(Vector3){16.0f - Game::wallWidth/2, 2.5f - Game::wallHeight/2, 0.0f - Game::wallLength/2},
                (Vector3){16.0f + Game::wallWidth/2, 2.5f + Game::wallHeight/2, 0.0f + Game::wallLength/2}},

        {(Vector3){0.0f - Game::wallLength/2, 2.5f - Game::wallHeight/2, 16.0f - Game::wallWidth/2},
                (Vector3){0.0f + Game::wallLength/2, 2.5f + Game::wallHeight/2, 16.0f + Game::wallWidth/2}},

        {(Vector3){-Game::floorLength/2, -0.5, -Game::floorLength/2},
                (Vector3){Game::floorLength/2, 0.5, Game::floorLength/2}}
};

std::vector<BoundingBox> Game::topBoxVector = {
        {(Vector3){-16.0f - 0.5, 4.5f, 0.0f -Game::wallLength/2},(Vector3){-16.0f + 0.5, 2.5f + Game::wallHeight/2, 0.0f +Game::wallLength/2}},
        {(Vector3){16.0f - 0.5, 4.5f, 0.0f -Game::wallLength/2},(Vector3){16.0f + 0.5, 2.5f + Game::wallHeight/2, 0.0f +Game::wallLength/2}},
        {(Vector3){0.0f - Game::wallLength/2, 4.5f, 16.0f -0.5},(Vector3){0.0f + Game::wallLength/2, 2.5f + Game::wallHeight/2, 16.0f + 0.5}},
        {(Vector3){-Game::floorLength/2,-0.5,-Game::floorLength/2},(Vector3){Game::floorLength/2,0.5,Game::floorLength/2}}
};


Game::Game(): previousStatesOfPlayers(), currentStatesOfPlayers() {

}

size_t Game::createNewPlayer(Vector3 initPosition, Vector3 initVelocity, Vector3 initHitBox, float dt) {
    FPSClientState previousState;
    FPSClientState currentState;

    previousState.position = initPosition;
    previousState.velocity = initVelocity;
    previousState.hitBox = initHitBox;
    previousState.playerBox = (BoundingBox){(Vector3){previousState.position.x - previousState.hitBox.x/2,
                                                      previousState.position.y - previousState.hitBox.y/2,
                                                      previousState.position.z - previousState.hitBox.z/2},
                                            (Vector3){previousState.position.x + previousState.hitBox.x/2,
                                                      previousState.position.y + previousState.hitBox.y/2,
                                                      previousState.position.z + previousState.hitBox.z/2}};
    previousState.camera = {0};
    previousState.camera.position = previousState.position;
    previousState.camera.target = (Vector3){10.0f, 2.0f, 10.0f};
    previousState.camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    previousState.camera.fovy = 60.0f;
    previousState.camera.projection = CAMERA_PERSPECTIVE;
    previousState.alive = true;
    previousState.grounded = false;
    previousState.topCollision = false;
    previousState.coolDown = 0;
    previousState.separationVector = (Vector3){0.0f, 0.0f, 0.0f}; //TODO: check default value with joseph

    previousState.dt = dt;

    bool slotFound = false;
    size_t index = previousStatesOfPlayers.size();
    for (int i = 0; i < previousStatesOfPlayers.size(); i++) {
        if (previousStatesOfPlayers[i] == nullptr){
            slotFound = true;
            previousStatesOfPlayers[i] = &previousState;
            currentStatesOfPlayers[i] = &currentState;
            index = i;
            break;
        }
    }
    if(!slotFound){
        previousStatesOfPlayers.push_back(&previousState);
        currentStatesOfPlayers.push_back(&currentState);
        index = previousStatesOfPlayers.size() - 1;
    }

    return index;
}

void Game::updatePlayer(size_t playerIndex, bool w, bool a, bool s, bool d,Vector2 mouseDelta,bool shoot,bool space,float dt, vector<BoundingBox> &terrainList,vector<BoundingBox> &topBoxVector,bool sprint,bool crouch) {
    if(playerIndex < currentStatesOfPlayers.size() && currentStatesOfPlayers[playerIndex] != nullptr && previousStatesOfPlayers[playerIndex] != nullptr){
        Player::UpdatePlayer(*previousStatesOfPlayers[playerIndex],
                             *currentStatesOfPlayers[playerIndex],
                             w, a, s, d,
                             mouseDelta,
                             shoot,
                             space,
                             dt,
                             terrainList,
                             topBoxVector,
                             sprint,
                             crouch);
    }
   //call Player's static method using the current and previous state's using the player index:  Player::UpdatePlayer()
}

void Game::updateEntities(){
    for(int i = 0; i < currentStatesOfPlayers.size(); i++){
        Player::updateEntities(*currentStatesOfPlayers[i], currentStatesOfPlayers[i]->dt);
    }
    return;
}

void Game::checkEntityCollisions() {
    //check entities against terrain
    for(int i = 0; i < Game::terrainVector.size(); i++){
        for(int j = 0; j < currentStatesOfPlayers.size(); j++){
            for(int k = 0; k < currentStatesOfPlayers[i]->entities.size(); k++){
                if(CheckCollisionBoxes(currentStatesOfPlayers[i]->entities[k].bulletBox, terrainVector[i])){
                    cout << "Bullet Hit terrain: " << currentStatesOfPlayers[i]->entities[k].position.x << ", " << currentStatesOfPlayers[i]->entities[k].position.y << ", " << currentStatesOfPlayers[i]->entities[k].position.z << endl;
                    currentStatesOfPlayers[i]->entities[k].alive = false;
                }
            }
        }
    }

    //TODO: Implement lag compensation version of client bullet collision
    //check entities against client boxes
    for(int i = 0; i < currentStatesOfPlayers.size(); i++){
        for(int j = 0; j < currentStatesOfPlayers.size(); j++){
            for(int k = 0; k < currentStatesOfPlayers[j]->entities.size(); k++){
                if(CheckCollisionBoxes(currentStatesOfPlayers[j]->entities[k].bulletBox, currentStatesOfPlayers[i]->playerBox)){
                    currentStatesOfPlayers[j]->entities[k].alive = false;
                    currentStatesOfPlayers[i]->alive = false;
                    cout << "Client killed at loc: " << currentStatesOfPlayers[i]->position.x << ", " << currentStatesOfPlayers[i]->position.y << ", " << currentStatesOfPlayers[i]->position.z << ", " << endl;
                }
            }
        }
    }

}