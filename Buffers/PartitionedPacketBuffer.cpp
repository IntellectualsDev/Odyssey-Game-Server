//
// Created by Anshul Gowda on 3/3/24.
//

#include "PartitionedPacketBuffer.h"

PartitionedPacketBuffer::PartitionedPacketBuffer(size_t numPartitions, size_t bufferSize) : maxBufferSize(bufferSize), maxPartitions(numPartitions) {}

optional<size_t> PartitionedPacketBuffer::allocatePartition() {
    return writeAction([&]() -> optional<size_t> {

        // Check the set of freed partitions and pull the first one
        if(!freePartitions.empty()) {
            auto it = freePartitions.begin();
            size_t index = *it;
            freePartitions.erase(it);
            return index;
        }

        // if under receiveBuffer partition capacity create a new partition and return the index
        else if(partitions.size() < maxPartitions) {
            partitions.emplace_back(make_unique<CircularBuffer>(maxBufferSize)); // set the max capacity of the circ buffer
            return partitions.size() - 1;
        }
        return nullopt;
    });
}

void PartitionedPacketBuffer::freePartition(size_t index) {
    writeAction([&]() -> void {
        if (index < partitions.size()){
            // Go reset the Circular buffer within this freed partition to allow for reuse
            partitions[index]->resetBuffer();
            // Add it to the freed partition set
            freePartitions.insert(index);
        }
    });
}

bool PartitionedPacketBuffer::pushToPartition(size_t index, unique_ptr<Packet> packet) {
    if(index >= partitions.size()){
        return false;
    }
    return partitions[index]->push(move(packet));
}

unique_ptr<Packet> PartitionedPacketBuffer::popFromPartition(size_t index) {
    if(index >= partitions.size()){
        return nullptr;
    }
    return partitions[index]->pop();
}