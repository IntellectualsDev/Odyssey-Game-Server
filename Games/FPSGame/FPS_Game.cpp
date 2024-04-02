
#include "FPS_Game.h"


void printVector3(const Vector3& vec) {
    std::cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
}

// Helper function to print BoundingBox type
void printBoundingBox(const BoundingBox& box) {
    std::cout << "Min: ";
    printVector3(box.min);
    std::cout << ", Max: ";
    printVector3(box.max);
}

// Assuming a simple structure for Camera3D type; adjust according to actual structure
void printCamera3D(const Camera3D& camera) {
    std::cout << "Position: ";
    printVector3(camera.position);
    std::cout << ", Target: ";
    printVector3(camera.target);
    std::cout << ", Up: ";
    printVector3(camera.up);
    std::cout << ", Fovy: " << camera.fovy;
}

// The structured print method for FPSClientState
void printFPSClientState(FPSClientState& state) {
    std::cout << "FPSClientState {" << std::endl;
    std::cout << "  dt: " << state.dt << std::endl;
    std::cout << "  SeparationVector: ";
    printVector3(state.separationVector);
    std::cout << std::endl;
    std::cout << "  TopCollision: " << std::boolalpha << state.topCollision << std::endl;
    std::cout << "  Grounded: " << state.grounded << std::endl;
    std::cout << "  Space: " << state.space << std::endl;
    std::cout << "  PlayerBox: ";
    printBoundingBox(state.playerBox);
    std::cout << std::endl;
    std::cout << "  CoolDown: " << state.coolDown << std::endl;
    std::cout << "  Entities: [size=" << state.entities.size() << "]" << std::endl; // Detailed entity printing can be added here
    for(auto entity : state.entities){
        cout << "\tPosition: ";
        printVector3(entity.position);
        cout << endl <<  "\tVelocity: ";
        printVector3(entity.velocity);
        cout << endl << "\tEntity Alive: " << entity.alive << endl;
    }
    std::cout << "  Camera: ";
    printCamera3D(state.camera);
    std::cout << std::endl;
    std::cout << "  Position: ";
    printVector3(state.position);
    std::cout << std::endl;
    std::cout << "  Velocity: ";
    printVector3(state.velocity);
    std::cout << std::endl;
    std::cout << "  Alive: " << state.alive << std::endl;
    std::cout << "  CameraMode: " << state.cameraMode << std::endl;
    std::cout << "}" << std::endl;
}

std::vector<BoundingBox> FPS_Game::terrainVector = {
        {(Vector3){-16.0f - FPS_Game::wallWidth/2, 2.5f - FPS_Game::wallHeight/2, 0.0f - FPS_Game::wallLength/2},
                (Vector3){-16.0f + FPS_Game::wallWidth/2, 2.5f + FPS_Game::wallHeight/2, 0.0f + FPS_Game::wallLength/2}},

        {(Vector3){16.0f - FPS_Game::wallWidth/2, 2.5f - FPS_Game::wallHeight/2, 0.0f - FPS_Game::wallLength/2},
                (Vector3){16.0f + FPS_Game::wallWidth/2, 2.5f + FPS_Game::wallHeight/2, 0.0f + FPS_Game::wallLength/2}},

        {(Vector3){0.0f - FPS_Game::wallLength/2, 2.5f - FPS_Game::wallHeight/2, 16.0f - FPS_Game::wallWidth/2},
                (Vector3){0.0f + FPS_Game::wallLength/2, 2.5f + FPS_Game::wallHeight/2, 16.0f + FPS_Game::wallWidth/2}},

        {(Vector3){-FPS_Game::floorLength/2, -0.5, -FPS_Game::floorLength/2},
                (Vector3){FPS_Game::floorLength/2, 0.5, FPS_Game::floorLength/2}}
};

std::vector<BoundingBox> FPS_Game::topBoxVector = {
        {(Vector3){-16.0f - 0.5, 4.5f, 0.0f -FPS_Game::wallLength/2},(Vector3){-16.0f + 0.5, 2.5f + FPS_Game::wallHeight/2, 0.0f +FPS_Game::wallLength/2}},
        {(Vector3){16.0f - 0.5, 4.5f, 0.0f -FPS_Game::wallLength/2},(Vector3){16.0f + 0.5, 2.5f + FPS_Game::wallHeight/2, 0.0f +FPS_Game::wallLength/2}},
        {(Vector3){0.0f - FPS_Game::wallLength/2, 4.5f, 16.0f -0.5},(Vector3){0.0f + FPS_Game::wallLength/2, 2.5f + FPS_Game::wallHeight/2, 16.0f + 0.5}},
        {(Vector3){-FPS_Game::floorLength/2,-0.5,-FPS_Game::floorLength/2},(Vector3){FPS_Game::floorLength/2,0.5,FPS_Game::floorLength/2}}
};


