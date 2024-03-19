//
// Created by Anshul Gowda on 3/3/24.
//

#ifndef ODYSSEY_GAME_SERVER_PARTITIONEDPACKETBUFFER_H
#define ODYSSEY_GAME_SERVER_PARTITIONEDPACKETBUFFER_H

#include <optional>
#include <set>
#include "../Data Structs/BufferHandler.h"
#include "PacketBuffer.h"
#include "CircularBuffer.h"
#include <mutex>
#include <thread>
#include <condition_variable>

using namespace std;

class PartitionedPacketBuffer{
public:
    PartitionedPacketBuffer(size_t numPartitions, size_t bufferSize, std::mutex& consoleMutex);

    // TODO: Allocate partition (include reuse parition func)
    std::optional<size_t> allocatePartition();

    bool partitionExists(size_t index);

    // TODO: Free allocated partition
    void freePartition(size_t index);

    // TODO: push/pop from partitions
    bool pushToPartition(size_t index, unique_ptr<BufferHandler> packet);

    unique_ptr<BufferHandler> popFromPartition(size_t index);

    std::optional<vector<unique_ptr<BufferHandler>>> popAllFromPartition(size_t index);

    void notifyAllOnPartition(size_t index);


    // TODO: Print Buffer stats (avg partition fill %) (avg buffer fill &) (number of allocated partitions)
private:
    vector<unique_ptr<CircularBuffer>> partitions;
    set<size_t> freePartitions;

    std::mutex mutex;

    size_t maxPartitions;
    size_t maxBufferSize;

    std::mutex& consoleMutex;
};


#endif //ODYSSEY_GAME_SERVER_PARTITIONEDPACKETBUFFER_H
