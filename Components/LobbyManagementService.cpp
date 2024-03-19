//
// Created by Anshul Gowda on 3/3/24.
//

#include "LobbyManagementService.h"

LobbyManagementService::LobbyManagementService(PartitionedPacketBuffer &receiveBuffer, PacketBuffer &outputBuffer)
: receiveBuffer(receiveBuffer), outputBuffer(outputBuffer) {}

optional<size_t> LobbyManagementService::createReceiveBufferPartition() {
    return receiveBuffer.allocatePartition();
}

void LobbyManagementService::freeReceiveBufferPartition(size_t index) {
    receiveBuffer.freePartition(index);
}

unique_ptr<BufferHandler> LobbyManagementService::popFromReceiveBufferParition(size_t index) {
    return receiveBuffer.popFromPartition(index);
}

vector<unique_ptr<BufferHandler>> LobbyManagementService::popAllFromReceiveBufferParition(size_t index) {
    return receiveBuffer.popAllFromPartition(index);
}

bool LobbyManagementService::pushToReceiveBufferPartition(size_t index, unique_ptr<BufferHandler> packet) {
    return receiveBuffer.pushToPartition(index, std::move(packet));
}

void LobbyManagementService::pushToOutputBuffer(unique_ptr<BufferHandler> packet) {
    outputBuffer.addPacket(std::move(packet));
}

void LobbyManagementService::notifyAllOnPartition(size_t index) {
    receiveBuffer.notifyAllOnPartition(index);
}

void LobbyManagementService::receiveBufferStats() {

}

void LobbyManagementService::myPartitionStats() {

}
