//
// Created by Anshul Gowda on 3/3/24.
//


#include <iostream>
#include <enet/enet.h>

#include "../Buffers/PartitionedPacketBuffer.h"
#include "../Buffers/PacketBuffer.h"

#include <mutex>
mutex consoleMutex;

void producer(PartitionedPacketBuffer& buffer, size_t partitionIndex, int startId, int endId) {
    for (int i = startId; i <= endId; ++i) {
        ENetPacket* enetPacket = enet_packet_create(&i, sizeof(i), ENET_PACKET_FLAG_RELIABLE);
        auto packet = make_unique<Packet>("Packet " + to_string(i), enetPacket);
//        cout << "Producer put packet with label" << packet->label << endl;

        buffer.pushToPartition(partitionIndex, move(packet));
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}

void consumer(PartitionedPacketBuffer& buffer, size_t partitionIndex, int numPackets){
    for(int i = 0; i < numPackets; i++){
        auto packet = buffer.popFromPartition(partitionIndex);
        if(packet) {
            lock_guard<std::mutex> guard(consoleMutex);
            cout << "Consumer got packet with label " << packet->label << endl;
            enet_packet_destroy(packet->packet);
        }
        this_thread::sleep_for(chrono::milliseconds(5));
    }
}

//void producer(PartitionedPacketBuffer& buffer, int numPackets) {
//    for (int i = 1; i <= numPackets; ++i) {
//        // Simplified packet creation without actual ENetPacket payload for clarity
//        auto packet = std::make_unique<Packet>("Packet " + std::to_string(i), nullptr);
//        buffer.pushToPartition(0, std::move(packet)); // Assuming partition 0 is pre-allocated
//        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Simulate some work
//    }
//}
//
//void consumer(PartitionedPacketBuffer& buffer, int numPackets) {
//    int packetsConsumed = 0;
//    while (packetsConsumed < numPackets) {
//        auto packet = buffer.popFromPartition(0); // Assuming partition 0 is where packets are pushed
//        if (packet) {
//            std::cout << "Consumer got " << packet->label << std::endl;
//            packetsConsumed++;
//        }
//    }
//}

int main() {
    if (enet_initialize () != 0)
    {
        fprintf (stderr, "An error occurred while initializing ENet.\n");
        return EXIT_FAILURE;
    }
    atexit (enet_deinitialize);

    const size_t bufferSize = 10;
    const size_t numPartitions = 3;
    PartitionedPacketBuffer buffer(numPartitions, bufferSize);

    auto partition1 = buffer.allocatePartition().value_or(-1);
    auto partition2 = buffer.allocatePartition().value_or(-1);

    if(partition1 == -1 || partition1 == -1) {
        cerr << "Failed to allocate partitions" << endl;
    }

    // Start producer and consumer threads for each partition
    std::thread producer1(producer, std::ref(buffer), partition1, 1, 50);
    std::thread consumer1(consumer, std::ref(buffer), partition1, 50);

    std::thread producer2(producer, std::ref(buffer), partition2, 51, 100);
    std::thread consumer2(consumer, std::ref(buffer), partition2, 50);

    // Wait for all threads to finish
    producer1.join();
    consumer1.join();
    producer2.join();
    consumer2.join();

//    const size_t bufferSize = 100; // Large enough to avoid overflow
//    const size_t numPackets = 50; // Number of packets to produce/consume
//    PartitionedPacketBuffer buffer(1, bufferSize);
//    auto partition = buffer.allocatePartition().value_or(-1);
//
//    if(partition == -1){
//        cout << "Partition creation error" << endl;
//    }
//
//    thread producerThread(producer, ref(buffer), numPackets);
//    thread consumerThread(consumer, ref(buffer), numPackets);
//
//    producerThread.join();
//    consumerThread.join();

    return 0;
}