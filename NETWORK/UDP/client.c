/*udp client
### **UDP Client - Short Algorithm**  
1. **Create Socket:** Use `socket(AF_INET, SOCK_DGRAM, 0)`.  
2. **Configure Server Address:** Set `sin_family`, `sin_port`, and `sin_addr`.  
3. **Send Data:** Use `sendto()` to transmit messages.  
4. **Receive Response:** Use `recvfrom()` to get server replies.  
5. **Repeat Until Exit:** Continue sending and receiving messages.  
6. **Close Socket:** Use `close()`.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    while (1) {
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';  // Remove newline

        // Send message to server
        sendto(sockfd, buffer, strlen(buffer), MSG_CONFIRM, 
               (const struct sockaddr *)&server_addr, addr_len);
        
        // Receive response from server
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, MSG_WAITALL, 
                         (struct sockaddr *)&server_addr, &addr_len);
        buffer[n] = '\0';

        printf("Server: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
