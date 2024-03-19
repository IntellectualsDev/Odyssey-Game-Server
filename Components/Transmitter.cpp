//
// Created by Anshul Gowda on 2/24/24.
//

#include "Transmitter.h"

Transmitter::Transmitter(string gatewayIP, int port, PacketBuffer *transmitBuffer, int maxConnections, int numChannels, int incomingBandwith,
                         int outgoingBandwith):
    port(port),
    transmitBuffer(transmitBuffer),
    shutdownFlag(false){
        char* serverAddressChar = new char[gatewayIP.length()+1]; // convert string IP to char * used in enet set host ip
        strcpy(serverAddressChar, gatewayIP.c_str());
        printf("char array for Gateway Server = %s\n", serverAddressChar);

        enet_address_set_host_ip(&address, serverAddressChar);
        server = enet_host_create(&address,
                         maxConnections,
                         numChannels,
                         incomingBandwith,
                         outgoingBandwith
        );
    if (server == nullptr) {
        fprintf(stderr, "An error occurred while trying to create Transmitter Server ENetHost instance\n");
        exit(EXIT_FAILURE);
    }
    printf("Created Transmitter Server ENetHost  instance @ %x:%u\n", server->address.host, server->address.port);
}

void Transmitter::start() {
    shutdownFlag.store(false);
    transmitThread = thread(&Transmitter::transmitLoop, this);
}

void Transmitter::transmitLoop() {
    while(!shutdownFlag.load()){
        auto packet = transmitBuffer->removePacket();
        if(packet){
            transmitPacket(std::move(packet));
        }
        else{
            fprintf(stderr, "ERR in Transmitter's transmitLoop() method | line 27\n\tTransmitter pulled a NULL packet from buffer\n\n");
        }
    }

}

void Transmitter::transmitPacket(unique_ptr<BufferHandler> packet){
//    ENetEvent event;
//    ENetAddress clientAddress = packet->toSendAddress;
//
//    if(packet->isSendAddressSet){
//        ENetPeer* client = enet_host_connect(server, &clientAddress, 1, 0);
//
//        if(client == nullptr){
//            fprintf(stderr, "ERR In Transmitter's Transmit method() | line 41\n\tTransmitter's ENet Host cannot support initiating a new ENet Connection\n\n");
//
//            // TODO: implement error catching in case ENetHost cannot support creating a new connection
//            exit(EXIT_FAILURE); // temporary
//        }
//        if(enet_host_service(server, &event, 6000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT){
//            printf("Transmitter Connected to the server successfully\n\tIP: %u\n\tPort: %s\n\n", server->address.host, to_string(port).c_str());
//        }
//        else{
//            enet_peer_reset(client);
//            fprintf(stderr, "ERR In Transmitter's Transmit method() | line 51\n\tFailed to connect to server\n\tIP: %u\n\tPort: %s\n\n", server->address.host, to_string(port).c_str());
//        }
//
//        enet_peer_send(client, 0, packet->packet);
//        enet_host_flush(server);
//
//        printf("Transmitter Sent packet\n\tdata = %s", packet->packet->data);
//    }
//    else{
//        fprintf(stderr, "ERR In Transmitter's Transmit method() | Line 60\n\tPacket to be sent does not have recipient data set\n\n");
//    }
}

void Transmitter::shutdown() {
    shutdownFlag.store(true);
    transmitBuffer->notifyAll();

    if(transmitThread.joinable()){
        transmitThread.join();
    }
}

