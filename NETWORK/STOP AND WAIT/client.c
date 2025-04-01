/* stop and wait sliding window protocol*

### **Stop-and-Wait Protocol (Sender) - Short Algorithm**  
1. **Initialize:** Create a UDP socket and configure the server address.  
2. **Send Frame:** If the last frame was acknowledged, input data, assign a sequence number, and send it.  
3. **Wait for ACK:** Receive an acknowledgment from the receiver.  
4. **Verify ACK:** If ACK is correct, proceed to the next frame; else, retransmit.  
5. **Repeat Until Done:** Continue sending and receiving ACKs.  
6. **Close Connection:** Once all frames are sent, close the socket.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

typedef struct packet{
    char data[1024];
}Packet;

typedef struct frame{
    int frame_kind; //ACK:0, SEQ:1 FIN:2
    int sq_no;
    int ack;
    Packet packet;
}Frame;

int main(int argc, char *argv[]){
    // Port set to 9009 directly
    int port = 9009;
    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[1024];
    socklen_t addr_size;

    int frame_id = 0;
    Frame frame_send;
    Frame frame_recv;
    int ack_recv = 1;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    while(1){
        
        if(ack_recv == 1){
            frame_send.sq_no = frame_id;
            frame_send.frame_kind = 1;
            frame_send.ack = 0;        

            printf("Enter Data: ");
            scanf("%s", buffer);
            strcpy(frame_send.packet.data, buffer);
            sendto(sockfd, &frame_send, sizeof(Frame), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
            printf("[+]Frame Send\n");
        }
        int addr_size = sizeof(serverAddr);
        int f_recv_size = recvfrom(sockfd, &frame_recv, sizeof(frame_recv), 0 ,(struct sockaddr*)&serverAddr, &addr_size);
        
        if( f_recv_size > 0 && frame_recv.sq_no == 0 && frame_recv.ack == frame_id+1){
            printf("[+]Ack Received\n");
            ack_recv = 1;
        }else{
            printf("[-]Ack Not Received\n");
            ack_recv = 0;
        }    
              
        frame_id++;        
    }
    
    close(sockfd);
    return 0;
}

