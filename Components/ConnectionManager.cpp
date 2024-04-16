//
// Created by Anshul Gowda on 4/15/24.
//

#include "ConnectionManager.h"

void ConnectionManager::addPlayer(int playerID, const IPEndpoint &endpoint) {
    playerEndpoints[playerID] = endpoint;
    playerPeers[playerID] = nullptr;  // Initially not connected
}

void ConnectionManager::removePlayer(int playerID) {
    playerEndpoints.erase(playerID);
    playerPeers.erase(playerID);
}

IPEndpoint ConnectionManager::getEndpoint(int playerID) {
    if (playerEndpoints.find(playerID) != playerEndpoints.end()) {
        return playerEndpoints[playerID];
    }
    return {"", 0};
}

ENetPeer *ConnectionManager::getPeer(int playerID) {
    return playerPeers[playerID];
}

void ConnectionManager::setPeer(int playerId, ENetPeer* peer) {
    playerPeers[playerId] = peer;
}

bool ConnectionManager::isPlayerConnected(int playerID) {
    return playerPeers[playerID] != nullptr;
}

const std::map<int, IPEndpoint> &ConnectionManager::getPlayerEndpoints() const {
    return playerEndpoints;
}

const std::map<int, ENetPeer *> &ConnectionManager::getPlayerPeers() const {
    return playerPeers;
}

