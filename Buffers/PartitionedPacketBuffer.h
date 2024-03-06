//
// Created by Anshul Gowda on 3/3/24.
//

#ifndef ODYSSEY_GAME_SERVER_PARTITIONEDPACKETBUFFER_H
#define ODYSSEY_GAME_SERVER_PARTITIONEDPACKETBUFFER_H

#include <optional>
#include <set>
#include "../Templates/ThreadSafeData.h"
#include "PacketBuffer.h"
#include "CircularBuffer.h"

using namespace std;

class PartitionedPacketBuffer : public ThreadSafeData{
public:
    PartitionedPacketBuffer(size_t numPartitions, size_t bufferSize);

    // TODO: Allocate partition (include reuse parition func)
    std::optional<size_t> allocatePartition();

    // TODO: Free allocated partition
    void freePartition(size_t index);

    // TODO: push/pop from partitions
    bool pushToPartition(size_t index, unique_ptr<Packet> packet);

    unique_ptr<Packet> popFromPartition(size_t index);

    void notifyAllOnPartition(size_t index);


    // TODO: Print Buffer stats (avg partition fill %) (avg buffer fill &) (number of allocated partitions)
private:
    vector<unique_ptr<CircularBuffer>> partitions;
    set<size_t> freePartitions;

    std::mutex mutex;

    size_t maxPartitions;
    size_t maxBufferSize;
};


#endif //ODYSSEY_GAME_SERVER_PARTITIONEDPACKETBUFFER_H
