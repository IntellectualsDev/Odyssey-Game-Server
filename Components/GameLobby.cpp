//
// Created by Anshul Gowda on 3/3/24.
//

#include "GameLobby.h"
#include "../game_state_generated.h"

//GameLobby::GameLobby(PartitionedPacketBuffer &receiveBuffer, PacketBuffer &outputBuffer, mutex& consoleMutex) : LobbyServices(receiveBuffer, outputBuffer), consoleMutex(consoleMutex) {
GameLobby::GameLobby(PartitionedPacketBuffer* receiveBuffer, PacketBuffer* outputBuffer, std::mutex &consoleMutex, bool showGUI): receiveBuffer(receiveBuffer), outputBuffer(outputBuffer), consoleMutex(consoleMutex), game(), showGUI(showGUI){
    // Create a receive buffer partition
    if(showGUI){
        InitWindow(0, 0, "Shooter Game");
        screenWidth = GetMonitorWidth(0);
        screenHeight = GetMonitorHeight(0);
        CloseWindow();
        InitWindow(screenWidth-400, screenHeight-400, "Shooter Game");
        //set fps
        SetTargetFPS(60);
        //disable cursor
        DisableCursor();
    }

    auto index = receiveBuffer->allocatePartition();

    if(index.has_value()){
        this->partitionIndex = index.value();
    }
    else{
        cerr << "Error GameLobby.cpp line 18:\n\tFailed to allocate a receive buffer partition for this game lobby" << endl;
    }

    // TODO: will be removed once, there is handshake and will be dynamically called
    // given the initial ID of 0
    game.createNewPlayer((Vector3){0,2,1},(Vector3){0,0,0}, 1.0f/60.0f);

}

void GameLobby::start() {
    stopFlag.store(false);
    workerThread = thread(&GameLobby::run, this);

}

void GameLobby::stop() {
    stopFlag.store(true);
    receiveBuffer->notifyAllOnPartition(partitionIndex);
//    LobbyServices.notifyAllOnPartition(partitionIndex);

    if(workerThread.joinable()){
        workerThread.join();
    }
}

void GameLobby::run() {
//    auto lastTick = std::chrono::steady_clock::now();
    const auto secondsPerTick = std::chrono::duration<float>(1.0f / tickRate);

    while(!stopFlag.load()){
        auto tickStartTime = std::chrono::steady_clock::now();
        auto nextTick = tickStartTime + secondsPerTick;
        {
            std::lock_guard<std::mutex> guard(consoleMutex);
//            cout << "Entering tick " << tickNumber << std::endl;
        }

        auto currentTickPackets = receiveBuffer->popAllFromPartition(partitionIndex);
        cout << "# of pulled packets: " << currentTickPackets.size() << endl;
        if(!currentTickPackets.empty()){
//            vector<unique_ptr<BufferHandler>> currentTickPackets = std::move(temp.value());
            for(int i = 0; i < currentTickPackets.size(); i++){
//                processPacket(std::move(currentTickPackets[i]));
                update(std::move(currentTickPackets[i]));
            }
        }


//        sendSnapShot();
//        render();


        auto now = std::chrono::steady_clock::now();
        auto timeRemaining = nextTick - now;
        {
            std::lock_guard<std::mutex> guard(consoleMutex);
            cout << "Remaining time in tick: " << tickNumber << ":\n\t"
                 << std::chrono::duration_cast<std::chrono::milliseconds>(nextTick - now).count() << " ms" << endl;
        }




        //TODO: Wraparound should be defined as the client with the highest RTT. Currently statically defined
        tickNumber = (tickNumber+1) % 60;
        if ( timeRemaining > std::chrono::milliseconds(0)){
            std::this_thread::sleep_for(timeRemaining);
        }

//        while(std::chrono::steady_clock::now() < nextTick){
//            auto packet = LobbyServices.popFromReceiveBufferParition(partitionIndex);
//            if(packet){
//                {
//                    std::lock_guard<std::mutex> guard(consoleMutex);
//                    std::cout << "Processing " << packet->label << " in tick " << tickNumber << std::endl;
//                }
////                processPacket(std::move(packet)); // TODO: this will update the global game state
//            } else{
//                continue; // no more packets left
//            }
//        }
//
//
//        auto now = std::chrono::steady_clock::now();
//        auto timeRemaining = nextTick - now;
//        {
//            std::lock_guard<std::mutex> guard(consoleMutex);
//            cout << "Remaining time in tick: " << tickNumber << ":\n\t"
//                 << std::chrono::duration_cast<std::chrono::milliseconds>(nextTick - now).count() << " ms" << endl;
//        }
//        sendSnapShot();
//        tickNumber++;
//        if ( timeRemaining > std::chrono::milliseconds(0)){
//            std::this_thread::sleep_for(timeRemaining);
//        }

    }
    CloseWindow();
}

