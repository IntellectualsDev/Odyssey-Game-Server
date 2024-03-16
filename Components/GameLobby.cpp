//
// Created by Anshul Gowda on 3/3/24.
//

#include "GameLobby.h"

GameLobby::GameLobby(PartitionedPacketBuffer &receiveBuffer, PacketBuffer &outputBuffer, mutex& consoleMutex) : LobbyServices(receiveBuffer, outputBuffer), consoleMutex(consoleMutex) {
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
//    auto lastTick = std::chrono::steady_clock::now();
    const auto secondsPerTick = std::chrono::duration<float>(1.0f / tickRate);

//    auto nextTick = lastTick + secondPerTick;

    while(!stopFlag.load()){
        auto tickStartTime = std::chrono::steady_clock::now();
        auto nextTick = tickStartTime + secondsPerTick;
        {
            std::lock_guard<std::mutex> guard(consoleMutex);
            cout << "Entering tick " << tickNumber << std::endl;
        }


        while(std::chrono::steady_clock::now() < nextTick){
            auto packet = LobbyServices.popFromReceiveBufferParition(partitionIndex);
            if(packet){
                {
                    std::lock_guard<std::mutex> guard(consoleMutex);
                    std::cout << "Processing " << packet->label << " in tick " << tickNumber << std::endl;
                }
//                processPacket(std::move(packet)); // TODO: this will update the global game state
            } else{
                break; // no more packets left
            }
        }


        auto now = std::chrono::steady_clock::now();
        auto timeRemaining = nextTick - now;
        {
            std::lock_guard<std::mutex> guard(consoleMutex);
            cout << "Remaining time in tick: " << tickNumber << ":\n\t"
                 << std::chrono::duration_cast<std::chrono::milliseconds>(nextTick - now).count() << " ms" << endl;
        }
        sendSnapShot();
        tickNumber++;
        if ( timeRemaining > std::chrono::milliseconds(0)){
            std::this_thread::sleep_for(timeRemaining);
        }

    }



}

void GameLobby::processPacket(unique_ptr<Packet> packet) {
    {
        std::lock_guard<std::mutex> guard(consoleMutex);
        printf("Game Lobby received packet of size %zu and data = \"%s\"\n\n", packet->packet->dataLength, packet->packet->data);
    }



    // TODO: Enter a decision tree determining what authoritative decision is required
}

void GameLobby::update() {
    return;
}

void GameLobby::sendSnapShot() {
    return;
}

size_t GameLobby::getPartitionIndex() const {
    return partitionIndex;
}