FPS_Game::FPS_Game(): previousStatesOfPlayers(), currentStatesOfPlayers(), cumulativeDeltaStatesPlayers() {

}

size_t FPS_Game::createNewPlayer(Vector3 initPosition, Vector3 initVelocity, float dt) {
    //Build my EN
    FPSClientState previousState;
    FPSClientState currentState;
    FPSClientState cumulativeState;

    previousState.position = initPosition;
    previousState.velocity = initVelocity;
//    previousState.hitBox = initHitBox;
    previousState.playerBox = (BoundingBox){(Vector3){previousState.position.x - FPS_Player::hitbox.x/2,
                                                      previousState.position.y - FPS_Player::hitbox.y/2,
                                                      previousState.position.z - FPS_Player::hitbox.z/2},
                                            (Vector3){previousState.position.x + FPS_Player::hitbox.x/2,
                                                      previousState.position.y + FPS_Player::hitbox.y/2,
                                                      previousState.position.z + FPS_Player::hitbox.z/2}};
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
            previousStatesOfPlayers[i] = make_unique<FPSClientState>(previousState);
            currentStatesOfPlayers[i] = make_unique<FPSClientState>(currentState);
            cumulativeDeltaStatesPlayers[i] = make_unique<FPSClientState>(cumulativeState);
            index = i;
            break;
        }
    }
    if(!slotFound){
        previousStatesOfPlayers.push_back(make_unique<FPSClientState>(previousState));
        currentStatesOfPlayers.push_back(make_unique<FPSClientState>(currentState));
        cumulativeDeltaStatesPlayers.push_back(make_unique<FPSClientState>(cumulativeState));
        index = previousStatesOfPlayers.size() - 1;
    }

    return index;
}

void FPS_Game::updatePlayer(size_t playerIndex, bool w, bool a, bool s, bool d,Vector2 mouseDelta,bool shoot,bool space,float dt, bool sprint,bool crouch) {
    if(playerIndex < currentStatesOfPlayers.size() && currentStatesOfPlayers[playerIndex] != nullptr && previousStatesOfPlayers[playerIndex] != nullptr){
        // enters here
        FPS_Player::UpdatePlayer(*previousStatesOfPlayers[playerIndex],
                             *currentStatesOfPlayers[playerIndex],
                             w, a, s, d,
                             mouseDelta,
                             shoot,
                             space,
                             dt,
                             FPS_Game::terrainVector,
                             FPS_Game::topBoxVector,
                             sprint,
                             crouch);
    }

    // check for collisions and seperate between host player (init function call) and all other players
    for(int i = 0; i < currentStatesOfPlayers.size(); i++){
        if(i != playerIndex && previousStatesOfPlayers[i] != nullptr){
            if(FPS_Player::CheckCollision(previousStatesOfPlayers[playerIndex]->playerBox,previousStatesOfPlayers[i]->playerBox, currentStatesOfPlayers[playerIndex]->separationVector)){
                cout << "In Player " << playerIndex << " updatePlayer()\n\tCollision detected with player " << i << endl;
                currentStatesOfPlayers[playerIndex]->position = Vector3Add(currentStatesOfPlayers[playerIndex]->position,currentStatesOfPlayers[playerIndex]->separationVector);
                currentStatesOfPlayers[playerIndex]->camera.position = currentStatesOfPlayers[playerIndex]->position;
                currentStatesOfPlayers[playerIndex]->camera.target = Vector3Add(currentStatesOfPlayers[playerIndex]->camera.target,currentStatesOfPlayers[playerIndex]->separationVector);
            }
        }
    }

    printFPSClientState(*currentStatesOfPlayers[playerIndex]);
   //call Player's static method using the current and previous state's using the player index:  Player::UpdatePlayer()
}

void FPS_Game::updateEntities(){
    for(int i = 0; i < currentStatesOfPlayers.size(); i++){
        FPS_Player::updateEntities(*currentStatesOfPlayers[i], currentStatesOfPlayers[i]->dt);
    }
    return;
}

