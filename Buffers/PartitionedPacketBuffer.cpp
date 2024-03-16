//
// Created by Anshul Gowda on 3/3/24.
//

#include "PartitionedPacketBuffer.h"

PartitionedPacketBuffer::PartitionedPacketBuffer(size_t numPartitions, size_t bufferSize, std::mutex& consoleMutex) : maxBufferSize(bufferSize), maxPartitions(numPartitions), consoleMutex(consoleMutex) {}

optional<size_t> PartitionedPacketBuffer::allocatePartition() {
    lock_guard<std::mutex> lock(mutex); // use a lock guard because it keep lock untl its destruction
    if(!freePartitions.empty()) {
        auto it = freePartitions.begin();
        size_t index = *it;
        freePartitions.erase(it);
        return index;
    }
    else if(partitions.size() < maxPartitions) {
        partitions.emplace_back(make_unique<CircularBuffer>(maxBufferSize)); // set the max capacity of the circ buffer
        return partitions.size() - 1;
    }
    return std::nullopt;

//    return writeAction([&]() -> optional<size_t> {
//
//        // Check the set of freed partitions and pull the first one
//        if(!freePartitions.empty()) {
//            auto it = freePartitions.begin();
//            size_t index = *it;
//            freePartitions.erase(it);
//            return index;
//        }
//
//        // if under receiveBuffer partition capacity create a new partition and return the index
//        else if(partitions.size() < maxPartitions) {
//            partitions.emplace_back(make_unique<CircularBuffer>(maxBufferSize)); // set the max capacity of the circ buffer
//            return partitions.size() - 1;
//        }
//        return nullopt;
//    });
}

// TODO: Determine the necessity of having a nested lock here
void PartitionedPacketBuffer::freePartition(size_t index) {
//    lock_guard<std::mutex> lock(mutex);
    if (index < partitions.size()){
        // Go reset the Circular buffer within this freed partition to allow for reuse
        partitions[index]->resetBuffer();
        // Add it to the freed partition set
    }
    lock_guard<std::mutex> lock(mutex);
    {
        freePartitions.insert(index);
    }

//    writeAction([&]() -> void {
//        if (index < partitions.size()){
//            // Go reset the Circular buffer within this freed partition to allow for reuse
//            partitions[index]->resetBuffer();
//            // Add it to the freed partition set
//            freePartitions.insert(index);
//        }
//    });
}

bool PartitionedPacketBuffer::pushToPartition(size_t index, unique_ptr<Packet> packet) {
    if(index >= partitions.size()){
        return false;
    }
    {
        std::lock_guard<std::mutex> guard(consoleMutex); //testing
        cout << "Added Packet Label: " << packet->label << "\ncount = " << partitions[index]->getCount()+1 << endl;
    }

    return partitions[index]->push(move(packet));
}

unique_ptr<Packet> PartitionedPacketBuffer::popFromPartition(size_t index) {
    if(index >= partitions.size()){
        return nullptr;
    }
    return partitions[index]->pop();
}

void PartitionedPacketBuffer::notifyAllOnPartition(size_t index) {
    partitions[index]->notifyAll();
}