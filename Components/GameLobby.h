//
// Created by Anshul Gowda on 3/3/24.
//

#ifndef ODYSSEY_GAME_SERVER_GAMELOBBY_H
#define ODYSSEY_GAME_SERVER_GAMELOBBY_H

#include "LobbyManagementService.h"

#include <thread>
#include <mutex>
#include <condition_variable>


using namespace std;

class GameLobby {
public:
    GameLobby(PartitionedPacketBuffer& receiveBuffer, PacketBuffer& outputBuffer, mutex& consoleMutex);
    void start();
    void stop();

private:
    void run();
    void processPacket(unique_ptr<Packet> packet);
    void update();
    void sendSnapShot();

    LobbyManagementService LobbyServices;

    size_t partitionIndex;
public:
    size_t getPartitionIndex() const;

private:

    atomic<bool> stopFlag;
    thread workerThread;

    uint32_t tickNumber = 0;
    const float tickRate = 60.0f;
//    const float secondPerTick = 1.0f / tickRate;
    std::chrono::steady_clock::time_point lastTick;

    mutex& consoleMutex;
};


#endif //ODYSSEY_GAME_SERVER_GAMELOBBY_H
