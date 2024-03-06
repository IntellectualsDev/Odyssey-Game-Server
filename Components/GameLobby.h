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
    GameLobby(PartitionedPacketBuffer& receiveBuffer, PacketBuffer& outputBuffer);
    void start();
    void stop();

private:
    void run();
    void processPacket(unique_ptr<Packet> packet);

    LobbyManagementService LobbyServices;

    size_t partitionIndex;

    atomic<bool> stopFlag;
    thread workerThread;
};


#endif //ODYSSEY_GAME_SERVER_GAMELOBBY_H
