# Odyssey-Game-Server

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


