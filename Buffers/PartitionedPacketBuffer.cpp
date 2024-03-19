//
// Created by Anshul Gowda on 3/3/24.
//

#include "PartitionedPacketBuffer.h"
//#include "../Development Env/flatbuffers/include/flatbuffers/flatbuffers.h"
#include "../game_state_generated.h"

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
}

bool PartitionedPacketBuffer::partitionExists(size_t index) {
    lock_guard<std::mutex> lock(mutex);
    {
        // if the partition is within the range of paritions and it is not in the free partitions set, then it exists
        if(index < partitions.size() && freePartitions.find(index) == freePartitions.end()){
            return true;
        }
    }
    return false;
}

bool PartitionedPacketBuffer::pushToPartition(size_t index, unique_ptr<BufferHandler> packet) {
    if(index >= partitions.size()){
        return false;
    }
//    {
//        std::lock_guard<std::mutex> guard(consoleMutex); //testing
//        cout << "Added Packet w/type: " << EnumNamePacketType(packet->getPacketView()->packet_type()) << "\n\t From Address: " << packet->getPacketView()->source_point()->address() << ", Port: " << packet->getPacketView()->source_point()->port() << endl;
//        cout << "Size: " << partitions[index]->getCount()+1 << endl;
//
////        cout << "Added Packet" << packet->label << "\ncount = " << partitions[index]->getCount()+1 << endl;
//    }

    return partitions[index]->push(std::move(packet));
}

unique_ptr<BufferHandler> PartitionedPacketBuffer::popFromPartition(size_t index) {
    if(index >= partitions.size()){
        return nullptr;
    }
    return partitions[index]->pop();
}

vector<unique_ptr<BufferHandler>> PartitionedPacketBuffer::popAllFromPartition(size_t index) {
    return partitions[index]->popAll();
}

void PartitionedPacketBuffer::notifyAllOnPartition(size_t index) {
    partitions[index]->notifyAll();
}