void GameLobby::processPacket(unique_ptr<BufferHandler> packet) {
//    {
//        std::lock_guard<std::mutex> guard(consoleMutex);
//        cout << "Packet Label: " << EnumNamePacketType(packet->getPacketView()->packet_type()) << endl;
//        cout << "Source Point: (IP: " << packet->getPacketView()->source_point()->address() << ", Port: " << packet->getPacketView()->source_point()->port() << ") " << endl;
//        cout << "Destination Point: (IP: " << packet->getPacketView()->dest_point()->address() << ", Port: " << packet->getPacketView()->dest_point()->port() << ") " << endl;
//        cout << "Lobby Number: " << packet->getPacketView()->lobby_number() << endl;
//        cout << "Client Tick Number: " << packet->getPacketView()->tick()->tick_number() << endl;
//        cout << "Payload: \n\t: ";
//        cout << "Payload Type: " << EnumNamePacketPayload(packet->getPacketView()->payload_type()) << endl;
//        cout << "\tdt: " << packet->getPacketView()->tick()->dt() << endl;
//        cout << "\tW: " << packet->getPacketView()->payload_as_Input()->w() << ", A: " << packet->getPacketView()->payload_as_Input()->a() << " S: " << packet->getPacketView()->payload_as_Input()->s() << " D: " << packet->getPacketView()->payload_as_Input()->d() << endl;
//        cout << "\tMouseDelta: (" << packet->getPacketView()->payload_as_Input()->mouse_delta()->x() << ", " << packet->getPacketView()->payload_as_Input()->mouse_delta()->y() << ")" << endl;
//        cout << "\tShoot: " << to_string(packet->getPacketView()->payload_as_Input()->shoot()) << ", Jump: " << to_string(packet->getPacketView()->payload_as_Input()->space()) << ", Sprint: " << to_string(packet->getPacketView()->payload_as_Input()->sprint()) << endl;
//        cout << "\tPrevious State: (" << packet->getPacketView()->payload_as_Input()->previous_position()->x() << ", " << packet->getPacketView()->payload_as_Input()->previous_position()->y() << ", " << packet->getPacketView()->payload_as_Input()->previous_position()->z() << ")\n" << endl;
//    }

    // TODO: Enter a decision tree determining what authoritative decision is required
}

