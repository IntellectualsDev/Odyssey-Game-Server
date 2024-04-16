//
// Created by Anshul Gowda on 2/18/24.
//

#include "Gateway.h"
#include "../Development Env/flatbuffers/include/flatbuffers/flatbuffers.h"
#include "../game_state_generated.h"
#include "../Data Structs/BufferHandler.h"
#include <memory>

Gateway::Gateway(std::string gatewayIP, int gatewayPort, PartitionedPacketBuffer* buffer, ConnectionManager* connectionManager,  int maxConnections, int numChannels, int incomingBandwith,
                 int outgoingBandwith) : receiveBuffer(buffer), connectionManager(connectionManager) {
    char* serverAddressChar = new char[gatewayIP.length()+1]; // convert string IP to char * used in enet set host ip
    strcpy(serverAddressChar, gatewayIP.c_str());
    printf("char array for Gateway Server = %s\n", serverAddressChar);

    enet_address_set_host_ip(&address, serverAddressChar);
    address.port = gatewayPort;
    port = gatewayPort;

    if(maxConnections > 4000){
        fprintf(stderr, "ENet only supports < 4000 connections to one ENetHost\n");
        exit(EXIT_FAILURE);
    }

    server = enet_host_create(&address,
                              maxConnections,
                              numChannels,
                              incomingBandwith,
                              outgoingBandwith
    );

    if (server == NULL) {
        fprintf(stderr, "An error occurred while trying to create Gateway Server ENetHost instance\n");
        exit(EXIT_FAILURE);
    }
    printf("Created Gateway Server ENetHost  instance @ %x:%u\n", server->address.host, server->address.port);
}

Gateway::~Gateway() {
    if(shutdownFlag.load()){
        shutdown();
    }
}

void Gateway::start() {
    shutdownFlag.store(false);
    networkThread = thread(&Gateway::networkLoop, this); // create the networkLoop thread and allow it to execute
}

void Gateway::networkLoop() {
    if(server == NULL){
        fprintf(stderr, "Gateway was not initialized. No ENetHost exists.\n");
        exit(EXIT_FAILURE);
    }

    ENetEvent event;
    printf("Network Loop Initialized\n");

    while (!shutdownFlag.load()) {
//        cout << "Gateway Running: " << noboolalpha << shutdownFlag.load() << endl;
        //        printf("In the network loop");
        while (enet_host_service(server, &event, 0) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    printf("A new client connected from %x:%u.\n", event.peer->address.host, event.peer->address.port);
                    break;

                case ENET_EVENT_TYPE_RECEIVE: {
                    cout << "Received Packet!" << endl;
                    //TODO: Pass to the middle buffer, if NEW CLIENT slaves will translate IP to MAC ID if authenticated

                    auto odPacket = GetOD_Packet(event.packet->data);
                    if(!odPacket){
                        cerr << "Invalid Packet: No serial Data" << endl;
                        break;
                    }

                    // TODO: Unravel the serialized bytes and read the lobby_number field and then call pushToPartition(int lobby_number):
                    auto lobbyNumber = odPacket->lobby_number();
                    cout << "A packet of length " << event.packet->dataLength << " was received from " << odPacket->source_point()->address() << ", " << odPacket->source_point()->port() << endl;

                    if(receiveBuffer->partitionExists(lobbyNumber)){
                        //copy the bytes from EnetPacket into a temp, because that previous pointer to a byte array is managed by Enet, so we can't simply steal that pointer
                        std::unique_ptr<uint8_t[]> bufferCopy(new uint8_t[event.packet->dataLength]);
                        memcpy(bufferCopy.get(), event.packet->data, event.packet->dataLength);
                        unique_ptr<BufferHandler> packetBufferHandler = std::make_unique<BufferHandler>(std::move(bufferCopy), event.packet->dataLength);
                        receiveBuffer->pushToPartition(lobbyNumber, std::move(packetBufferHandler));

                        cout << "Added to Lobby #" << lobbyNumber << "'s partition" << endl;
                    }
                    else{
                        cerr << "Lobby #" << lobbyNumber << " does not exist." << endl;
                    }

                    // Clean up the packet now that we're done using it
                    enet_packet_destroy(event.packet);
                    break;
                }

                    // TODO: Either client was successfully to game server or connection was dropped. No Action Required
                case ENET_EVENT_TYPE_DISCONNECT:
                    printf("%s disconnected.\n", event.peer->data);
            }
        }
    }
}

void Gateway::shutdown() {
//    printf("Initializing Gateway Server Shutdown Process\n");
    ENetEvent event;
    shutdownFlag.store(false);

    //TODO: refactor to use the ConnectionManager's PlayerPeers map
//    if (!clientList.empty()){
//        printf("Disconnecting from Gateway Server's Clients:\n");
//        map<string, pair<string, ENetPeer *>>::iterator it;
//
//        for(it = clientList.begin(); it != clientList.end(); it++){
//            enet_peer_disconnect(it->second.second, 0);
//
//            while(enet_host_service (server, &event, 200) > 0){
//                switch(event.type) {
//                    case ENET_EVENT_TYPE_RECEIVE:
//                        enet_packet_destroy(event.packet);
//                        break;
//                    case ENET_EVENT_TYPE_DISCONNECT:
//                        printf("\t%x:%u disconnected\n", it->second.second->address.host,
//                               it->second.second->address.port);
//                }
//            }
//
//            // if a disconnect event is not received, manually kill the connection
//            enet_peer_reset(it->second.second);
//        }
//    }

    // Notify waiting threads to wake up and complete any work, for the future. Can be removed in current implementation.
    {
        unique_lock<mutex> lock(networkMutex);
        networkCV.notify_all();
    }

    networkThread.join(); // the main thread will not wait for the network thread to wait before executing
//    delete this;
    return;
}

ENetHost* Gateway::getServer() const {
    return server;
}

const ENetAddress& Gateway::getAddress() const {
    return address;
}

int Gateway::getPort() const {
    return port;
}

const atomic<bool> &Gateway::getShutdownFlag() const {
    return shutdownFlag;
}

//const map<string, pair<string, ENetPeer *>> &Gateway::getClientList() const {
//    return clientList;
//}
