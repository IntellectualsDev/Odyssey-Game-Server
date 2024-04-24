//
// Created by Anshul Gowda on 3/3/24.
//

#include "GameLobby.h"
#include "../game_state_generated.h"

//GameLobby::GameLobby(PartitionedPacketBuffer &receiveBuffer, PacketBuffer &outputBuffer, mutex& consoleMutex) : LobbyServices(receiveBuffer, outputBuffer), consoleMutex(consoleMutex) {
GameLobby::GameLobby(PartitionedPacketBuffer* receiveBuffer, PacketBuffer* outputBuffer, ConnectionManager* connectionManager, std::mutex &consoleMutex, bool showGUI):
receiveBuffer(receiveBuffer),
outputBuffer(outputBuffer),
consoleMutex(consoleMutex),
connectionManager(connectionManager),
game(),
showGUI(showGUI){

    // Create a receive buffer partition
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
    connectionManager->addPlayer(0, IPEndpoint({"192.168.56.1", 5453}));
}

void GameLobby::start() {
    stopFlag.store(false);
    workerThread = thread(&GameLobby::run, this);

}

void GameLobby::stop() {
    stopFlag.store(true);
    receiveBuffer->notifyAllOnPartition(partitionIndex);

    if(workerThread.joinable()){
        workerThread.join();
    }
    CloseWindow();
}

void GameLobby::run() {
    const auto secondsPerTick = std::chrono::duration<float>(1.0f / tickRate);

    while(!stopFlag.load()){
        auto tickStartTime = std::chrono::steady_clock::now();
        auto nextTick = tickStartTime + secondsPerTick;
        {
            std::lock_guard<std::mutex> guard(consoleMutex);
//            cout << "Entering tick " << tickNumber << std::endl;
        }

        // TODO:  severTick init sequence:
        //  (1) updateEntities(), updates position of entities & generates raycasts to be used in checkEntityCollisions
        serverTickInit();

        auto currentTickPackets = receiveBuffer->popAllFromPartition(partitionIndex);
        cout << "# of pulled packets: " << currentTickPackets.size() << endl;
        if(!currentTickPackets.empty()){
            for(int i = 0; i < currentTickPackets.size(); i++){
                cout << "Before update method in game lobby.cpp" << endl;

//                packetPreprocessing(*currentTickPackets[i].get());
                update(std::move(currentTickPackets[i]));
            }

            // TESTING
            sendSnapShot();
            sendDifferentials();
//            if(tickNumber % ticksTillGlobalState == 0){ // every 20 differentials send a global state
//                sendSnapShot();
//            }
//            else{
//                sendDifferentials();
//            }
        }
        // TODO

        auto now = std::chrono::steady_clock::now();
        auto timeRemaining = nextTick - now;
        {
            std::lock_guard<std::mutex> guard(consoleMutex);
            cout << "Remaining time in tick: " << tickNumber << ":\n\t"
                 << std::chrono::duration_cast<std::chrono::milliseconds>(nextTick - now).count() << " ms" << endl;
        }

        //TODO: Wraparound should be defined as the client with the highest RTT. Currently statically defined
        tickNumber = (tickNumber+1) % wrapAround;
        if ( timeRemaining > std::chrono::milliseconds(0)){
            std::this_thread::sleep_for(timeRemaining);
        }

    }
}

void GameLobby::serverTickInit() {
    game.updateEntities(GameLobby::tickRate);
}

void GameLobby::processPacket(unique_ptr<BufferHandler> packet) {
    {
//        std::lock_guard<std::mutex> guard(consoleMutex);
//        cout << "Packet Label: " << EnumNamePacketType(packet->getPacketView()->packet_type()) << endl;
//        cout << "Source Point: (IP: " << packet->getPacketView()->source_point()->address() << ", Port: " << packet->getPacketView()->source_point()->port() << ") " << endl;
//        cout << "Destination Point: (IP: " << packet->getPacketView()->dest_point()->address() << ", Port: " << packet->getPacketView()->dest_point()->port() << ") " << endl;
//        cout << "Lobby Number: " << packet->getPacketView()->lobby_number() << endl;
//        cout << "Client Tick Number: " << packet->getPacketView()->client_tick()->tick_number() << endl;
//        cout << "Payload: \n\t: ";
//        cout << "Payload Type: " << EnumNamePayloadTypes(packet->getPacketView()->payload()->payload_type()) << endl;
//        cout << "\tdt: " << packet->getPacketView()->client_tick()->dt() << endl;
//        cout << "\tW: " << packet->getPacketView()->payload()->payload_as_Input()->w() << ", A: " << packet->getPacketView()->payload()->payload_as_Input()->a() << " S: " << packet->getPacketView()->payload()->payload_as_Input()->s() << " D: " << packet->getPacketView()->payload()->payload_as_Input()->d() << endl;
//        cout << "\tMouseDelta: (" << packet->getPacketView()->payload()->payload_as_Input()->mouse_delta()->x() << ", " << packet->getPacketView()->payload()->payload_as_Input()->mouse_delta()->y() << ")" << endl;
//        cout << "\tShoot: " << to_string(packet->getPacketView()->payload()->payload_as_Input()->shoot()) << ", Jump: " << to_string(packet->getPacketView()->payload()->payload_as_Input()->space()) << ", Sprint: " << to_string(packet->getPacketView()->payload()->payload_as_Input()->sprint()) << endl;
//        cout << "\tPrevious State: (" << packet->getPacketView()->payload()->payload_as_Input()->previous_position()->x() << ", " << packet->getPacketView()->payload()->payload_as_Input()->previous_position()->y() << ", " << packet->getPacketView()->payload()->payload_as_Input()->previous_position()->z() << ")\n" << endl;
    }

    // TODO: Enter a decision tree determining what authoritative decision is required
}

