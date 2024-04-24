//
// Created by Anshul Gowda on 2/24/24.
//

#ifndef ODYSSEYGAMESERVER_TRANSMITTER_H
#define ODYSSEYGAMESERVER_TRANSMITTER_H

#include <thread>
#include <map>

#include "../Buffers/PacketBuffer.h"
#include "../Data Structs/BufferHandler.h"
#include "../Components/ConnectionManager.h"


class Transmitter {
public:
    Transmitter(string gatewayIP,
                int port,
                PacketBuffer *transmitBuffer,
                ConnectionManager * connectionManager,
                mutex& consoleMutex,
                int maxConnections=3500,
                int numChannels=20,
                int incomingBandwith=0,
                int outgoingBandwith=0);

    void start();

    void shutdown();




private:
    // Pass down in GatewayServer constructor
    void transmitLoop();
    ENetPeer * connect(const string& clientIP, int port);
    bool disconnect(const string& client);
    void transmitPacket(unique_ptr<ENetPacket> packet);

    ENetHost* server;
    ENetAddress address;
//    vector<ENetPeer *> peers;
    int port;

    thread transmitThread;
    atomic<bool> shutdownFlag;

    PacketBuffer* transmitBuffer;
    ConnectionManager* connectionManager;

    std::mutex& consoleMutex;
};


#endif //ODYSSEYGAMESERVER_TRANSMITTER_H
