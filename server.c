//server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


struct dhcp_packet {
    uint8_t op;
    uint32_t ciaddr;
    uint32_t yiaddr;
    uint32_t fromIPAddr;
    uint32_t toIPAddr;
    uint16_t transID;
};

int main(int argc, char *argv[]) {


    int port = atoi(argv[1]);

    char *ip_address = argv[2];

    struct sockaddr_in server_addr;
    int sockfd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if ( sockfd == -1) {
        printf("socket error");
        exit(EXIT_FAILURE);
    }

    int enable_braodcast = -1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &enable_braodcast, sizeof(enable_braodcast)) == -1) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    //bind socket to port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Accept any incoming interface
    server_addr.sin_port = htons(port);

    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    //listening on port
    printf("Server listening on port %d...\n", port);
    
    struct dhcp_packet packet;

    if (recvfrom(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&client_addr, &client_len) == -1) {
        perror("recvfrom failed");
        exit(EXIT_FAILURE);
    }

    if (packet.op == 1 && packet.fromIPAddr == 0 && packet.toIPAddr == htonl(0xFFFFFFFF)) {
        // Send offer back to client
        packet.op = 2;
	packet.yiaddr = 0;
        packet.fromIPAddr = inet_addr(ip_address);
    
        sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&client_addr, client_len);

    }
    else {
        printf("Received invalid request from client.\n");
    }

    printf("DHCP packet is :\n");

    printf("Op code: %d\n", packet.op);
    printf("ciaddr: %u\n", packet.ciaddr);
    printf("yiaddr: %s\n",inet_ntoa(*(struct in_addr*)&packet.yiaddr));
    printf("fromIPAddr: %s\n", inet_ntoa(*(struct in_addr*)&packet.fromIPAddr));
    printf("toIPAddr: %s\n", inet_ntoa(*(struct in_addr*)&packet.toIPAddr));
    printf("transID: %u\n", packet.transID);   
    close(sockfd);
    return 0;
}
