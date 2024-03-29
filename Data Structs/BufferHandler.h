//
// Created by Anshul Gowda on 3/18/24.
//

#ifndef ODYSSEY_GAME_SERVER_BUFFERHANDLER_H
#define ODYSSEY_GAME_SERVER_BUFFERHANDLER_H

#include <memory>
#include "../game_state_generated.h"

class BufferHandler {
public:
    BufferHandler(std::unique_ptr<uint8_t[]> data, size_t size): data(std::move(data)), size(size) {};

    const OD_Packet* getPacketView() const {
        return GetOD_Packet(data.get());
    }


private:
    std::unique_ptr<uint8_t[]> data;
    size_t size;
};


#endif //ODYSSEY_GAME_SERVER_BUFFERHANDLER_H
