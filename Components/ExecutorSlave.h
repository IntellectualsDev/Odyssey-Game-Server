//
// Created by Anshul Gowda on 2/18/24.
//

#ifndef ODYSSEYGAMESERVER_EXECUTORSLAVES_H
#define ODYSSEYGAMESERVER_EXECUTORSLAVES_H

#include <memory>
#include <thread>
#include <iostream>

#include "../Buffers/PacketBuffer.h"
#include "../Data Structs/ServerAddressInfo.h"



class ExecutorSlave {
public:
    // constrcutor
    ExecutorSlave(PacketBuffer& buffer, ServerAddressInfo& serverContact);
    ~ExecutorSlave();

    void start();
    void stop();

private:

    atomic<bool> stopFlag;
    PacketBuffer& buffer;
    ServerAddressInfo& serverContactInfo;
    thread workerThread;

    void run();
    static void processPacket(unique_ptr<Packet> packet);
};


#endif //ODYSSEYGAMESERVER_EXECUTORSLAVES_H
