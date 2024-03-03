//
// Created by Anshul Gowda on 3/3/24.
//

#include "LobbyManagementService.h"

LobbyManagementService::LobbyManagementService(PartitionedPacketBuffer &receiveBuffer, PacketBuffer &outputBuffer)
: receiveBuffer(receiveBuffer), outputBuffer(outputBuffer) {}

size_t LobbyManagementService::createReceiveBufferPartition() {
    return receiveBuffer.allocatePartition();
}

void LobbyManagementService::freeReceiveBufferPartition(size_t index) {
    receiveBuffer.freePartition(index);
}

unique_ptr<Packet> LobbyManagementService::popFromReceiveBufferParition(size_t index) {
    return receiveBuffer.popFromPartition(index);
}

bool LobbyManagementService::pushToReceiveBufferPartition(size_t index, unique_ptr<Packet> packet) {
    return receiveBuffer.pushToPartition(index, std::move(packet));
}

void LobbyManagementService::pushToOutputBuffer(unique_ptr<Packet> packet) {
    outputBuffer.addPacket(std::move(packet));
}

void LobbyManagementService::receiveBufferStats() {

}

void LobbyManagementService::myPartitionStats() {

}
