//
// Created by Anshul Gowda on 2/18/24.
//

#ifndef ODYSSEYGAMESERVER_GATEWAY_H
#define ODYSSEYGAMESERVER_GATEWAY_H

#include <string>
#include <map>

// include in manual build
#include <cstring>

#include "../Buffers/PartitionedPacketBuffer.h"
#include "../game_state_generated.h"
#include "../Components/ConnectionManager.h"

using namespace std;


class Gateway {
public:
    Gateway(string gatewayIP, int gatewayPort, PartitionedPacketBuffer* buffer, ConnectionManager* connectionManager, int maxConnections=3500, int numChannels=20, int incomingBandwith=0, int outgoingBandwith=0);
    ~Gateway();

    void start();

    void shutdown();
//    GatewayServer(string gatewayServerIP, int gatewayServerPort, int maxConnections, int numChannels, int incomingBandwith, int outgoingBandwith);
    ENetHost *getServer() const;

    const ENetAddress &getAddress() const;

    int getPort() const;

    const atomic<bool> &getShutdownFlag() const;

    const map<string, pair<string, ENetPeer *>> &getClientList() const;

private:

    void networkLoop();

    ENetHost* server;
    ENetAddress address;
    int port;

    thread networkThread;
    mutex networkMutex;
    condition_variable networkCV;
    atomic<bool> shutdownFlag;

    PartitionedPacketBuffer* receiveBuffer;
//    map<string, pair<string, ENetPeer *>> clientList;
    ConnectionManager * connectionManager;

};


#endif //ODYSSEYGAMESERVER_GATEWAY_H
