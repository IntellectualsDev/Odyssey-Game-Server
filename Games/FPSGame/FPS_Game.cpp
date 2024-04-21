
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

    initState.position = initPosition;
    initState.velocity = initVelocity;
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

    return id;
}

/*
 * Handles Desynchronized Server and Client tick rates by updating a mapping of Client to Server Ticks. It is resilient to variable/oscillating Client tick rates, and
 * can handle both server and client tick wraparounds, as long as the Server tick rate > client tick rate
 */
void FPS_Game::mapDesyncClientandServerTicks(size_t playerIndex, int serverTick, int clientTick) {
    auto iterClientToServer = clientToServerTick.find(playerIndex);
    auto iterServerToClient = serverToClientTick.find(playerIndex);

    if(iterClientToServer == clientToServerTick.end()){
        fprintf(stderr, "Error: Async Client to Server Tick Map for Client %d is empty or does not exist.\n\tFPS_Game.cpp ~ mapAsyncClientandServerTicks()", playerIndex);
        return;
    }
    if (iterServerToClient == serverToClientTick.end()){
        fprintf(stderr, "Error: Async Server to Client Tick Map for Client %d is empty or does not exist.\n\tFPS_Game.cpp ~ mapAsyncClientandServerTicks()", playerIndex);
        return;
    }
    // map client tick to server tick
    iterClientToServer->second[clientTick] = serverTick;
    // map server tick to client tick
    iterServerToClient->second[serverTick] = clientTick;
}

void FPS_Game::updatePlayer(int playerIndex, int serverTick, vector<const Input *> inputs, float serverTickRate) {
    FPSClientState currentState;
    auto iter = playerStates.find(playerIndex);
    if(iter == playerStates.end()){
        fprintf(stderr, "Error: retrieving Circular State Buffer for player ID # %d. playerID not found in playerStates map.\n\tFPS_Game.cpp ~  updatePlayer(vector<const Input *>)", playerIndex);
    }

    std::unique_ptr<FPSClientState>& mostRecentState = iter->second.peek();

    // clone the most Recent State and undo the outer unique_ptr to create a mutable version of the most recent state
    std::unique_ptr<FPSClientState> mostRecentStateClone = (*mostRecentState).clone();

    if(mostRecentStateClone != nullptr){
        FPSClientState mostRecentStateMutable = std::move(*(mostRecentStateClone));
        // Simulate all the inputs within the to-be processed inputs vector and accumulate inputs
        for(auto& input: inputs){
            FPS_Player::UpdatePlayer(mostRecentStateMutable,
                                     currentState,
                                     input->w(),
                                     input->a(),
                                     input->s(),
                                     input->d(),
                                     (Vector2){input->mouse_delta()->x(), input->mouse_delta()->y()},
                                     input->shoot(),
                                     input->space(),
                                     input->tick()->dt(),
                                     FPS_Game::terrainVector,
                                     FPS_Game::topBoxVector,
                                     input->sprint(),
                                     input->crouch(),
                                     serverTickRate);

            // update the mostRecentState and reset the currentState
            mostRecentStateMutable = std::move(currentState);
            currentState = FPSClientState();
        }
        // At the end of the loop the mostRecentStateMutable should hold the newest state

        // Check for collisions between calling player and all other players. If collision generate seperation vector to push client back
        for (const auto& playerPair: playerStates){
            if(playerPair.first != playerIndex && !playerPair.second.isEmpty()){
                const unique_ptr<FPSClientState>& otherPlayerState = playerPair.second.peek();

                //TODO: ensure that checkCollision takes player's currentState.playerbox and not the previous State's playerBox;
                if(FPS_Player::CheckCollision(mostRecentStateMutable.playerBox, otherPlayerState->playerBox, mostRecentStateMutable.separationVector)){
                    // collision detected between calling Player and otherPlayer
                    fprintf(stdout, "Player v Player {%d,%d} collision detected.\n\t in FPS_Gane.cpp ~ updatePlayer(vector<inputs>)", playerIndex, playerPair.first);
                    mostRecentStateMutable.position = Vector3Add(mostRecentStateMutable.position, mostRecentStateMutable.separationVector);
                    mostRecentStateMutable.camera.position = mostRecentStateMutable.position;
                    mostRecentStateMutable.camera.target = Vector3Add(mostRecentStateMutable.camera.target, mostRecentStateMutable.separationVector);
                }
            }
        }

        printFPSClientState(mostRecentStateMutable);
        mostRecentStateMutable.tick = serverTick; // assign the current server tick to this new state
        iter->second.push(std::make_unique<FPSClientState>(std::move(mostRecentStateMutable)));
    }
}

