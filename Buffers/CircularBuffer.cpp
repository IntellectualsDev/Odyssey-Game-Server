//
// Created by Anshul Gowda on 3/3/24.
//

#include "CircularBuffer.h"

CircularBuffer::CircularBuffer(size_t capacity) : capacity(capacity){
    buffer.resize(capacity);
}

bool CircularBuffer::push(unique_ptr<BufferHandler> packet) {
    unique_lock<std::mutex> lock(mutex);

    if(isFull()){
        return false;
    }
    buffer[tail] = std::move(packet);
    tail = (tail + 1) % capacity;
    count++;
    not_empty.notify_one();
    return true;

    //    return writeAction([&]() -> bool{
//        if (isFull()) {
//            return false;
//        }
//        buffer[tail] = std::move(packet);
//        tail = (tail + 1) % capacity;
//        count++;
//        return true;
//    });
}

unique_ptr<BufferHandler> CircularBuffer::pop() {
    unique_lock<std::mutex> lock(mutex);
    if(isEmpty()){
        return nullptr;
    }

    auto packet = std::move(buffer[head]);
    head = (head + 1) % capacity;
    count--;
    return packet;


    // previous wait loop, that would cause blocking wait until a packet is produced
//    unique_lock<std::mutex> lock(mutex);
//    not_empty.wait(lock, [this] {
//        return (!isEmpty());
//    });
//
//    auto packet = std::move(buffer[head]);
//    head = (head + 1) % capacity;
//    count--;
//    return packet;
}

//TODO: get rid of optional
vector<unique_ptr<BufferHandler>> CircularBuffer::popAll() {
    unique_lock<std::mutex> lock(mutex);
    if(isEmpty()){
        vector<unique_ptr<BufferHandler>> empty;
        return empty;
    }
    vector<unique_ptr<BufferHandler>> output;
    output.reserve(count);
    while(!isEmpty()){
        output.push_back(std::move(buffer[head]));
        head = (head+1) % capacity;
        count--;
    }
    return std::move(output);
}



void CircularBuffer::resetBuffer() {
    unique_lock<std::mutex> lock(mutex);

    for(auto& packet : buffer){
        packet.reset();
    }

    buffer.clear();
    buffer.resize(capacity);
    head = 0;
    tail = 0;
    count = 0;

//    writeAction([&]() -> void {
//        for(auto& packet : buffer){
//            packet.reset(); // this will clear the unique ptr contents, and unique ptrs will automatically destruct Packet
//        }
//
//        head = 0;
//        tail = 0;
//        count = 0;
//    });
}

void CircularBuffer::notifyAll() {
    not_empty.notify_all();
}

bool CircularBuffer::isEmpty() const {
    return count == 0;
}

bool CircularBuffer::isFull() const {
    return count == capacity;
}

size_t CircularBuffer::getCount() const {
    return count;
}