void GameLobby::packetPreprocessing(BufferHandler &packet) {

}

void GameLobby::update(unique_ptr<ENetPacket> packet) {
//    {
//        std::lock_guard<std::mutex> guard(consoleMutex);
//        cout << "Packet Label: " << EnumNamePacketType(packet->getPacketView()->packet_type()) << endl;
//        cout << "Source Point: (IP: " << packet->getPacketView()->source_point()->address() << ", Port: " << packet->getPacketView()->source_point()->port() << ") " << endl;
//        cout << "Destination Point: (IP: " << packet->getPacketView()->dest_point()->address() << ", Port: " << packet->getPacketView()->dest_point()->port() << ") " << endl;
//        cout << "Lobby Number: " << packet->getPacketView()->lobby_number() << endl;
//        cout << "Client Tick Number: " << packet->getPacketView()->client_tick()->tick_number() << endl;
//        cout << "Payload: \n\t: ";
//        cout << "Payload Type: " << EnumNamePayloadTypes(packet->getPacketView()->payload()->payload_type()) << endl;
//        cout << "\tdt: " << packet->getPacketView()->client_tick()->dt() << endl;
//        cout << "\tW: " << packet->getPacketView()->payload()->payload_as_Input()->w() << ", A: " << packet->getPacketView()->payload()->payload_as_Input()->a() << " S: " << packet->getPacketView()->payload()->payload_as_Input()->s() << " D: " << packet->getPacketView()->payload()->payload_as_Input()->d() << endl;
//        cout << "\tMouseDelta: (" << packet->getPacketView()->payload()->payload_as_Input()->mouse_delta()->x() << ", " << packet->getPacketView()->payload()->payload_as_Input()->mouse_delta()->y() << ")" << endl;
//        cout << "\tShoot: " << to_string(packet->getPacketView()->payload()->payload_as_Input()->shoot()) << ", Jump: " << to_string(packet->getPacketView()->payload()->payload_as_Input()->space()) << ", Sprint: " << to_string(packet->getPacketView()->payload()->payload_as_Input()->sprint()) << endl;
//        cout << "\tPrevious State: (" << packet->getPacketView()->payload()->payload_as_Input()->previous_position()->x() << ", " << packet->getPacketView()->payload()->payload_as_Input()->previous_position()->y() << ", " << packet->getPacketView()->payload()->payload_as_Input()->previous_position()->z() << ")\n" << endl;
//    }

    const OD_Packet* od_Packet = flatbuffers::GetRoot<OD_Packet>(packet->data);

    // print out the contents of the head of the history input vector
    {
        std::lock_guard<std::mutex> guard(consoleMutex);
        cout << "Packet Label: " << EnumNamePacketType(od_Packet->packet_type()) << endl;
        cout << "Source Point: (IP: " << od_Packet->source_point()->address() << ", Port: " << od_Packet->source_point()->port() << ") " << endl;
        cout << "Destination Point: (IP: " << od_Packet->dest_point()->address() << ", Port: " << od_Packet->dest_point()->port() << ") " << endl;
        cout << "Lobby Number: " << od_Packet->lobby_number() << endl;
        cout << "Client Tick Number: " << od_Packet->client_tick()->tick_number() << endl;
        cout << "Payload: \n\t: ";
//        cout << "Payload Type: " << EnumNamePayloadTypes(packet->getPacketView()->payload()->payload_type()) << endl;
        for(auto value: *(od_Packet->payload()->payload_CI()->client_inputs())){
            cout << "\tW: " << value->w() << ", A: " << value->a() << " S: " << value->s() << " D: " << value->d() << endl;
            cout << "\tMouseDelta: (" << value->mouse_delta()->x() << ", " << value->mouse_delta()->y() << ")" << endl;
            cout << "\tShoot: " << to_string(value->shoot()) << ", Jump: " << to_string(value->space()) << ", Sprint: " << to_string(value->sprint()) << endl;
            cout << "\tPrevious State: (" << value->previous_position()->x() << ", " << value->previous_position()->y() << ", " << value->previous_position()->z() << ")\n" << endl;
        }
//        cout << "\tdt: " << packet->getPacketView()->client_tick()->dt() << endl;
//        cout << "\tW: " << packet->getPacketView()->payload()->payload_as_ClientInputs()->client_inputs()->Get(0)->w() << ", A: " << packet->getPacketView()->payload()->payload_as_ClientInputs()->client_inputs()->Get(0)->a() << " S: " << packet->getPacketView()->payload()->payload_as_ClientInputs()->client_inputs()->Get(0)->s() << " D: " << packet->getPacketView()->payload()->payload_as_ClientInputs()->client_inputs()->Get(0)->d() << endl;
//        cout << "\tMouseDelta: (" << packet->getPacketView()->payload()->payload_as_ClientInputs()->client_inputs()->Get(0)->mouse_delta()->x() << ", " << packet->getPacketView()->payload()->payload_as_ClientInputs()->client_inputs()->Get(0)->mouse_delta()->y() << ")" << endl;
//        cout << "\tShoot: " << to_string(packet->getPacketView()->payload()->payload_as_ClientInputs()->client_inputs()->Get(0)->shoot()) << ", Jump: " << to_string(packet->getPacketView()->payload()->payload_as_ClientInputs()->client_inputs()->Get(0)->space()) << ", Sprint: " << to_string(packet->getPacketView()->payload()->payload_as_ClientInputs()->client_inputs()->Get(0)->sprint()) << endl;
//        cout << "\tPrevious State: (" << packet->getPacketView()->payload()->payload_as_ClientInputs()->client_inputs()->Get(0)->previous_position()->x() << ", " << packet->getPacketView()->payload()->payload_as_ClientInputs()->client_inputs()->Get(0)->previous_position()->y() << ", " << packet->getPacketView()->payload()->payload_as_ClientInputs()->client_inputs()->Get(0)->previous_position()->z() << ")\n" << endl;
    }


    // Start of Server tick: draw Entity Raycasts (for use in Player v Entity Collisions)

    // 1. Parse input  packets for packet loss/re-ordered packets & generate list of inputs to be simulated
    // 1. mapAsyncClientTick { client(i) -> server  & server -> client(i) }
    // 2. updatePlayer (if unacked inputs exist simulate all and mark with current server tick)
    // 4. checkEntityCollisions() (check raycasts with each change in player states and apply lag compensation)
    // 5. Calculate Deltas (generate deltas between added state and old state for clients who sent inputs)

    // End of Server Tick: updateEntities (at the end of server tick move entities to end of raycast line)

    // Pre-process for packet loss/re-ordered packets, and generate packets that must be simulated and simulate current and any past missed inputs

//    vector<const Input *>  toProcess = game.parseInputPackets(packet->getPacketView()->client_id(),
//                                            packet->getPacketView()->payload()->payload_as_ClientInputs()->client_inputs());
    vector<const Input *>  toProcess = game.parseInputPackets(od_Packet->client_id(),
                                                              od_Packet->payload()->payload_CI()->client_inputs());

    game.mapDesyncClientandServerTicks(od_Packet->client_id(),
                                       tickNumber,
                                       od_Packet->client_tick()->tick_number());

    if(!toProcess.empty()){
        // simulate all to-be processed inputs and update state
        // update the buffering of opponents of each client
        game.updatePlayer(od_Packet->client_id(),
                          tickNumber,
                          toProcess,
                          GameLobby::tickRate);
    }

    game.checkEntityTerrainCollisions();
    game.checkEntityPlayerCollisions(); // TODO: modify to pull other player position via lag compensation and enable collision detection via raycasts
//    game.checkEntityCollisions();
    game.calculateDeltas();

//________________________OLD IMPLEMENTATION---No multi-input simulation or history packet retreival______________________________________________
////    // 1. Pre-process packets for packet loss/re-ordered packets & generate list of inputs to be simulated
////    // 1. mapAsyncClientTick { client(i) -> server  & server -> client(i) }
////    // 2. updatePlayer
////    // 3. updateEntities
////    // 4. checkEntityCollisions()
////    // 5. Optional: display Server GUI
////
//    game.updatePlayer(0,
//                      packet->getPacketView()->payload()->payload_as_Input()->w(),
//                      packet->getPacketView()->payload()->payload_as_Input()->a(),
//                      packet->getPacketView()->payload()->payload_as_Input()->s(),
//                      packet->getPacketView()->payload()->payload_as_Input()->d(),
//                      (Vector2){packet->getPacketView()->payload()->payload_as_Input()->mouse_delta()->x(), packet->getPacketView()->payload()->payload_as_Input()->mouse_delta()->y()},
//                      packet->getPacketView()->payload()->payload_as_Input()->shoot(),
//                      packet->getPacketView()->payload()->payload_as_Input()->space(),
//                      (1.0 / GameLobby::tickRate),
//                      packet->getPacketView()->payload()->payload_as_Input()->sprint(),
//                      false,
//                      GameLobby::tickRate);
//
//    game.updateEntities(GameLobby::tickRate);
//    game.checkEntityCollisions();
//    game.calculateDeltas();
    //___________________________________________________________________________________________________________________________________________

    return;
}

