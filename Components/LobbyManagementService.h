//
// Created by Anshul Gowda on 3/3/24.
//

#ifndef ODYSSEY_GAME_SERVER_LOBBYMANAGEMENTSERVICE_H
#define ODYSSEY_GAME_SERVER_LOBBYMANAGEMENTSERVICE_H

#include "../Buffers/PartitionedPacketBuffer.h"

class LobbyManagementService {
public:
    LobbyManagementService(PartitionedPacketBuffer& receiveBuffer);

    // TODO: Create an allocated partition method (encapsulate reused partition func.)

    // TODO: Free allocated partition method

    // TODO: Print Lobby partition stats

private:
    PartitionedPacketBuffer& receiveBuffer;

};


#endif //ODYSSEY_GAME_SERVER_LOBBYMANAGEMENTSERVICE_H
