/* tcp server 

### **TCP Server - Short Algorithm**  
1. **Create Socket:** Use `socket(AF_INET, SOCK_STREAM, 0)`.  
2. **Bind to Address:** Use `bind()` to associate with a port.  
3. **Listen for Connections:** Use `listen()`.  
4. **Accept Connection:** Use `accept()` for incoming clients.  
5. **Receive Data:** Use `recv()` to read client messages.  
6. **Close Connection:** Use `close()`.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int sock_desc, temp, k, len;
    struct sockaddr_in server, client;
    char buff[100];

    sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_desc == -1) {
        printf("error in socket creation\n");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(3005);

    k = bind(sock_desc, (struct sockaddr*) &server, sizeof(server));
    if (k == -1) {
        printf("error in binding\n");
        return 1;
    }

    k = listen(sock_desc, 1);
    if (k == -1) {
        printf("error in listening\n");
        return 1;
    }

    len = sizeof(client);
    temp = accept(sock_desc, (struct sockaddr*) &client, &len);
    if (temp == -1) {
        printf("error in temporary socket creation\n");
        return 1;
    }

    k = recv(temp, buff, sizeof(buff), 0);
    if (k == -1) {
        printf("error in receiving\n");
        return 1;
    }

    printf("message got from client is: %s\n", buff);
    close(temp);
    close(sock_desc);

    return 0;
}