cmake_minimum_required(VERSION 3.27)
project(Odyssey_Game_Server)

set(CMAKE_CXX_STANDARD 17)

# Add the path to ENet header files, an explicit path to the .h files is not required, CMake will find them
include_directories(/usr/local/include)

include_directories("/Users/anshulgowda/Documents/CODE/Odyssey/Odyssey-Game-Server/Development Env/flatbuffers/include")

# Add the path to ENet library
link_directories(/usr/local/lib)

link_directories(/usr/local/bin)



add_executable(Odyssey_Game_Server main.cpp
        Servers/GameServer.h
        Servers/GameServer.cpp

        Buffers/PacketBuffer.h
        Buffers/PacketBuffer.cpp
        Components/ExecutorSlave.h
        Components/ExecutorSlave.cpp
        Components/Gateway.h
        Components/Gateway.cpp
        Components/Transmitter.h
        Components/Transmitter.cpp
        Templates/ThreadSafeData.h
        "Data Structs/ServerAddressInfo.h"
        "Data Structs/ServerAddressInfo.cpp"
        Buffers/PartitionedPacketBuffer.cpp
        Buffers/PartitionedPacketBuffer.h
        Components/LobbyManagementService.cpp
        Components/LobbyManagementService.h
        Buffers/CircularBuffer.cpp
        Buffers/CircularBuffer.h
        Components/GameLobby.cpp
        Components/GameLobby.h
        Buffers/LIFOCircularBuffer.cpp
        Buffers/LIFOCircularBuffer.h
)
    # TODO: Don't forget to add the encryption files when completed

# Link against the ENet library. It informs CMake that the executable needs to be linked with ENet library
# and that it is a required dependency
target_link_libraries(Odyssey_Game_Server enet)