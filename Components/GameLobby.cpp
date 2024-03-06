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
    while(!stopFlag.load()){
        auto packet = LobbyServices.popFromReceiveBufferParition(partitionIndex);
        if(packet){
            processPacket(std::move(packet));
        }
    }
}

void GameLobby::processPacket(unique_ptr<Packet> packet) {
    printf("Game Lobby received packet of size %zu and data = \"%s\"\n\n", packet->packet->dataLength, packet->packet->data);

    // TODO: Enter a decision tree determining what authoritative decision is required
}