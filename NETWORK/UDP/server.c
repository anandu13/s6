/* udp server

### **UDP Server - Short Algorithm**  
1. **Create Socket:** Use `socket(AF_INET, SOCK_DGRAM, 0)`.  
2. **Configure Server Address:** Set `sin_family`, `sin_port`, and `sin_addr`.  
3. **Bind Socket:** Use `bind()` to associate the socket with an address.  
4. **Receive Data:** Use `recvfrom()` to get messages from the client.  
5. **Send Response:** Use `sendto()` to echo messages back.  
6. **Repeat Until Exit:** Keep listening for messages.  
7. **Close Socket:** Use `close()`.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to the server address
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP Server is listening on port %d...\n", PORT);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        
        // Receive data from client
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, MSG_WAITALL, 
                         (struct sockaddr *)&client_addr, &addr_len);
        buffer[n] = '\0';

        printf("Client: %s\n", buffer);

        // Send response to client
        sendto(sockfd, buffer, strlen(buffer), MSG_CONFIRM, 
               (const struct sockaddr *)&client_addr, addr_len);
        printf("Message sent back to client\n");
    }

    close(sockfd);
    return 0;
}