void FPS_Game::updatePlayer(size_t playerIndex, bool w, bool a, bool s, bool d,Vector2 mouseDelta,bool shoot,bool space,float dt, bool sprint,bool crouch, float serverTickRate) {
    cout << "Entered FPS_Game::updatePlayer method " << endl;

    //TODO: move the head - 1 state to the head, and then during the batch iteration use "previousStateRef = iter->second.peek();" in the FPS_Player previousStateRef parameter, but after
    // FPS_Player::UpdatePlayer() check player v player collisions and MOST IMPORTANTLY instead of pushing new state use rewriteAtIndex() to rewrite. Allowing inputs in batch to build upon
    // each other (packet-loss batch processing)

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

        // TODO: replace with CircularBuffer's rewriteAtIndex() method to change current Server tick's state (packet-loss batch processing)
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

// TODO: convert to checkEntityTerrainCollisions()
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

// TODO: Call with each server tick, post-creation of Entity raycasting (packet-loss batch processing)
void FPS_Game::checkEntityTerrainCollisions() {
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
}

// TODO: Called with each call of UpdatePlayer, between each input of a batch (packet-loss batch processing)
void FPS_Game::checkEntityPlayerCollisions() {
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
}

// the newly populated deltas CircularBuffer is added to from least recent to most recent (with peak returning the most recent)
void FPS_Game::calculateDeltas() {

    FPSClientState delta = FPSClientState();
    for(auto& playerPair: playerStates){
        size_t playerIndex = playerPair.first;
        auto& deltas = deltaStates.at(playerIndex);
        auto& states = playerPair.second;
        if(states.getCount() >= 2){
            auto& newerState = states.peekAtIndex(0);
            auto& olderState = states.peekAtIndex(1);

            // do all of the delta calculations for all entities client state
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

            // append to this delta to the deltas circular buffer
            // fully calculated the delta for a client, now appended from oldest -> newest to the player's delta CircularBuffer
             deltas.push(std::make_unique<FPSClientState>(std::move(delta)));
             delta = FPSClientState(); // reset to skeleton for the next set of newer and older states
        }
    }
}

void FPS_Game::buildServerFlatBuffer(flatbuffers::FlatBufferBuilder &builder,
                                     flatbuffers::Offset<SourcePoint> sourcePoint,
                                     flatbuffers::Offset<DestPoint> destPoint, int clientID, bool reliable,
                                     int serverTick, float serverDT, int wrapAround, int lobbyNumber,
                                     PacketType packetType, bool delta) {
    auto& map = delta ? deltaStates : playerStates;
    std::vector<flatbuffers::Offset<States>> playerStatesOffsets;

    for(auto& playerPair: map){
        size_t playerID = playerPair.first;
        auto& states = playerPair.second;
        std::vector<flatbuffers::Offset<Client>> clientOffsets;
        for(int i = 0; i < states.getCount(); i++){
            auto& state = states.peekAtIndex(i);

            const auto& entities = state->entities;
            std::vector<flatbuffers::Offset<Entity>> entityOffsets;
            for(int j = 0; j < entities.size(); j++) {
                auto entityLabel = builder.CreateString("<blank Entity label>");

                flatbuffers::Offset<OD_Vector3> entityPosition = CreateOD_Vector3(builder, entities[j].position.x, entities[j].position.y, entities[j].position.z);
                if (vector3IsZero(Vector3(entities[j].position)) and delta){
                    entityPosition = 0;
                }

                flatbuffers::Offset<OD_Vector3> entityVelocity = CreateOD_Vector3(builder, entities[j].velocity.x, entities[j].velocity.y, entities[j].velocity.z);
                if (vector3IsZero(Vector3(entities[j].velocity)) and delta){
                    entityVelocity = 0;
                }

                auto entity = CreateEntity(builder,
                                           j,
                                           entityLabel,
                                           entityPosition,
                                           entityVelocity,
                                           entities[j].alive
                );
                entityOffsets.push_back(entity);
            }
            auto entitiesVector = builder.CreateVector(entityOffsets);

            //TODO: take the client tick, normalize to server tick and store in FPSClientState on Client input (UpdatePlayer)
            flatbuffers::Offset<Tick> clientTick = CreateTick(builder, state->tick, state->dt, wrapAround);

            flatbuffers::Offset<OD_Vector3> cameraPosition = CreateOD_Vector3(builder, state->camera.position.x,state->camera.position.y, state->camera.position.z);
            if(vector3IsZero(state->camera.position) and delta){
                cameraPosition = 0;
            }

            flatbuffers::Offset<OD_Vector3> cameraTarget = CreateOD_Vector3(builder, state->camera.target.x, state->camera.target.y, state->camera.target.z);
            if(vector3IsZero(state->camera.target) and delta){
                cameraTarget = 0;
            }

            flatbuffers::Offset<OD_Vector3> cameraUp = CreateOD_Vector3(builder, state->camera.up.x, state->camera.up.y, state->camera.up.z);
            if(vector3IsZero(state->camera.up) and delta){
                cameraUp = 0;
            }

            auto camera = CreateOD_Camera3D(
                    builder,
                    cameraPosition,
                    cameraTarget,
                    cameraUp,
                    state->camera.fovy,
                    state->camera.projection
            );

            flatbuffers::Offset<OD_Vector3> position = CreateOD_Vector3(builder, state->position.x, state->position.y, state->position.z);
            if(vector3IsZero(state->position) and delta){
                position = 0;
            }

            flatbuffers::Offset<OD_Vector3> velocity = CreateOD_Vector3(builder, state->velocity.x, state->velocity.y, state->velocity.z);
            if(vector3IsZero(state->velocity) and delta){
                velocity = 0;
            }

            float snapshotRTT = delta ? NULL : 0;

            auto client = CreateClient(builder,
                                       NULL,
                                       clientTick,
                                       snapshotRTT,
                                       i,
                                       state->alive,
                                       state->sprint,
                                       camera,
                                       state->grounded,
                                       state->coolDown,
                                       position,
                                       velocity,
                                       entitiesVector
            );

            clientOffsets.push_back(client);
        }

        // TODO: the list of states for a player has been generated but the list of lists needs to be generated now
        //  specifically, building the "Differentials" or the "Snapshot" table
        auto clientStatesVector = builder.CreateVector(clientOffsets);

        float deltaRTT = delta ? 0 : NULL;
        auto clientStatesOffset = CreateClientStates(builder, deltaRTT, clientStatesVector);

        auto statesOffset = CreateStates(builder, StatesOptions_ClientStates, clientStatesOffset.Union());
        playerStatesOffsets.push_back(statesOffset);
    }

    flatbuffers::Offset<Tick> tick = CreateTick(builder, serverTick, serverDT);
    auto playerStatesVector = builder.CreateVector(playerStatesOffsets);

    auto payload = delta ?
                                CreatePayload(builder, PayloadTypes_AllPlayerDeltas, playerStatesVector.Union())
                                : CreatePayload(builder, PayloadTypes_AllPlayerSnapshots, playerStatesVector.Union());

    OD_PacketBuilder packetBuilder(builder);
    packetBuilder.add_packet_type(packetType);
    packetBuilder.add_dest_point(destPoint);
    packetBuilder.add_client_id(clientID);
    packetBuilder.add_source_point(sourcePoint);
    packetBuilder.add_lobby_number(lobbyNumber);
    packetBuilder.add_reliable(reliable);
    packetBuilder.add_tick(tick);
    packetBuilder.add_payload(payload);

    auto packet = packetBuilder.Finish();
    builder.Finish(packet);
}

std::vector<const Input*> FPS_Game::parseInputPackets(int playerIndex, const flatbuffers::Vector<flatbuffers::Offset<Input>> * clientInputs) {
    std::vector<const Input*> inputsToProcess;
    auto iter = playerStates.find(playerIndex);

    if(iter == playerStates.end() || iter->second.isEmpty()){
        fprintf(stderr, "Error: retrieving most Current State for player ID # %d. playerID not found in playerStates map || Client State's map is empty.\n\tFPS_Game.cpp ~ getMostPlayerMostCurrentState()", playerIndex);
        return inputsToProcess; // Return empty vector in case of error
    }

    int mostRecentServerTick = iter->second.peek()->tick;

    // map most recent server tick to client tick
    int mostRecentClientTick = serverToClientTick[playerIndex][mostRecentServerTick];
    int clientInputHeadTick = clientInputs->Get(0)->tick()->tick_number();

    // If the head is less than the last verified tick, then there was out-of-order delivery. So ignore, and head will be ignored in update
    if(clientInputHeadTick <= mostRecentClientTick){
        return inputsToProcess;
    }
    // If the head of inputs is consecutive with the last verified tick, we're synchronized. So send consecutive tick's packet to be processed
    else if(clientInputHeadTick == mostRecentClientTick + 1) {
        inputsToProcess.push_back(clientInputs->Get(0));
        return inputsToProcess;
    }
    else if(clientInputHeadTick > mostRecentClientTick + 1 ){ // packet loss has occured, and intermediate packets must be pulled from the buffer
        for(auto inputPacket: *clientInputs){
            int tickNum = inputPacket->tick()->tick_number();
            if(tickNum <= mostRecentClientTick + 1){
                cout << "Packet loss w/ Client #" << playerIndex << " has occured. Inputs of ticks [" << tickNum << "," << clientInputHeadTick <<  "] pulled from Input history.";
                break;
            }
            // add this packet to our list to process
            inputsToProcess.push_back(inputPacket);
        }
        // Reverse the vector to get oldest -> newest input
        std::reverse(inputsToProcess.begin(), inputsToProcess.end());
    }
    return inputsToProcess;
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

const vector<BoundingBox> &FPS_Game::getTerrainVector() {
    return terrainVector;
}

const vector<BoundingBox> &FPS_Game::getTopBoxVector() {
    return topBoxVector;
}