void GameLobby::update(unique_ptr<BufferHandler> packet) {
//    {
//        std::lock_guard<std::mutex> guard(consoleMutex);
//        cout << "Packet Label: " << EnumNamePacketType(packet->getPacketView()->packet_type()) << endl;
//        cout << "Source Point: (IP: " << packet->getPacketView()->source_point()->address() << ", Port: " << packet->getPacketView()->source_point()->port() << ") " << endl;
//        cout << "Destination Point: (IP: " << packet->getPacketView()->dest_point()->address() << ", Port: " << packet->getPacketView()->dest_point()->port() << ") " << endl;
//        cout << "Lobby Number: " << packet->getPacketView()->lobby_number() << endl;
//        cout << "Client Tick Number: " << packet->getPacketView()->tick()->tick_number() << endl;
//        cout << "Payload: \n\t: ";
//        cout << "Payload Type: " << EnumNamePacketPayload(packet->getPacketView()->payload_type()) << endl;
//        cout << "\tdt: " << packet->getPacketView()->tick()->dt() << endl;
//        cout << "\tW: " << packet->getPacketView()->payload_as_Input()->w() << ", A: " << packet->getPacketView()->payload_as_Input()->a() << " S: " << packet->getPacketView()->payload_as_Input()->s() << " D: " << packet->getPacketView()->payload_as_Input()->d() << endl;
//        cout << "\tMouseDelta: (" << packet->getPacketView()->payload_as_Input()->mouse_delta()->x() << ", " << packet->getPacketView()->payload_as_Input()->mouse_delta()->y() << ")" << endl;
//        cout << "\tShoot: " << to_string(packet->getPacketView()->payload_as_Input()->shoot()) << ", Jump: " << to_string(packet->getPacketView()->payload_as_Input()->space()) << ", Sprint: " << to_string(packet->getPacketView()->payload_as_Input()->sprint()) << endl;
//        cout << "\tPrevious State: (" << packet->getPacketView()->payload_as_Input()->previous_position()->x() << ", " << packet->getPacketView()->payload_as_Input()->previous_position()->y() << ", " << packet->getPacketView()->payload_as_Input()->previous_position()->z() << ")\n" << endl;
//    }

    // 1. updatePlayer
    // 2. updateEntities
    // 3. checkEntityCollisions()
    // 4. Optional: display Server GUI
    game.updatePlayer(0,
                      packet->getPacketView()->payload_as_Input()->w(),
                      packet->getPacketView()->payload_as_Input()->a(),
                      packet->getPacketView()->payload_as_Input()->s(),
                      packet->getPacketView()->payload_as_Input()->d(),
                      (Vector2){packet->getPacketView()->payload_as_Input()->mouse_delta()->x(), packet->getPacketView()->payload_as_Input()->mouse_delta()->y()},
                      packet->getPacketView()->payload_as_Input()->shoot(),
                      packet->getPacketView()->payload_as_Input()->space(),
                      packet->getPacketView()->tick()->dt(),
                      packet->getPacketView()->payload_as_Input()->sprint(),
                      false);



    game.updateEntities();

    // TODO: both of these lead to SIGSEGV faults
//    game.checkEntityCollisions();
    game.calculateDeltas(); // TODO: double check: the difference between the current and previous states (Current - Previous)

    return;
}

void GameLobby::render() {
    if(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(RAYWHITE);

        FPSClientState* player = game.getPlayerCurrentState(0);
        vector<FPSClientState*> allPlayers = game.getCurrentStatesOfPlayers();

        BeginMode3D(player->camera); //TODO: choosing to be Player 0 to be FPOV, do eagle eye POV

        //render ther full terrain
        for(int i = 0; i < FPS_Game::getTerrainVector().size();i++){
            DrawBoundingBox(FPS_Game::getTerrainVector()[i],BLACK);
        }
        for(int i = 0; i < FPS_Game::getTopBoxVector().size();i++){
            DrawBoundingBox(FPS_Game::getTopBoxVector()[i],RED);
        }

        // render all of the players' boxes
        for(int i = 0; i < allPlayers.size(); i++){
            DrawBoundingBox(allPlayers[i]->playerBox,PINK);
        };

        // render the environment
        DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 50.0f, 50.0f }, LIGHTGRAY); // Draw ground
        DrawCube((Vector3){ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);     // Draw a blue wall
        DrawCube((Vector3){ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);      // Draw a green wall
        DrawCube((Vector3){ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);      // Draw a yellow wall

        // render all of the entities:
        for(int i = 0; i < allPlayers.size(); i++){
            for(int j = 0; j < allPlayers[i]->entities.size(); j++){
                DrawCube(allPlayers[i]->entities[j].position,
                         allPlayers[i]->entities[j].hitbox.x,
                         allPlayers[i]->entities[j].hitbox.y,
                         allPlayers[i]->entities[j].hitbox.z,
                         RED);
                DrawBoundingBox(allPlayers[i]->entities[j].bulletBox, BLACK);
            }
        }
        EndMode3D();
        EndDrawing();

    }
}

void GameLobby::sendSnapShot() {

    game.updatePreviousStates();
    //TODO: Assign the Previous State to Current State, and Current State will automatically be overwritten next tick
    return;
}

size_t GameLobby::getPartitionIndex() const {
    return partitionIndex;
}
