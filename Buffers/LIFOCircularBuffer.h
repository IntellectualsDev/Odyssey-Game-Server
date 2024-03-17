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
        : buffer(std::make_unique<T[]>(size)), size(size)
    {}

    void push(const T& value){
        buffer[head] = value;
        head = (head+1) % size;
    }

    T pop(){
        head = (head - 1 + size) % size;
        return buffer[head];
    }

private:
    std::unique_ptr<T[]> buffer;
    std::size_t head = 0;
    std::size_t size = 0;
};


#endif //ODYSSEY_GAME_SERVER_LIFOCIRCULARBUFFER_H
