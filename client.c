//client 
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

    int server_port = atoi(argv[1]);

    int sockfd;
    struct sockaddr_in server_addr;
    struct dhcp_packet packet;
    socklen_t server_len = sizeof(server_addr);

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Enable broadcast
    int broadcast_enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) == -1) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }


    packet.op = 1;
    packet.ciaddr = 0;
    packet.yiaddr = 0;
    packet.fromIPAddr = htonl(0);
    packet.toIPAddr = htonl(0xFFFFFFFF);
    packet.transID = (uint16_t)rand(); // Generate random transaction ID




    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("255.255.255.255"); // Broadcast address
    server_addr.sin_port = htons(server_port);

    // Send request to server
    sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&server_addr, server_len);
    

    printf("Request sent to server.\n");

    // Receive reply from server
    recvfrom(sockfd, &packet, sizeof(packet), 0, NULL, NULL);


    // Validate reply packet
    if (packet.op == 2 && packet.toIPAddr == htonl(0xFFFFFFFF)) {
        printf("Received offer from server:\n");
        printf("Op code: %d\n", packet.op);
        printf("ciaddr: %u\n", packet.ciaddr);
        printf("yiaddr: %s\n",inet_ntoa(*(struct in_addr*)&packet.yiaddr));
        printf("fromIPAddr: %s\n", inet_ntoa(*(struct in_addr*)&packet.fromIPAddr));
        printf("toIPAddr: %s\n", inet_ntoa(*(struct in_addr*)&packet.toIPAddr));
        printf("transID: %u\n", packet.transID);
    } else {
        printf("Invalid offer received from server.\n");
    }

    close(sockfd);
    return 0;
}
