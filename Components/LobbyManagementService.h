//
// Created by Anshul Gowda on 3/3/24.
//

#ifndef ODYSSEY_GAME_SERVER_LOBBYMANAGEMENTSERVICE_H
#define ODYSSEY_GAME_SERVER_LOBBYMANAGEMENTSERVICE_H

#include "../Buffers/PartitionedPacketBuffer.h"
#include "../Buffers/PacketBuffer.h"

class LobbyManagementService {
public:
    LobbyManagementService(PartitionedPacketBuffer& receiveBuffer, PacketBuffer& outputBuffer); // TODO: Inject the output Buffer as well

    // TODO: Partitioned Receive Buffer methods
    optional<size_t> createReceiveBufferPartition();
    void freeReceiveBufferPartition(size_t index);

    unique_ptr<BufferHandler> popFromReceiveBufferParition(size_t index);
    vector<unique_ptr<BufferHandler>> popAllFromReceiveBufferParition(size_t index);
    bool pushToReceiveBufferPartition(size_t index, unique_ptr<BufferHandler> packet);
    void notifyAllOnPartition(size_t index);

    void receiveBufferStats();
    void myPartitionStats();

    // TODO: Standard Output Buffer methods
    void pushToOutputBuffer(unique_ptr<BufferHandler> packet);

    // TODO: Encapsulated Game Logic methods


    // TODO:


private:
    PartitionedPacketBuffer& receiveBuffer;
    PacketBuffer& outputBuffer;

};


#endif //ODYSSEY_GAME_SERVER_LOBBYMANAGEMENTSERVICE_H
