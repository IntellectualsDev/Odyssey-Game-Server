//
// Created by Anshul Gowda on 4/15/24.
//

#ifndef ODYSSEY_GAME_SERVER_CONNECTIONMANAGER_H
#define ODYSSEY_GAME_SERVER_CONNECTIONMANAGER_H

#include <map>
#include <string>
#include <enet/enet.h>

struct IPEndpoint {
    std::string ip;
    int port;
};

class ConnectionManager {
public:
    void addPlayer(int playerID, const IPEndpoint& endpoint);

    void removePlayer(int playerID);

    ENetPeer * getPeer(int playerID);

    void setPeer(int playerID, ENetPeer *);

    IPEndpoint getEndpoint(int playerID);

    bool isPlayerConnected(int playerID);

    const std::map<int, IPEndpoint> &getPlayerEndpoints() const;

    const std::map<int, ENetPeer *> &getPlayerPeers() const;

private:
    std::map<int, IPEndpoint> playerEndpoints;
    std::map<int, ENetPeer *> playerPeers;
};


#endif //ODYSSEY_GAME_SERVER_CONNECTIONMANAGER_H
