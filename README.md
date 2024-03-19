# Odyssey-Game-Server
<img width="1400" alt="Screen Shot 2024-03-19 at 6 17 14 PM" src="https://github.com/IntellectualsDev/Odyssey-Game-Server/assets/72891464/3c8ca696-8fa6-4330-bd7f-effdc021dbe9">

## Project Overview:
The GameServer project is a robust, horizantally scalable backend solution designed to support The Odyssey Platform's real-time multiplayer games. It is a part of The Odyssey MicroService Suite, encapsulating all game functionality. Its core functionality revolves around managing player connections, simulating game states to ensure validity, and implementing networking techniques such as Client Side Prediction, Entity Interpolation, and Lag Compensation to ensure smooth, real-time interactions among players across various devices. The server is engineered to handle high loads with minimal latency, providing a seamless gaming experience that is crucial for maintaining engagement and competitive fairness.

## Technology Stack:
The GameServer leverages a modern technology stack that includes `ENet` for reliable UDP networking, `FlatBuffers` for efficient data serialization, and `raylib` for game development and rendering. This stack is chosen for its performance, scalability, and flexibility, allowing the server to handle complex multiplayer dynamics and large volumes of data with efficiency and speed.

## System Architecture:
The architecture of the GameServer is modular, with key components handling specific aspects of the game's backend logic. These components include a Gateway endpoint, Partitioned Receive Packet Buffer of Circular Buffer's each with it's own relative Game Lobby, an Output Buffer, and a Transmitter Endpoint. These components interact seamlessly, driven by a central server engine that orchestrates the flow of data, utilizing unique_ptrs to ensure safe memory allocation/dealloaction, and it uses dependency injection in the form of LobbyManagementService to ensures inter-component access.

## Networking Model:
At its heart, the GameServer employs a client-server networking model optimized for multiplayer games. It uses ENet to manage connections and service their endpoint's UDP sockets, providing both reliability and high performance necessary for real-time gameplay. The model supports dynamic joining and leaving of players, efficient packet handling, and real-time synchronization, and tick based retreival, ensuring all players have a coherent view of the game world.

 ## Requirements:
Building and running the GameServer requires specific software and libraries. The server is developed to be platform-independent, with additional dependencies like raylib for those interested in integrating real-time rendering or simulation directly within the server environment. It runs C++17 standards and CMake 3.27 or greater.
It also requires these dependencies:
* `build-essential` (GNU Compiler)
* `libstdc++-11-dev` (C++ std libraries)
* `autoconf` and `libtool`
* `raylib`
*` enet`
* `flatbuffers`
* `wget` (OPTIONAL to make a request for CMake)
* `git` (OPTIONAL to retreive ENet Library)
  
## Game Lobby:
The GameLobby system facilitates creating, joining, and managing game sessions, providing players with the ability to group and enter games together. The Game Server run as independent threads each with their own receive buffer partition to poll from. They implement a defined tick rate, which is used to normalize non-standard client tick rates. It simulates game physics and actions to authoritatively control a global game state, using static update shell methods mirroring the game code. On each tick cient input packets are used to modify the global game state, and these changes or deltas are sent authoritatively to all other clients known as delta compression. It will support lag compensation by storing client's past states and accounting for the delay involved with the client side delay required for entity interpolation, by rewinding state by the client's entites delay. In addition, a simulation window is used for debugging to provide a server based view of the overall game state. 

## Buffers:
Buffers play a crucial role in managing the flow of network data. The PartitionedPacketBuffer Receive Buffer allows for segregating packet data into isolated sections for different game lobbies, allowing for O(1) retreival enhancing data management and scalability.The partitions are impelemented as custom CircularBuffer used for efficiently handling data streams in a FIFO manner. General PacketBuffer stores packets in a queue temporarily before they are processed or sent. All buffers implement thread safe operations to prevent race conditions using mutexes, and allowing for servicing threads to be polled on state changes.

## Endpoints:
The Gateway component is pivotal in managing incoming connections and routing packets to the correct destinations within the server. It encapsulates an ENetHost object, which is the ENet endpoint. This endpoint will not be publically available but will be shared via the Gateway Server, which serves as the Odyssey MicroService's Global Gateway. The Transmitter is tasked with sending packets to clients, carefully handling packet ordering and reliability, ensuring that game states are serialied via FlatBuffers and sent efficiently. The Transmitter is not adressable.

## DockerFile:
The provided `Dockerfile` facilitates the containerization of the GameServer, simplifying deployment and scaling. It encapsulates the server environment, dependencies, and configurations into a portable container. It enables the Game Server to be deployed on `AWS EC2` instances and other cloud services, while also enabling horizantal scaling via `Kubernetes` for load balancing.

## FlatBuffers:
FlatBuffers is utilized for its high-performance serialization capabilities, allowing for efficient network communication by compacting game state data into small, quickly processable formats. In addition, it's ability to presist as serialized data enables a significant performance increase, which is crucial for real-time gameplay responsiveness. The schema defined in `game_state.fbs` includes various packet content options including actions and states.


## Docker Container
To setup your Docker development environment (Debian based Linux (Ubunut, Linux Mint, etc):
  * `sudo apt-get install docker.io`
  * `sudo systemctl enable --now docker` Enable docker to run
  * `sudo usermod -aG docker $USER` Add your user to the user group to prevent always using sudo
  * `newgrp docker` apply it to your current session
  * `docker run hello-world` simple test to see if the setup was successful


To build the Docker Image from the associated Dockerfile run `docker build -t <image-name-here> .`

To save this image as a file to be distributed:
  * Save the image to a file: `docker save <image_name> <image_name>.tar
  * (If you the .tar has moved to another machine) Load the image `docker load < <image_name>.tar`

To instantiate a Docker Container from the Docker Image with host <-> container port forwarding `docker run -p <host port>:<container port (ideally should match port in EXPOSE>/udp <your_image_name>`

Since Docker Images take up a lot of space, these commands help manage the Images
* `Docker Images`
* `Docker rmi <optional "-f"> <ID Name>`
* `Docker image prune` It will remove images of containers that are no longer running (protected otherwise)


