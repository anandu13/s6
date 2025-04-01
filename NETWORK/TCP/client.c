/* tcp client

### **TCP Client - Short Algorithm**  
1. **Create Socket:** Use `socket(AF_INET, SOCK_STREAM, 0)`.  
2. **Initialize Address:** Set up `sockaddr_in` with IP and port.  
3. **Connect to Server:** Use `connect()`.  
4. **Send Data:** Read input and send it using `send()`.  
5. **Close Connection:** Use `close()`.*/*/


#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main() {
    char buf[100];
    int k, sock_desc;
    struct sockaddr_in client;

    sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_desc == -1) {
        printf("Socket creation failed...\n");
        return 1;
    }

    client.sin_family = AF_INET;
    client.sin_addr.s_addr = INADDR_ANY;
    client.sin_port = htons(3005);

    k = connect(sock_desc, (struct sockaddr *)&client, sizeof(client));

    if (k == -1) {
        printf("Connection failed...\n");
        close(sock_desc);
        return 1;
    }

    printf("Enter data to be sent\n");
    fgets(buf, sizeof(buf), stdin);
    k = send(sock_desc, buf, strlen(buf), 0);
    
    close(sock_desc);
    return 0;
}