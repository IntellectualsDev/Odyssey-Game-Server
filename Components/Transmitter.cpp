//
// Created by Anshul Gowda on 2/24/24.
//

#include "Transmitter.h"

Transmitter::Transmitter(string gatewayIP,
                         int port,
                         PacketBuffer *transmitBuffer,
                         ConnectionManager * connectionManager,
                         mutex& consoleMutex,
                         int maxConnections,
                         int numChannels,
                         int incomingBandwith,
                         int outgoingBandwith):
    port(port),
    transmitBuffer(transmitBuffer),
    connectionManager(connectionManager),
    consoleMutex(consoleMutex),
    shutdownFlag(false)
{
//        char* serverAddressChar = new char[gatewayIP.length()+1]; // convert string IP to char * used in enet set host ip
        std::vector<char> serverAddressChar(gatewayIP.begin(), gatewayIP.end());
        serverAddressChar.push_back('\0');

//        strcpy(serverAddressChar, gatewayIP.c_str());
//        printf("char array for Gateway Server = %s\n", serverAddressChar);

        enet_address_set_host_ip(&address, serverAddressChar.data());
        server = enet_host_create(&address, //TODO: the &address should be replaced with NULL !!! (since it is sending)
                         maxConnections,
                         numChannels,
                         incomingBandwith,
                         outgoingBandwith
        );
    if (server == nullptr) {
        fprintf(stderr, "An error occurred while trying to create Transmitter Server ENetHost instance\n");
        exit(EXIT_FAILURE);
    }
   fprintf(stdout, "Created Transmitter Server ENetHost  instance @ %x:%u\n", server->address.host, server->address.port);
}

void Transmitter::start() {
    shutdownFlag.store(false);
    transmitThread = thread(&Transmitter::transmitLoop, this);
}

void Transmitter::shutdown() {
    shutdownFlag.store(true);
    transmitBuffer->notifyAll();

    if(transmitThread.joinable()){
        transmitThread.join();
    }
}

void Transmitter::transmitLoop() {
    while(!shutdownFlag.load()){
        auto bufferHandler = transmitBuffer->removePacket();
        if(bufferHandler != nullptr){
            transmitPacket(std::move(bufferHandler));
        }
        else{
            fprintf(stderr, "ERR in Transmitter's transmitLoop() method | line 27\n\tTransmitter pulled a NULL packet from buffer\n\n");
        }
    }

}

void Transmitter::transmitPacket(unique_ptr<ENetPacket> packet){
    ENetEvent event;
    ENetPeer * client;
//    ENetPacket* packetToSend;

//    const OD_Packet* OD_Packet = packet->data;
    auto byteBuffer = packet->data;
    auto od_Packet = flatbuffers::GetRoot<OD_Packet>(byteBuffer);

//    packetToSend = enet_packet_create(packet->getByteView(), packet->getSize(), flags);
    int clientID = od_Packet->client_id();

    client = connect(od_Packet->dest_point()->address()->str(), od_Packet->dest_point()->port());
    if (client != nullptr) {
        connectionManager->setPeer(clientID, client);
    } else {
        fprintf(stderr, "Transmit Error: Unable to connect to client ID#%d %s:%d, ENetPeer* is nullptr\n", clientID,
                od_Packet->dest_point()->address()->str().c_str(), od_Packet->dest_point()->port());
        return;
    }

//    if(!connectionManager->isPlayerConnected(clientID)) {
//        client = connect(OD_Packet->dest_point()->address()->str(), OD_Packet->dest_point()->port());
//        if (client != nullptr) {
//            connectionManager->setPeer(clientID, client);
//        } else {
//            fprintf(stderr, "Transmit Error: Unable to connect to client ID#%d %s:%d, ENetPeer* is nullptr\n", clientID,
//                    OD_Packet->dest_point()->address()->str().c_str(), OD_Packet->dest_point()->port());
//            return;
//        }
//    }
//    else {
//        client = connectionManager->getPeer(clientID);
//    }

    enet_host_service(server, & event, 0);
//    ENetPacket* packetToSend = enet_packet_create(packet->getByteView(), packet->getSize(), flags);
    enet_peer_send(client, 0, packet.get());
    enet_host_flush(server);

    {
        std::lock_guard<std::mutex> guard(consoleMutex);
        fprintf(stdout, "Game Server's Transmitter Sent packet\n\tpayload_type = %s\n", EnumNamePacketType(od_Packet->packet_type()));
    }
}

ENetPeer * Transmitter::connect(const std::string &clientIP, int port) {
    ENetAddress clientAddress;
    ENetEvent event;

//    char* clientAddressChar = new char[clientIP.length()+1];
    std::vector<char> clientAddressChar(clientIP.begin(), clientIP.end());
    clientAddressChar.push_back('\0');

//    strcpy(clientAddressChar,clientIP.c_str());

    enet_address_set_host_ip(& clientAddress, clientAddressChar.data());
    clientAddress.port = port;
    ENetPeer * client = enet_host_connect(server, &clientAddress, 2, 0);
    if(client == nullptr){
        {
            std::lock_guard<std::mutex> guard(consoleMutex);
            fprintf(stderr, "Client @ %s:%d not available for initiating an ENet connection w/ Game Server\n", clientIP.c_str(), port);
        }

        //TODO: add error checking or create a flag
    }
    if(enet_host_service(server, &event, 3000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT){
        {
            std::lock_guard<std::mutex> guard(consoleMutex);
            fprintf(stdout, "Game Server Connected to the Client %s:%d successfully\n\t", clientIP.c_str(), port);
        }
        return client;
    }
    else{
        enet_peer_reset(client);
        {
            std::lock_guard<std::mutex> guard(consoleMutex);
            fprintf(stderr, "Game Server failed to connect to Client\n\tIP: %s\n\tPort: %d\n", clientIP.c_str(), port);
        }
        return nullptr;
    }
}

