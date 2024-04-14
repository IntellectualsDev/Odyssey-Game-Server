//
// Created by Anshul Gowda on 3/16/24.
//

#ifndef ODYSSEY_GAME_SERVER_LIFOCIRCULARBUFFER_H
#define ODYSSEY_GAME_SERVER_LIFOCIRCULARBUFFER_H

#include <memory>

template <typename T>
class LIFOCircularBuffer {
public:
    LIFOCircularBuffer(std::size_t size)
            : buffer(std::make_unique<T[]>(size)), size(size), count(0){}

    bool isEmpty() const {
        return count == 0;
    }

    void push(T &value) {
//        newestPacket = value->clone();
        buffer[head] = value;
        head = (head + 1) % size;
        if (count < size) ++count;
    }

    void push(T &&value) {
//        newestPacket = value->clone();
        buffer[head] = std::move(value);
        head = (head + 1) % size;
        if (count < size) ++count;
    }

    T pop() {
        if (isEmpty()) throw std::runtime_error("Buffer is empty");
        head = (head - 1 + size) % size;
        --count;
        return std::move(buffer[head]);
    }

    T &peek() const {
        if (isEmpty()) throw std::runtime_error("Buffer is empty");
        auto peekIndex = (head - 1 + size) % size;
        return buffer[peekIndex];
    }

    // Create an iterator to pull out the ith element of the buffer
    T &peekAtIndex(int i) const {
        if (isEmpty()) throw std::runtime_error("Buffer is empty");
        if (i < 0 || i >= count) throw std::out_of_range("LIFOCircularBuffer Index out of Range");
        auto peekIndex = (head - 1 - i + size) % size;
        return buffer[peekIndex];
    }

    size_t getCount() const {
        return count;
    }


//    bool peekIfNull(){
//        if(buffer[head] == nullptr){
//            return true;
//        }
//        else{
//            return false;
//        }
//    }

private:
//    std::unique_ptr<T> newestPacket;
    std::unique_ptr<T[]> buffer;
    std::size_t head = 0;
    std::size_t size = 0;
    std::size_t count = 0;

};
#endif //ODYSSEY_GAME_SERVER_LIFOCIRCULARBUFFER_H
