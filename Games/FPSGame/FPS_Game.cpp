
#include "FPS_Game.h"

int FPS_Game::playerIDCount = 0;

const void FPS_Game::printVector3(const Vector3& vec) {
    std::cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
}

// Helper function to print BoundingBox type
const void FPS_Game::printBoundingBox(const BoundingBox& box) {
    std::cout << "Min: ";
    printVector3(box.min);
    std::cout << ", Max: ";
    printVector3(box.max);
}

// Assuming a simple structure for Camera3D type; adjust according to actual structure
const void FPS_Game::printCamera3D(const Camera3D& camera) {
    std::cout << "Position: ";
    printVector3(camera.position);
    std::cout << ", Target: ";
    printVector3(camera.target);
    std::cout << ", Up: ";
    printVector3(camera.up);
    std::cout << ", Fovy: " << camera.fovy;
}

// The structured print method for FPSClientState
const void FPS_Game::printFPSClientState(FPSClientState& state) {
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
    for(auto& entity : state.entities){
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

const bool FPS_Game::cameraIsZero(const Camera3D &camera) {
    if(camera.position.x !=0 or camera.position.y != 0 or camera.position.z != 0){
        return false;
    }
    if(camera.target.x != 0 or camera.target.y != 0 or camera.target.z != 0){
        return false;
    }
    if(camera.up.x != 0 or camera.up.y != 0 or camera.up.z != 0){
        return false;
    }

    // should I care about camera3D's fovy field???
    return true;
}

const bool FPS_Game::vector3IsZero(const Vector3 &vector) {
    if(vector.x != 0 or vector.y != 0 or vector.z != 0){
        return false;
    }
    return true;
}

const bool FPS_Game::vector2IsZero(const Vector2 &vector) {
    if(vector.x != 0 or vector.y != 0){
        return false;
    }
    return true;
}

const bool FPS_Game::boundingBoxIsZero(const BoundingBox &box) {
    if(vector3IsZero(box.max) and vector3IsZero(box.min)){
        return true;
    }
    else{
        return false;
    }
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

FPS_Game::FPS_Game(): playerStates(), deltaStates() {}

//FPS_Game::FPS_Game(): previousStatesOfPlayers(), currentStatesOfPlayers(), cumulativeDeltaStatesPlayers() {
//
//}


size_t FPS_Game::createNewPlayer(Vector3 initPosition, Vector3 initVelocity, float dt) {
    //Build my EN
    FPSClientState initState;
//    FPSClientState currentState;
//    FPSClientState cumulativeState;

    initState.position = initPosition;
    initState.velocity = initVelocity;
//    previousState.hitBox = initHitBox;
    initState.playerBox = (BoundingBox){(Vector3){initState.position.x - FPS_Player::hitbox.x/2,
                                                  initState.position.y - FPS_Player::hitbox.y/2,
                                                  initState.position.z - FPS_Player::hitbox.z/2},
                                            (Vector3){initState.position.x + FPS_Player::hitbox.x/2,
                                                      initState.position.y + FPS_Player::hitbox.y/2,
                                                      initState.position.z + FPS_Player::hitbox.z/2}};
    initState.camera = {0};
    initState.camera.position = initState.position;
    initState.camera.target = (Vector3){10.0f, 2.0f, 10.0f};
    initState.camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    initState.camera.fovy = 60.0f;
    initState.camera.projection = CAMERA_PERSPECTIVE;
    initState.alive = true;
    initState.grounded = false;
    initState.topCollision = false;
    initState.coolDown = 0;
    initState.separationVector = (Vector3){0.0f, 0.0f, 0.0f}; //TODO: check default value with joseph

    initState.dt = dt;

    bool slotFound = false;
    size_t id;

    for(int i = 0; i < playerIDCount; i++){
        if(playerStates.find(i) != playerStates.end() && deltaStates.find(i) != deltaStates.end()){
            slotFound = true;
            id = i;
            auto [statesIter, statesInserted] = playerStates.insert({i, LIFOCircularBuffer<unique_ptr<FPSClientState>>(MAX_HISTORY_PACKETS)});
            auto [deltasIter, deltaInserted] = deltaStates.insert({i, LIFOCircularBuffer<unique_ptr<FPSClientState>>(MAX_HISTORY_PACKETS)});
            if(statesInserted && deltaInserted){
                statesIter->second.push(std::make_unique<FPSClientState>(std::move(initState)));
            }
            else{
                fprintf(stderr, "Failed to Create History Buffer of type LIFOCircularBuffer\nFPS_Game.cpp - Line 173\n");
            }
        }
    }

    if(!slotFound){
        auto [statesIter, statesInserted] = playerStates.insert({playerIDCount, LIFOCircularBuffer<unique_ptr<FPSClientState>>(MAX_HISTORY_PACKETS)});
        auto [deltasIter, deltaInserted] = deltaStates.insert({playerIDCount, LIFOCircularBuffer<unique_ptr<FPSClientState>>(MAX_HISTORY_PACKETS)});
        statesIter->second.push(std::make_unique<FPSClientState>(std::move(initState)));
        id = playerIDCount;
        playerIDCount += 1;
    }

//
//    for (int i = 0; i < previousStatesOfPlayers.size(); i++) {
//        if (previousStatesOfPlayers[i] == nullptr){
//            slotFound = true;
//            previousStatesOfPlayers[i] = make_unique<FPSClientState>(previousState);
//            currentStatesOfPlayers[i] = make_unique<FPSClientState>(currentState);
//            cumulativeDeltaStatesPlayers[i] = make_unique<FPSClientState>(cumulativeState);
//            index = i;
//            break;
//        }
//    }
//    if(!slotFound){
//        previousStatesOfPlayers.push_back(make_unique<FPSClientState>(previousState));
//        currentStatesOfPlayers.push_back(make_unique<FPSClientState>(currentState));
//        cumulativeDeltaStatesPlayers.push_back(make_unique<FPSClientState>(cumulativeState));
//        index = previousStatesOfPlayers.size() - 1;
//    }
    return id;
}

void FPS_Game::updatePlayer(size_t playerIndex, bool w, bool a, bool s, bool d,Vector2 mouseDelta,bool shoot,bool space,float dt, bool sprint,bool crouch, float serverTickRate) {
    cout << "Entered FPS_Game::updatePlayer method " << endl;

    FPSClientState currentState;
    auto iter = playerStates.find(playerIndex);
    if(iter != playerStates.end()){
        unique_ptr<FPSClientState>& previousStateRef = iter->second.peek();
        if(previousStateRef){

            FPS_Player::UpdatePlayer(*previousStateRef,
                     currentState,
                     w, a, s, d,
                     mouseDelta,
                     shoot,
                     space,
                     dt,
                     FPS_Game::terrainVector,
                     FPS_Game::topBoxVector,
                     sprint,
                     crouch,
                     serverTickRate);
        }
        cout << " After FPS_Player::UpdatePlayer" << endl;

        //check for collisions between calling player and all other players. If collision generate seperation vector to push client back
        for (const auto& playerPair: playerStates){
            if(playerPair.first != playerIndex && !playerPair.second.isEmpty()){
                const unique_ptr<FPSClientState>& otherPlayerState = playerPair.second.peek();

                //TODO: ensure that checkCollision takes player's currentState.playerbox and not the previous State's playerBox;
                if(FPS_Player::CheckCollision(currentState.playerBox, otherPlayerState->playerBox, currentState.separationVector)){
                    // collision detected between calling Player and otherPlayer
                    std::cout << "In Player " << playerIndex << " updatePlayer()\n\tCollision detected with player " << playerPair.first << std::endl;
                    currentState.position = Vector3Add(currentState.position, currentState.separationVector);
                    currentState.camera.position = currentState.position;
                    currentState.camera.target = Vector3Add(currentState.camera.target, currentState.separationVector);
                }
            }
        }

        cout << "After collision player with other player collision checks" << endl;

        printFPSClientState(currentState);
        iter->second.push(std::make_unique<FPSClientState>(std::move(currentState))); // add the new currentState to the Circular Buffer, making it most current state
    }

//    ___________________________________________________________________________________________________________________________________________________________
    //    if(playerIndex < currentStatesOfPlayers.size() && currentStatesOfPlayers[playerIndex] != nullptr && previousStatesOfPlayers[playerIndex] != nullptr){
        // enters here

//        FPS_Player::UpdatePlayer(*previousStatesOfPlayers[playerIndex],
//                             *currentStatesOfPlayers[playerIndex],
//                             w, a, s, d,
//                             mouseDelta,
//                             shoot,
//                             space,
//                             dt,
//                             FPS_Game::terrainVector,
//                             FPS_Game::topBoxVector,
//                             sprint,
//                             crouch);
//        }

//    // check for collisions and seperate between host player (init function call) and all other players
//    for(int i = 0; i < currentStatesOfPlayers.size(); i++){
//        if(i != playerIndex && previousStatesOfPlayers[i] != nullptr){
//            if(FPS_Player::CheckCollision(previousStatesOfPlayers[playerIndex]->playerBox,previousStatesOfPlayers[i]->playerBox, currentStatesOfPlayers[playerIndex]->separationVector)){
//                cout << "In Player " << playerIndex << " updatePlayer()\n\tCollision detected with player " << i << endl;
//                currentStatesOfPlayers[playerIndex]->position = Vector3Add(currentStatesOfPlayers[playerIndex]->position,currentStatesOfPlayers[playerIndex]->separationVector);
//                currentStatesOfPlayers[playerIndex]->camera.position = currentStatesOfPlayers[playerIndex]->position;
//                currentStatesOfPlayers[playerIndex]->camera.target = Vector3Add(currentStatesOfPlayers[playerIndex]->camera.target,currentStatesOfPlayers[playerIndex]->separationVector);
//            }
//        }
//    }
//
//    printFPSClientState(*currentStatesOfPlayers[playerIndex]);
   //call Player's static method using the current and previous state's using the player index:  Player::UpdatePlayer()
}

void FPS_Game::updateEntities(){
    for(auto& playerPair: playerStates){
        if(!playerPair.second.isEmpty()){
            const unique_ptr<FPSClientState>& currentState = playerPair.second.peek();
            FPS_Player::updateEntities(*currentState, currentState->dt);
        }
        else{
            fprintf(stderr, "LIFO Circular Buffer is empty. updateEntities() on client %d not possible\nFPS_Game::updateEntities() - line 279\n", playerPair.first);
        }
    }

//    OLD IMPLEMENTATION:
//    for(int i = 0; i < currentStatesOfPlayers.size(); i++){
//        FPS_Player::updateEntities(*currentStatesOfPlayers[i], currentStatesOfPlayers[i]->dt);
//    }
//    return;
}

void FPS_Game::checkEntityCollisions() {
    // Check for collisions between terrain and the entities of every single entity.
    // Entities marked as .alive = false, will be
    for(const auto & terrain : FPS_Game::terrainVector){
        for(auto& playerPair: playerStates){
            auto& playerState = playerPair.second.peek();
            for(auto& entity : playerState->entities) {
                if(CheckCollisionBoxes(entity.bulletBox, terrain) && entity.claimed && entity.alive){
                    cout << "Bullet Hit terrain: " << entity.position.x << ", " << entity.position.y << ", " << entity.position.z << endl;
                    entity.alive = false;
                }
            }
        }
    }

    for(auto& playerPair: playerStates){
        auto& shooterState = playerPair.second.peek();
        for(auto& entity: shooterState->entities){
            for(auto& otherPlayerPair : playerStates) {
                if (playerPair.first == otherPlayerPair.first) {
                    continue;
                }

                auto &targetState = otherPlayerPair.second.peek();
                if (CheckCollisionBoxes(entity.bulletBox, targetState->playerBox)) {
                    entity.alive = false;
                    targetState->alive = false;
                    cout << "Client killed at loc: " << targetState->position.x << ", " << targetState->position.y
                         << ", " << targetState->position.z << ", " << endl;
                }
            }
        }
    }


//    OLD IMPLEMENTATION
//    //check entities against terrain
//    for(int i = 0; i < FPS_Game::terrainVector.size(); i++){
//        for(int j = 0; j < currentStatesOfPlayers.size(); j++){
//            for(int k = 0; k < currentStatesOfPlayers[j]->entities.size(); k++){
//                if(CheckCollisionBoxes(currentStatesOfPlayers[j]->entities[k].bulletBox, terrainVector[i])){
//                    cout << "Bullet Hit terrain: " << currentStatesOfPlayers[j]->entities[k].position.x << ", " << currentStatesOfPlayers[j]->entities[k].position.y << ", " << currentStatesOfPlayers[j]->entities[k].position.z << endl;
//                    currentStatesOfPlayers[j]->entities[k].alive = false;
//                }
//
//            }
//        }
//    }
//
//    //TODO: Implement lag compensation version of client bullet collision
//    //check entities against client boxes
//    for(int i = 0; i < currentStatesOfPlayers.size(); i++){
//        for(int j = 0; j < currentStatesOfPlayers.size(); j++){
//            for(int k = 0; k < currentStatesOfPlayers[j]->entities.size(); k++){
//                if(CheckCollisionBoxes(currentStatesOfPlayers[j]->entities[k].bulletBox, currentStatesOfPlayers[i]->playerBox)){
//                    currentStatesOfPlayers[j]->entities[k].alive = false;
//                    currentStatesOfPlayers[i]->alive = false;
//                    cout << "Client killed at loc: " << currentStatesOfPlayers[i]->position.x << ", " << currentStatesOfPlayers[i]->position.y << ", " << currentStatesOfPlayers[i]->position.z << ", " << endl;
//                }
//            }
//        }
//    }
}

void FPS_Game::calculateDeltas() {

    FPSClientState delta = FPSClientState();
    for(auto& playerPair: playerStates){
        size_t playerIndex = playerPair.first;
        auto& deltas = deltaStates.at(playerIndex);
        auto& states = playerPair.second;

        if(states.getCount() >= 2){
            for(int i = states.getCount() - 2; i >= 0; --i){
                auto& newerState  = states.peekAtIndex(i);
                auto& olderState = states.peekAtIndex(i+1);
                // i is the newer, and i+1 is older ( (i) - (i+1) )
                delta.separationVector = Vector3Subtract(newerState->separationVector, olderState->separationVector);
                delta.topCollision = newerState->topCollision;
                delta.grounded = newerState->grounded;
                delta.space = newerState->space;
                delta.playerBox = {Vector3Subtract(newerState->playerBox.min, olderState->playerBox.min),
                                   Vector3Subtract(newerState->playerBox.max, olderState->playerBox.max)};
                delta.coolDown = newerState->coolDown - olderState->coolDown;
                delta.camera = {Vector3Subtract(newerState->camera.position, olderState->camera.position),
                                Vector3Subtract(newerState->camera.target, olderState->camera.target),
                                Vector3Subtract(newerState->camera.up, olderState->camera.up),
                                newerState->camera.fovy - olderState->camera.fovy,
                                newerState->camera.projection - olderState->camera.projection};
                delta.position = Vector3Subtract(newerState->position, olderState->position);
                delta.velocity = Vector3Subtract(newerState->velocity, olderState->velocity);
                delta.alive = newerState->alive;

                 //TODO:  Create a weighted dt
                 delta.dt = newerState->dt - olderState->dt;

                 //iterate through the entities
                 for(int j = 0; j < newerState->entities.size(); j++){
                    auto& newerEntity = newerState->entities[j];

                     // edge cases: (1) if a new entity was created between states, newerState's entities size > olderState's entities size
                     //             (2) if an entity was deleted between state's newEntity will have claimed field set to false
                     // Simply add the newerEntity (if new at that position a skeleton exists) if deleted it will be skeleton with claimed set to false
                     if(j >= olderState->entities.size() || !newerEntity.claimed){
                         delta.entities.push_back(std::move(newerEntity));
                     }
                     else{
                         auto& olderEntity = olderState->entities[j];
                         FPSEntityState entityDelta;
                         entityDelta.position = Vector3Subtract(newerEntity.position, olderEntity.position);
                         entityDelta.bulletBox = {Vector3Subtract(newerEntity.bulletBox.min, olderEntity.bulletBox.min),
                                                  Vector3Subtract(newerEntity.bulletBox.max, olderEntity.bulletBox.max)};
                         entityDelta.velocity = Vector3Subtract(newerEntity.velocity, olderEntity.velocity);
                         entityDelta.alive = newerEntity.alive;
                         entityDelta.claimed = newerEntity.claimed;
                         delta.entities.push_back(std::move(entityDelta));
                     }
                 }

                 // fully calculated the delta for a client, now appended from oldest -> newest to the player's delta CircularBuffer
                 deltas.push(std::make_unique<FPSClientState>(std::move(delta)));
                 delta = FPSClientState(); // reset to skeleton for the next set of newer and older states
            }

        }
    }
//
//    for(int i = 0; i < currentStatesOfPlayers.size(); i++){
//        for(int i = 0; )
//
//        // For each client "subtract" previous from current (current - previous). Choose current for Booleans
//
//        cumulativeDeltaStatesPlayers[i]->dt = 0.2f*currentStatesOfPlayers[i]->dt + cumulativeDeltaStatesPlayers[i]->dt*0.8f; // calculate a rolling average of the dt
//        cumulativeDeltaStatesPlayers[i]->separationVector = Vector3Subtract(currentStatesOfPlayers[i]->separationVector, previousStatesOfPlayers[i]->separationVector);
//        cumulativeDeltaStatesPlayers[i]->topCollision = currentStatesOfPlayers[i]->topCollision;
//        cumulativeDeltaStatesPlayers[i]->grounded = currentStatesOfPlayers[i]->grounded;
//        cumulativeDeltaStatesPlayers[i]->space = currentStatesOfPlayers[i]->space;
//        cumulativeDeltaStatesPlayers[i]->playerBox = {Vector3Subtract(currentStatesOfPlayers[i]->playerBox.min, previousStatesOfPlayers[i]->playerBox.min),
//                                                      Vector3Subtract(currentStatesOfPlayers[i]->playerBox.max, previousStatesOfPlayers[i]->playerBox.max)};
//        // skip sending the hitbox (static on server and client)
//        cumulativeDeltaStatesPlayers[i]->coolDown = currentStatesOfPlayers[i]->coolDown - previousStatesOfPlayers[i]->coolDown;
//
//        cumulativeDeltaStatesPlayers[i]->camera = {Vector3Subtract(currentStatesOfPlayers[i]->camera.position, previousStatesOfPlayers[i]->camera.position),
//                                                   Vector3Subtract(currentStatesOfPlayers[i]->camera.target, previousStatesOfPlayers[i]->camera.target),
//                                                   Vector3Subtract(currentStatesOfPlayers[i]->camera.up, previousStatesOfPlayers[i]->camera.up),
//                                                   currentStatesOfPlayers[i]->camera.fovy - previousStatesOfPlayers[i]->camera.fovy,
//                                                   currentStatesOfPlayers[i]->camera.projection - previousStatesOfPlayers[i]->camera.projection};
//        cumulativeDeltaStatesPlayers[i]->position = Vector3Subtract(currentStatesOfPlayers[i]->position, previousStatesOfPlayers[i]->position);
//        cumulativeDeltaStatesPlayers[i]->velocity = Vector3Subtract(currentStatesOfPlayers[i]->velocity, previousStatesOfPlayers[i]->velocity);
//        cumulativeDeltaStatesPlayers[i]->alive = currentStatesOfPlayers[i]->alive;
//        // skip sending the camera perspective
//
//
//
//        // calculate the entities' delta values
//        // Edge case: If a new entity is created, size(currentState's entities) > size(previousState's entities)
//        //      Expand the size of Delta State's size to current State's size
//        // Edge case: If an is killed (NOT alive), checkEntityCollisions() sets entities that collide to dead, then next server tick it is deleted. How to handle when updateEntities() deletes entity
//        //      size(previous States) > size(currentStates)
//
//
//
//        // If the deltas and current state have mismatched size, resize delta to match and fill with default states
//        cout << "i = " << i << ", Current State size: " << currentStatesOfPlayers[i]->entities.size() << "\tPrevious state size: " << previousStatesOfPlayers[i]->entities.size() << "\t Delta state size: " << cumulativeDeltaStatesPlayers[i]->entities.size() << endl;
//
////        size_t oldSize = cumulativeDeltaStatesPlayers[i]->entities.size();
////        cout << "old Size: " << oldSize << endl;
////        if(oldSize < currentStatesOfPlayers[i]->entities.size()){
////            cumulativeDeltaStatesPlayers[i]->entities.resize(currentStatesOfPlayers[i]->entities.size());
////
////            for(int j = oldSize; j < currentStatesOfPlayers[i]->entities.size(); j++){
////                cumulativeDeltaStatesPlayers[i]->entities[j] = FPSEntityState();
////            }
////        }
//
//        for(int j = 0; j < currentStatesOfPlayers[i]->entities.size(); j++){
////            cout << "Current State size: " << currentStatesOfPlayers[i]->entities.size() << "\tPrevious state size: " << previousStatesOfPlayers[i]->entities.size() << "\t Delta state size: " << cumulativeDeltaStatesPlayers[i]->entities.size() << endl;
//            // skip bullet model as it is static
//
//            cumulativeDeltaStatesPlayers[i]->entities[j].alive = currentStatesOfPlayers[i]->entities[j].alive;
//            cout << "Break 1 , i = " << i << ", j = "  << j << endl;
//
//            if(previousStatesOfPlayers[i]->entities.size() < currentStatesOfPlayers[i]->entities.size()){
//                cumulativeDeltaStatesPlayers[i]->entities[j].bulletBox = {Vector3Subtract(currentStatesOfPlayers[i]->entities[j].bulletBox.min, Vector3 {0, 0, 0}),
//                                                                          Vector3Subtract(currentStatesOfPlayers[i]->entities[j].bulletBox.max, Vector3 {0, 0, 0})};
//                cumulativeDeltaStatesPlayers[i]->entities[j].position = Vector3Subtract(currentStatesOfPlayers[i]->entities[j].position, Vector3 {0, 0, 0});
//                cumulativeDeltaStatesPlayers[i]->entities[j].velocity = Vector3Subtract(currentStatesOfPlayers[i]->entities[j].velocity, Vector3 {0, 0, 0});
//            }
//            else{
//                cumulativeDeltaStatesPlayers[i]->entities[j].bulletBox = {Vector3Subtract(currentStatesOfPlayers[i]->entities[j].bulletBox.min, previousStatesOfPlayers[i]->entities[j].bulletBox.min),
//                                                                          Vector3Subtract(currentStatesOfPlayers[i]->entities[j].bulletBox.max, previousStatesOfPlayers[i]->entities[j].bulletBox.max)};
//                cout << "Break 2 , i = " << i << ", j = "  << j << endl;
//                cumulativeDeltaStatesPlayers[i]->entities[j].position = Vector3Subtract(currentStatesOfPlayers[i]->entities[j].position, previousStatesOfPlayers[i]->entities[j].position);
//                cout << "Break 3 , i = " << i << ", j = "  << j << endl;
//                cumulativeDeltaStatesPlayers[i]->entities[j].velocity = Vector3Subtract(currentStatesOfPlayers[i]->entities[j].velocity, previousStatesOfPlayers[i]->entities[j].velocity);
//                cout << "Break 4 , i = " << i << ", j = "  << j << endl;
//            }
////            cumulativeDeltaStatesPlayers[i]->entities[j].bulletBox = BoundingBox {Vector3 {0, 0, 0}, Vector3 {0, 0, 0}};
//            // entity's hitbox is static, so no update is required.
//        }
//        cout << "Finished calculating Deltas " << endl;
//    }
//
//    printFPSClientState(*cumulativeDeltaStatesPlayers[0]);
}

//void FPS_Game::updatePreviousStates() {
//    for(int i = 0; i < previousStatesOfPlayers.size(); i++){
//        if(currentStatesOfPlayers[i] != nullptr){
//            if(previousStatesOfPlayers[i] == nullptr) {
//                previousStatesOfPlayers[i] = make_unique<FPSClientState>();
//            }
//            *previousStatesOfPlayers[i] = *currentStatesOfPlayers[i];
//        }
//        else{
////            delete previousStatesOfPlayers[i]; // CAUSES ISSUES!
//            previousStatesOfPlayers[i] = nullptr;
//        }
//
////       delete previousStatesOfPlayers[i];
////       previousStatesOfPlayers[i] = nullptr;
////
////       if(currentStatesOfPlayers[i] != nullptr){
////           previousStatesOfPlayers[i] = new FPSClientState(*currentStatesOfPlayers[i]);
////       }
//    }
//}

//unique_ptr<FPSClientState> FPS_Game::getPlayerCurrentState(size_t index) {
//    return std::move(currentStatesOfPlayers[index]);
//}
//
//unique_ptr<FPSClientState> FPS_Game::getPlayerPreviousState(size_t index) {
//    return std::move(previousStatesOfPlayers[index]);
//}

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
//
//const vector<unique_ptr<FPSClientState>> &FPS_Game::getPreviousStatesOfPlayers() const {
//    return previousStatesOfPlayers;
//}
//
//const vector<unique_ptr<FPSClientState>> &FPS_Game::getCurrentStatesOfPlayers() const {
//    return currentStatesOfPlayers;
//}
//
//const vector<unique_ptr<FPSClientState>> &FPS_Game::getCumulativeDeltaStatesPlayers() const {
//    return cumulativeDeltaStatesPlayers;
//}