void GameLobby::sendSnapShot() {
    for(auto& player: connectionManager->getPlayerEndpoints()){
        flatbuffers::FlatBufferBuilder builder(1024);

        auto srcpoint = CreateSourcePoint(builder, builder.CreateString(GameLobby::serverIP), GameLobby::serverPort);
        auto destpoint = CreateDestPoint(builder, builder.CreateString(player.second.ip), player.second.port);


        //TODO: determine if snapshot packets should be sent reliably or is there a significant performance hit
        game.buildServerFlatBuffer(builder,
                                   srcpoint,
                                   destpoint,
                                   player.first,
                                   true,
                                   tickNumber,
                                   (1.0/tickRate),
                                   wrapAround,
                                   partitionIndex,
                                   PacketType_GlobalState,
                                   false);

        uint8_t* buffer = builder.GetBufferPointer();
        size_t size = builder.GetSize();
        {
            std::lock_guard<std::mutex> guard(consoleMutex);
            cout <<  "Snapshot Flatbuffer of size "<< size << " bytes was built." << endl;
        }
        const OD_Packet* od_Packet = flatbuffers::GetRoot<OD_Packet>(buffer);

        enet_uint32 flags = 0;
        if(od_Packet->reliable()){
            flags = ENET_PACKET_FLAG_RELIABLE;
        }

        ENetPacket* packetToSend = enet_packet_create(buffer, size, flags);
        unique_ptr<ENetPacket> finalPacket(packetToSend);
        outputBuffer->addPacket(std::move(finalPacket));


        // test that the vectors within the fsb are accesible
        auto odpacket = flatbuffers::GetRoot<OD_Packet>(buffer);
        size_t size_snapshots = odpacket->payload()->payload_PS()->payload()->size();
        auto itemRTT = odpacket->payload()->payload_PS()->payload()->Get(0)->state_as_ClientStates()->rtt();
        auto itemAlive = odpacket->payload()->payload_PS()->payload()->Get(0)->state_as_ClientStates()->client_states()->Get(0)->alive();

    }
}

