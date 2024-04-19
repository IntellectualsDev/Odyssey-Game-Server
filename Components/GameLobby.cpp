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
    connectionManager->addPlayer(0, IPEndpoint({"Enter Joseph IP", -99}));
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
            for(int i = 0; i < currentTickPackets.size(); i++){
                cout << "Before update method in game lobby.cpp" << endl;

                packetPreprocessing(*currentTickPackets[i].get());
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

void GameLobby::processPacket(unique_ptr<BufferHandler> packet) {
    {
        std::lock_guard<std::mutex> guard(consoleMutex);
        cout << "Packet Label: " << EnumNamePacketType(packet->getPacketView()->packet_type()) << endl;
        cout << "Source Point: (IP: " << packet->getPacketView()->source_point()->address() << ", Port: " << packet->getPacketView()->source_point()->port() << ") " << endl;
        cout << "Destination Point: (IP: " << packet->getPacketView()->dest_point()->address() << ", Port: " << packet->getPacketView()->dest_point()->port() << ") " << endl;
        cout << "Lobby Number: " << packet->getPacketView()->lobby_number() << endl;
        cout << "Client Tick Number: " << packet->getPacketView()->tick()->tick_number() << endl;
        cout << "Payload: \n\t: ";
        cout << "Payload Type: " << EnumNamePayloadTypes(packet->getPacketView()->payload()->payload_type()) << endl;
        cout << "\tdt: " << packet->getPacketView()->tick()->dt() << endl;
        cout << "\tW: " << packet->getPacketView()->payload()->payload_as_Input()->w() << ", A: " << packet->getPacketView()->payload()->payload_as_Input()->a() << " S: " << packet->getPacketView()->payload()->payload_as_Input()->s() << " D: " << packet->getPacketView()->payload()->payload_as_Input()->d() << endl;
        cout << "\tMouseDelta: (" << packet->getPacketView()->payload()->payload_as_Input()->mouse_delta()->x() << ", " << packet->getPacketView()->payload()->payload_as_Input()->mouse_delta()->y() << ")" << endl;
        cout << "\tShoot: " << to_string(packet->getPacketView()->payload()->payload_as_Input()->shoot()) << ", Jump: " << to_string(packet->getPacketView()->payload()->payload_as_Input()->space()) << ", Sprint: " << to_string(packet->getPacketView()->payload()->payload_as_Input()->sprint()) << endl;
        cout << "\tPrevious State: (" << packet->getPacketView()->payload()->payload_as_Input()->previous_position()->x() << ", " << packet->getPacketView()->payload()->payload_as_Input()->previous_position()->y() << ", " << packet->getPacketView()->payload()->payload_as_Input()->previous_position()->z() << ")\n" << endl;
    }

    // TODO: Enter a decision tree determining what authoritative decision is required
}

void GameLobby::packetPreprocessing(BufferHandler &packet) {

}

void GameLobby::update(unique_ptr<BufferHandler> packet) {
    {
        std::lock_guard<std::mutex> guard(consoleMutex);
        cout << "Packet Label: " << EnumNamePacketType(packet->getPacketView()->packet_type()) << endl;
        cout << "Source Point: (IP: " << packet->getPacketView()->source_point()->address() << ", Port: " << packet->getPacketView()->source_point()->port() << ") " << endl;
        cout << "Destination Point: (IP: " << packet->getPacketView()->dest_point()->address() << ", Port: " << packet->getPacketView()->dest_point()->port() << ") " << endl;
        cout << "Lobby Number: " << packet->getPacketView()->lobby_number() << endl;
        cout << "Client Tick Number: " << packet->getPacketView()->tick()->tick_number() << endl;
        cout << "Payload: \n\t: ";
        cout << "Payload Type: " << EnumNamePayloadTypes(packet->getPacketView()->payload()->payload_type()) << endl;
        cout << "\tdt: " << packet->getPacketView()->tick()->dt() << endl;
        cout << "\tW: " << packet->getPacketView()->payload()->payload_as_Input()->w() << ", A: " << packet->getPacketView()->payload()->payload_as_Input()->a() << " S: " << packet->getPacketView()->payload()->payload_as_Input()->s() << " D: " << packet->getPacketView()->payload()->payload_as_Input()->d() << endl;
        cout << "\tMouseDelta: (" << packet->getPacketView()->payload()->payload_as_Input()->mouse_delta()->x() << ", " << packet->getPacketView()->payload()->payload_as_Input()->mouse_delta()->y() << ")" << endl;
        cout << "\tShoot: " << to_string(packet->getPacketView()->payload()->payload_as_Input()->shoot()) << ", Jump: " << to_string(packet->getPacketView()->payload()->payload_as_Input()->space()) << ", Sprint: " << to_string(packet->getPacketView()->payload()->payload_as_Input()->sprint()) << endl;
        cout << "\tPrevious State: (" << packet->getPacketView()->payload()->payload_as_Input()->previous_position()->x() << ", " << packet->getPacketView()->payload()->payload_as_Input()->previous_position()->y() << ", " << packet->getPacketView()->payload()->payload_as_Input()->previous_position()->z() << ")\n" << endl;
    }

    // Pre-process for packet loss/re-ordered packets, and generate packets that must be simulated and simulate current and any past missed inputs
    vector<const Input *>  toProcess = game.parseInputPackets(packet->getPacketView()->client_id(),
                                            packet->getPacketView()->payload()->payload_as_ClientInputs()->client_inputs());

    game.mapDesyncClientandServerTicks(packet.getPacketView()->client_id(),
                                       tickNumber,
                                       packet.getPacketView()->tick()->tick_number());

    for(const Input * input : toProcess){
        game.updatePlayer(input->client_uid(),
                          input->w(),
                          input->a(),
                          input->s(),
                          input->d(),
                          (Vector2){input->mouse_delta()->x(), input->mouse_delta()->y()},
                          input->shoot(),
                          input->space(),
                          input->tick()->dt(),
                          input->sprint(),
                          false,
                          GameLobby::tickRate);

        game.updateEntities();
        game.checkEntityCollisions();
        game.calculateDeltas();
    }

    // 1. Pre-process packets for packet loss/re-ordered packets & generate list of inputs to be simulated
    // 1. mapAsyncClientTick { client(i) -> server  & server -> client(i) }
    // 2. updatePlayer
    // 3. updateEntities
    // 4. checkEntityCollisions()
    // 5. Optional: display Server GUI

    game.updatePlayer(0,
                      packet->getPacketView()->payload()->payload_as_Input()->w(),
                      packet->getPacketView()->payload()->payload_as_Input()->a(),
                      packet->getPacketView()->payload()->payload_as_Input()->s(),
                      packet->getPacketView()->payload()->payload_as_Input()->d(),
                      (Vector2){packet->getPacketView()->payload()->payload_as_Input()->mouse_delta()->x(), packet->getPacketView()->payload()->payload_as_Input()->mouse_delta()->y()},
                      packet->getPacketView()->payload()->payload_as_Input()->shoot(),
                      packet->getPacketView()->payload()->payload_as_Input()->space(),
                      packet->getPacketView()->tick()->dt(),
                      packet->getPacketView()->payload()->payload_as_Input()->sprint(),
                      false,
                      GameLobby::tickRate);

    game.updateEntities();
    game.checkEntityCollisions();
    game.calculateDeltas();

    // TODO: both of these lead to SIGSEGV faults

    // TODO: double check: the difference between the current and previous states (Current - Previous)

    return;
}

void GameLobby::sendSnapShot() {
    for(auto& player: connectionManager->getPlayerEndpoints()){
        flatbuffers::FlatBufferBuilder builder(1024);

        auto srcpoint = CreateSourcePoint(builder, builder.CreateString("<Dynamically Insert Server's IP Here>"), -99);
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

        //TODO: evaluate the performance of this. Could be more efficient to bypass the BufferHandler, due to a redundant memcpy
        std::unique_ptr<uint8_t[]> bufferCopy(new uint8_t[size]);
        memcpy(bufferCopy.get(), buffer, size);

        unique_ptr<BufferHandler> outputBufferHandler = std::make_unique<BufferHandler>(std::move(bufferCopy), builder.GetSize());
        outputBuffer->addPacket(std::move(outputBufferHandler));
    }
}

void GameLobby::sendDifferentials() {
    for(auto& player: connectionManager->getPlayerEndpoints()){
        flatbuffers::FlatBufferBuilder builder(1024);

        auto srcpoint = CreateSourcePoint(builder, builder.CreateString("<Dynamically Insert Server's IP Here>"), -99);
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

        //TODO: evaluate the performance of this. Could be more efficient to bypass the BufferHandler, due to a redundant memcpy
        std::unique_ptr<uint8_t[]> bufferCopy(new uint8_t[size]);
        memcpy(bufferCopy.get(), buffer, size);

        unique_ptr<BufferHandler> outputBufferHandler = std::make_unique<BufferHandler>(std::move(bufferCopy), builder.GetSize());
        outputBuffer->addPacket(std::move(outputBufferHandler));
    }
}

size_t GameLobby::getPartitionIndex() const {
    return partitionIndex;
}
