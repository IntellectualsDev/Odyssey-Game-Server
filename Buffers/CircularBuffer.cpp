//
// Created by Anshul Gowda on 3/3/24.
//

#include "CircularBuffer.h"

CircularBuffer::CircularBuffer(size_t capacity) : capacity(capacity){
    buffer.resize(capacity);
}

bool CircularBuffer::push(unique_ptr<Packet> packet) {
    return writeAction([&]() -> bool{
        if (isFull()) {
            return false;
        }
        buffer[tail] = std::move(packet);
        tail = (tail + 1) % capacity;
        count++;
        return true;
    });
}

unique_ptr<Packet> CircularBuffer::pop() {
    return writeAction([&]() -> unique_ptr<Packet> {
        if(isEmpty()){
            return nullptr;
        }
        auto packet = std::move(buffer[head]);
        head = (head + 1) & capacity;
        count--;
        return packet;
    });
}

void CircularBuffer::resetBuffer() {
    writeAction([&]() -> void {
        for(auto& packet : buffer){
            packet.reset(); // this will clear the unique ptr contents, and unique ptrs will automatically destruct Packet
        }

        head = 0;
        tail = 0;
        count = 0;
    });
}

bool CircularBuffer::isEmpty() const {
    return count == 0;
}

bool CircularBuffer::isFull() const {
    return count == capacity;
}
