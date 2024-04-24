//
// Created by Anshul Gowda on 3/3/24.
//

#ifndef ODYSSEY_GAME_SERVER_CIRCULARBUFFER_H
#define ODYSSEY_GAME_SERVER_CIRCULARBUFFER_H



#include <vector>
#include <enet/enet.h>

// needed for manual build
#include <mutex> 
#include <memory>
#include <condition_variable>

#include "../game_state_generated.h"
#include "../Data Structs/BufferHandler.h"
//#include "../Buffers/PacketBuffer.h"
//#include "../Templates/ThreadSafeData.h"

using namespace std;

class CircularBuffer {
public:
    explicit CircularBuffer(size_t capacity);

    bool push(std::unique_ptr<ENetPacket> packet);

    std::unique_ptr<ENetPacket> pop();

    vector<unique_ptr<ENetPacket>> popAll();

    void resetBuffer();

    void notifyAll();

    bool isEmpty() const;

    bool isFull() const;


private:
    std::vector<std::unique_ptr<ENetPacket>> buffer;

    mutable std::mutex mutex;
    std::condition_variable not_empty;

    size_t head = 0;
    size_t tail = 0;
    size_t count = 0;
public:
    size_t getCount() const;

private:
    size_t capacity = 0;
};

#endif //ODYSSEY_GAME_SERVER_CIRCULARBUFFER_H