void GameLobby::sendDifferentials() {
    for(auto& player: connectionManager->getPlayerEndpoints()){
        flatbuffers::FlatBufferBuilder builder(1024);

        auto srcpoint = CreateSourcePoint(builder, builder.CreateString(GameLobby::serverIP), GameLobby::serverPort);
        auto destpoint = CreateDestPoint(builder, builder.CreateString(player.second.ip), player.second.port);

        game.buildServerFlatBuffer(builder,
                                   srcpoint,
                                   destpoint,
                                   player.first,
                                   false,
                                   tickNumber,
                                   (1.0/tickRate),
                                   wrapAround,
                                   partitionIndex,
                                   PacketType_DifferentialState,
                                   true);

        uint8_t* buffer = builder.GetBufferPointer();
        size_t size = builder.GetSize();
        {
            std::lock_guard<std::mutex> guard(consoleMutex);
            cout <<  "Differential Flatbuffer of size "<< size << " bytes was built." << endl;
        }
        const OD_Packet* od_Packet = flatbuffers::GetRoot<OD_Packet>(buffer);

        enet_uint32 flags = 0;
        if(od_Packet->reliable()){
            flags = ENET_PACKET_FLAG_RELIABLE;
        }

        ENetPacket* packetToSend = enet_packet_create(buffer, size, flags);
        unique_ptr<ENetPacket> finalPacket(packetToSend);
        outputBuffer->addPacket(std::move(finalPacket));
    }
}

size_t GameLobby::getPartitionIndex() const {
    return partitionIndex;
}