void FPS_Game::checkEntityCollisions() {
    //check entities against terrain
    for(int i = 0; i < FPS_Game::terrainVector.size(); i++){
        for(int j = 0; j < currentStatesOfPlayers.size(); j++){
            for(int k = 0; k < currentStatesOfPlayers[j]->entities.size(); k++){
                if(CheckCollisionBoxes(currentStatesOfPlayers[j]->entities[k].bulletBox, terrainVector[i])){
                    cout << "Bullet Hit terrain: " << currentStatesOfPlayers[j]->entities[k].position.x << ", " << currentStatesOfPlayers[j]->entities[k].position.y << ", " << currentStatesOfPlayers[j]->entities[k].position.z << endl;
                    currentStatesOfPlayers[j]->entities[k].alive = false;
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

void FPS_Game::calculateDeltas() {
    for(int i = 0; i < currentStatesOfPlayers.size(); i++){
        // For each client "subtract" previous from current (current - previous). Choose current for Booleans

        cumulativeDeltaStatesPlayers[i]->dt = 0.2f*currentStatesOfPlayers[i]->dt + cumulativeDeltaStatesPlayers[i]->dt*0.8f; // calculate a rolling average of the dt
        cumulativeDeltaStatesPlayers[i]->separationVector = Vector3Subtract(currentStatesOfPlayers[i]->separationVector, previousStatesOfPlayers[i]->separationVector);
        cumulativeDeltaStatesPlayers[i]->topCollision = currentStatesOfPlayers[i]->topCollision;
        cumulativeDeltaStatesPlayers[i]->grounded = currentStatesOfPlayers[i]->grounded;
        cumulativeDeltaStatesPlayers[i]->space = currentStatesOfPlayers[i]->space;
        cumulativeDeltaStatesPlayers[i]->playerBox = {Vector3Subtract(currentStatesOfPlayers[i]->playerBox.min, previousStatesOfPlayers[i]->playerBox.min),
                                                      Vector3Subtract(currentStatesOfPlayers[i]->playerBox.max, previousStatesOfPlayers[i]->playerBox.max)};
        // skip sending the hitbox (static on server and client)
        cumulativeDeltaStatesPlayers[i]->coolDown = currentStatesOfPlayers[i]->coolDown - previousStatesOfPlayers[i]->coolDown;

        cumulativeDeltaStatesPlayers[i]->camera = {Vector3Subtract(currentStatesOfPlayers[i]->camera.position, previousStatesOfPlayers[i]->camera.position),
                                                   Vector3Subtract(currentStatesOfPlayers[i]->camera.target, previousStatesOfPlayers[i]->camera.target),
                                                   Vector3Subtract(currentStatesOfPlayers[i]->camera.up, previousStatesOfPlayers[i]->camera.up),
                                                   currentStatesOfPlayers[i]->camera.fovy - previousStatesOfPlayers[i]->camera.fovy,
                                                   currentStatesOfPlayers[i]->camera.projection - previousStatesOfPlayers[i]->camera.projection};
        cumulativeDeltaStatesPlayers[i]->position = Vector3Subtract(currentStatesOfPlayers[i]->position, previousStatesOfPlayers[i]->position);
        cumulativeDeltaStatesPlayers[i]->velocity = Vector3Subtract(currentStatesOfPlayers[i]->velocity, previousStatesOfPlayers[i]->velocity);
        cumulativeDeltaStatesPlayers[i]->alive = currentStatesOfPlayers[i]->alive;
        // skip sending the camera perspective



        // calculate the entities' delta values
        // If the deltas and current state have mismatched size, resize delta to match and fill with default states
        cout << "i = " << i << ", Current State size: " << currentStatesOfPlayers[i]->entities.size() << "\tPrevious state size: " << previousStatesOfPlayers[i]->entities.size() << "\t Delta state size: " << cumulativeDeltaStatesPlayers[i]->entities.size() << endl;

        size_t oldSize = cumulativeDeltaStatesPlayers[i]->entities.size();
        cout << "old Size: " << oldSize << endl;
        if(oldSize < currentStatesOfPlayers[i]->entities.size()){
            cumulativeDeltaStatesPlayers[i]->entities.resize(currentStatesOfPlayers[i]->entities.size());

            for(int j = oldSize; j < currentStatesOfPlayers[i]->entities.size(); j++){
                cumulativeDeltaStatesPlayers[i]->entities[j] = FPSEntityState();
            }
        }

        for(int j = 0; j < currentStatesOfPlayers[i]->entities.size(); j++){
//            cout << "Current State size: " << currentStatesOfPlayers[i]->entities.size() << "\tPrevious state size: " << previousStatesOfPlayers[i]->entities.size() << "\t Delta state size: " << cumulativeDeltaStatesPlayers[i]->entities.size() << endl;
            // skip bullet model as it is static

            cumulativeDeltaStatesPlayers[i]->entities[j].alive = currentStatesOfPlayers[i]->entities[j].alive;
            cout << "Break 1 , i = " << i << ", j = "  << j << endl;

            if(previousStatesOfPlayers[i]->entities.size() < currentStatesOfPlayers[i]->entities.size()){
                cumulativeDeltaStatesPlayers[i]->entities[j].bulletBox = {Vector3Subtract(currentStatesOfPlayers[i]->entities[j].bulletBox.min, Vector3 {0, 0, 0}),
                                                                          Vector3Subtract(currentStatesOfPlayers[i]->entities[j].bulletBox.max, Vector3 {0, 0, 0})};
                cumulativeDeltaStatesPlayers[i]->entities[j].position = Vector3Subtract(currentStatesOfPlayers[i]->entities[j].position, Vector3 {0, 0, 0});
                cumulativeDeltaStatesPlayers[i]->entities[j].velocity = Vector3Subtract(currentStatesOfPlayers[i]->entities[j].velocity, Vector3 {0, 0, 0});
            }
            else{
                cumulativeDeltaStatesPlayers[i]->entities[j].bulletBox = {Vector3Subtract(currentStatesOfPlayers[i]->entities[j].bulletBox.min, previousStatesOfPlayers[i]->entities[j].bulletBox.min),
                                                                          Vector3Subtract(currentStatesOfPlayers[i]->entities[j].bulletBox.max, previousStatesOfPlayers[i]->entities[j].bulletBox.max)};
                cout << "Break 2 , i = " << i << ", j = "  << j << endl;
                cumulativeDeltaStatesPlayers[i]->entities[j].position = Vector3Subtract(currentStatesOfPlayers[i]->entities[j].position, previousStatesOfPlayers[i]->entities[j].position);
                cout << "Break 3 , i = " << i << ", j = "  << j << endl;
                cumulativeDeltaStatesPlayers[i]->entities[j].velocity = Vector3Subtract(currentStatesOfPlayers[i]->entities[j].velocity, previousStatesOfPlayers[i]->entities[j].velocity);
                cout << "Break 4 , i = " << i << ", j = "  << j << endl;
            }
//            cumulativeDeltaStatesPlayers[i]->entities[j].bulletBox = BoundingBox {Vector3 {0, 0, 0}, Vector3 {0, 0, 0}};
            // entity's hitbox is static, so no update is required.
        }
        cout << "Finished calculating Deltas " << endl;
    }

    printFPSClientState(*cumulativeDeltaStatesPlayers[0]);
}

void FPS_Game::updatePreviousStates() {
    for(int i = 0; i < previousStatesOfPlayers.size(); i++){
        if(currentStatesOfPlayers[i] != nullptr){
            if(previousStatesOfPlayers[i] == nullptr) {
                previousStatesOfPlayers[i] = make_unique<FPSClientState>();
            }
            *previousStatesOfPlayers[i] = *currentStatesOfPlayers[i];
        }
        else{
//            delete previousStatesOfPlayers[i]; // CAUSES ISSUES!
            previousStatesOfPlayers[i] = nullptr;
        }

//       delete previousStatesOfPlayers[i];
//       previousStatesOfPlayers[i] = nullptr;
//
//       if(currentStatesOfPlayers[i] != nullptr){
//           previousStatesOfPlayers[i] = new FPSClientState(*currentStatesOfPlayers[i]);
//       }
    }
}

unique_ptr<FPSClientState> FPS_Game::getPlayerCurrentState(size_t index) {
    return std::move(currentStatesOfPlayers[index]);
}

unique_ptr<FPSClientState> FPS_Game::getPlayerPreviousState(size_t index) {
    return std::move(previousStatesOfPlayers[index]);
}

const float FPS_Game::getWallWidth() {
    return wallWidth;
}

const float FPS_Game::getWallHeight() {
    return wallHeight;
}

const float FPS_Game::getWallLength() {
    return wallLength;
}

const float FPS_Game::getFloorLength() {
    return floorLength;
}

//const vector<FPSClientState> &FPS_Game::getBuffer() {
//    return buffer;
//}

const vector<BoundingBox> &FPS_Game::getTerrainVector() {
    return terrainVector;
}

const vector<BoundingBox> &FPS_Game::getTopBoxVector() {
    return topBoxVector;
}

const vector<unique_ptr<FPSClientState>> &FPS_Game::getPreviousStatesOfPlayers() const {
    return previousStatesOfPlayers;
}

const vector<unique_ptr<FPSClientState>> &FPS_Game::getCurrentStatesOfPlayers() const {
    return currentStatesOfPlayers;
}

const vector<unique_ptr<FPSClientState>> &FPS_Game::getCumulativeDeltaStatesPlayers() const {
    return cumulativeDeltaStatesPlayers;
}

