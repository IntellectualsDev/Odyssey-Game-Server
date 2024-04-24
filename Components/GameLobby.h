//
// Created by Anshul Gowda on 3/3/24.
//

#ifndef ODYSSEY_GAME_SERVER_GAMELOBBY_H
#define ODYSSEY_GAME_SERVER_GAMELOBBY_H

#include "LobbyManagementService.h"
#include "../Games/FPSGame/FPS_Game.h"
#include "../Components/ConnectionManager.h"

#include <thread>
#include <mutex>
#include <condition_variable>

// Acts as the communication label for a client
//struct IPEndpoint {
//    enet_uint32 host;
//    enet_uint16 port;
//};


using namespace std;

class GameLobby {
public:
    GameLobby(PartitionedPacketBuffer* receiveBuffer, PacketBuffer* outputBuffer, ConnectionManager* connectionManager, std::mutex &consoleMutex, bool showGUI);
//    GameLobby(LobbyManagementService& lobbyManagementService): LobbyServices(lobbyManagementService){};
    void start();
    void stop();
    size_t getPartitionIndex() const;
    static constexpr const float tickRate = 60.0f;

private:
    void run();
    void serverTickInit();
    void processPacket(unique_ptr<BufferHandler> packet);
    void packetPreprocessing(BufferHandler& packet);
    void update(unique_ptr<ENetPacket> packet);
    void render();
    void sendDifferentials();
    void sendSnapShot();

//    void buildStateFlatBuffer(vector<unique_ptr<FPSClientState>>& states,
//                         flatbuffers::Offset<flatbuffers::String> sourcePoint,
//                         flatbuffers::Offset<flatbuffers::String> destPoint,
//                         bool reliable,
//                         PacketType packeType,
//                         PacketPayload payload,
//                         bool delta
//                         );

    void buildGenericFlatBuffer();

//    LobbyManagementService* LobbyServices;
    PartitionedPacketBuffer* receiveBuffer;
    PacketBuffer* outputBuffer;
    ConnectionManager* connectionManager;
    FPS_Game game;

    size_t partitionIndex;

    atomic<bool> stopFlag;
    thread workerThread;
    mutex& consoleMutex;

    uint32_t tickNumber = 0;
    const int ticksTillGlobalState = 20;
    const int wrapAround = 60;
//    const float tickRate = 60.0f;
    std::chrono::steady_clock::time_point lastTick;

    // change to reflect the current serverIP
    static constexpr const char* serverIP = "192.168.0.10";
    static constexpr const int serverPort = 5450;

    bool showGUI;
    int screenWidth;
    int screenHeight;
};


#endif //ODYSSEY_GAME_SERVER_GAMELOBBY_H
