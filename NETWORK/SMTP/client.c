/* smtp client

### **Algorithm for SMTP Client**  
1. **Initialize socket:**  
   - Create a UDP/TCP socket.  
   - Configure the server address (`IP` & `Port`).  

2. **Send SMTP handshake:**  
   - Send `HELO <client_name>` to the server.  
   - Receive and check the server's response.  

3. **Send sender's email address:**  
   - Send `MAIL FROM: <sender_email>`.  
   - Receive acknowledgment.  

4. **Send recipient’s email address:**  
   - Send `RCPT TO: <receiver_email>`.  
   - Receive acknowledgment.  

5. **Send email body:**  
   - Send `DATA` command.  
   - Receive acknowledgment.  
   - Send the email content line by line.  
   - End with a single `.` on a new line.  

6. **Terminate connection:**  
   - Send `QUIT` command.  
   - Receive final response.  
   - Close the socket.  

Let me know if you need a **TCP-based** version with authentication. 🚀
*/

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLINE 100

int main(int argc, char *argv[]) {
    int n;
    int sock_fd;
    int i = 0;

    struct sockaddr_in servaddr;
    char buf[MAXLINE + 1];

    char address_buf[MAXLINE], message_buf[MAXLINE];
    char *str_ptr, *buf_ptr, *str;

    if (argc != 3) {
        fprintf(stderr, "Command is :./client address port\n");
        exit(1);
    }

    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Cannot create socket\n");
        exit(1);
    }

    bzero((char *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    snprintf(buf, sizeof(buf), "SMTP REQUEST FROM CLIENT\n");
    n = sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (n < 0) {
        perror("ERROR");
        exit(1);
    }

    if ((n = recvfrom(sock_fd, buf, MAXLINE, 0, NULL, NULL)) == -1) {
        perror("UDP read error");
        exit(1);
    }
    buf[n] = '\0';
    printf("S:%s", buf);

    snprintf(buf, sizeof(buf), "HELLO name_of_client_mail_server\n");
    n = sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

    if ((n = recvfrom(sock_fd, buf, MAXLINE, 0, NULL, NULL)) == -1) {
        perror("UDP read error");
        exit(1);
    }
    buf[n] = '\0';
    printf("S:%s", buf);

    printf("please enter the email address of the sender:");
    fgets(address_buf, sizeof(address_buf), stdin);
    address_buf[strlen(address_buf) - 1] = '\0';

    snprintf(buf, sizeof(buf), "MAIL FROM :<%s>\n", address_buf);
    sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

    if ((n = recvfrom(sock_fd, buf, MAXLINE, 0, NULL, NULL)) == -1) {
        perror("UDP read error");
        exit(1);
    }
    buf[n] = '\0';
    printf("S:%s", buf);

    printf("please enter the email address of the receiver:");
    fgets(address_buf, sizeof(address_buf), stdin);
    address_buf[strlen(address_buf) - 1] = '\0';

    snprintf(buf, sizeof(buf), "RCPT TO : <%s>\n", address_buf);
    sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

    if ((n = recvfrom(sock_fd, buf, MAXLINE, 0, NULL, NULL)) == -1) {
        perror("UDP read error");
        exit(1);
    }
    buf[n] = '\0';
    printf("S:%s", buf);

    snprintf(buf, sizeof(buf), "DATA\n");
    sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

    if ((n = recvfrom(sock_fd, buf, MAXLINE, 0, NULL, NULL)) == -1) {
        perror("UDP read error");
        exit(1);
    }
    buf[n] = '\0';
    printf("S:%s", buf);

    do {
        fgets(message_buf, sizeof(message_buf), stdin);
        snprintf(buf, sizeof(buf), "%s", message_buf);
        sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

        message_buf[strlen(message_buf) - 1] = '\0';
        str = message_buf;
        while (isspace(*str++))
            ;

        if (strcmp(--str, ".") == 0)
            break;
    } while (1);

    if ((n = recvfrom(sock_fd, buf, MAXLINE, 0, NULL, NULL)) == -1) {
        perror("UDP read error");
        exit(1);
    }

    buf[n] = '\0';
    snprintf(buf, sizeof(buf), "QUIT\n");
    printf("S:%s", buf);
    sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

    if ((n = recvfrom(sock_fd, buf, MAXLINE, 0, NULL, NULL)) == -1) {
        perror("UDP read error");
        exit(1);
    }

    buf[n] = '\0';
    printf("S:%s", buf);
}

