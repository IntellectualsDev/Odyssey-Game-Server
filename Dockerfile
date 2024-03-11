FROM ubuntu:22.04

ARG DEBIAN_FRONTEND=noninteractive

EXPOSE 7777/udp

#Install the dependencies required for the build
    # build-essential (GNU Compiler)
    # libstdc++-11-dev (C++ std libraries)
    # wget to make a request for Cmake
    # git to retreive the ENet library
    # autoconf and libtool (dependency of autoconf)

# "apt-get clean" will delete the leftover .deb files from the install
# "rm -rf "var/lib/apt/lists/*" deletes the package list downloaded by "apt-get update"

RUN apt-get update && \
    apt-get install -y build-essential libstdc++-11-dev wget git libtool autoconf && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# fetch the ENet source code from GitHub, then build, compile, and install
    # ldconfig creates links and cache to trusted directory (usr/lib || lib)

RUN git clone https://github.com/lsalzman/enet.git /tmp/enet

RUN cd /tmp/enet && \ 
    autoreconf -vfi && \ 
    ./configure && \ 
    make && \ 
    make install && \
    ldconfig

# Utilize wget to make a HTTP request to download CMake binary and specify the output path as "tmp" and name it "cmake.tar.gz"
# Uncompress the dictated "/tmp/cmake.tar.gz" w/-f, verbose -v. Put it in /usr/local (container filesytem). Throw away top level directories w/strip_components
# Delete the old compressed cmake.tar.gz
    # Cmake 3.27.9 via Kiteware
RUN wget https://github.com/Kitware/CMake/releases/download/v3.27.9/cmake-3.27.9-linux-x86_64.tar.gz -O /tmp/cmake.tar.gz && \
    tar -xzvf /tmp/cmake.tar.gz -C /usr/local --strip-components=1 && \ 
    rm /tmp/cmake.tar.gz

#Change working directory to project direcotry
WORKDIR /usr/src/Odyssey-Game-Server

COPY . .

RUN cmake . && make

CMD ["/bin/bash"]
# CMD ["./Odyssey_Game_Server"]