//
// Created by Anshul Gowda on 3/3/24.
//

#include "GameLobby.h"

GameLobby::GameLobby(PartitionedPacketBuffer &receiveBuffer, PacketBuffer &outputBuffer) : LobbyServices(receiveBuffer, outputBuffer) {
    // Create a receive buffer partition
    auto index = LobbyServices.createReceiveBufferPartition();

    if(index.has_value()){
        this->partitionIndex = index.value();
    }
    else{
        cerr << "Failed to allocate a receive buffer partition for this game lobby" << endl;
    }
}

void GameLobby::start() {
    stopFlag.store(false);
    workerThread = thread(&GameLobby::run, this);

}

void GameLobby::stop() {
    stopFlag.store(true);
    LobbyServices.notifyAllOnPartition(partitionIndex);

    if(workerThread.joinable()){
        workerThread.join();
    }
}

void GameLobby::run() {
    auto lastTick = std::chrono::steady_clock::now();
    const auto secondsPerTick = std::chrono::duration<float>(1.0f / tickRate);

//    auto nextTick = lastTick + secondPerTick;

    while(!stopFlag.load()){
        auto nextTick = lastTick + secondsPerTick;

        while(std::chrono::steady_clock::now() < nextTick){
            auto packet = LobbyServices.popFromReceiveBufferParition(partitionIndex);
            if(packet){
                processPacket(std::move(packet)); // TODO: this will update the global game state
            } else{
                break; // no more packets left
            }
        }

        sendSnapShot();
        tickNumber++;

        auto now = std::chrono::steady_clock::now();
        auto timeRemaining = nextTick - now;
        if ( timeRemaining > std::chrono::milliseconds(0)){
            std::this_thread::sleep_for(timeRemaining);
        }
    }



}

void GameLobby::processPacket(unique_ptr<Packet> packet) {
    printf("Game Lobby received packet of size %zu and data = \"%s\"\n\n", packet->packet->dataLength, packet->packet->data);

    // TODO: Enter a decision tree determining what authoritative decision is required
}

void GameLobby::update() {

}