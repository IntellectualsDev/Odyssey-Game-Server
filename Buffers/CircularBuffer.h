//
// Created by Anshul Gowda on 3/3/24.
//

#ifndef ODYSSEY_GAME_SERVER_CIRCULARBUFFER_H
#define ODYSSEY_GAME_SERVER_CIRCULARBUFFER_H

#include <vector>
#include <enet/enet.h>
#include "../Buffers/PacketBuffer.h"

using namespace std;

class CircularBuffer {
public:
    explicit CircularBuffer(size_t capacity);

    bool push(unique_ptr<Packet> packet);

    unique_ptr<Packet> pop();

    bool isEmpty() const;

    bool isFull() const;

private:
    vector<unique_ptr<Packet>> buffer;
    size_t head = 0;
    size_t tail = 0;
    size_t count = 0;
    size_t capacity = 0;
};

#endif //ODYSSEY_GAME_SERVER_CIRCULARBUFFER_H
