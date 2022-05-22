//#include <arpa/inet.h>
//#include<stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <unistd.h>
//
//int main() {
//    char *ip = "127.0.0.1";
//    int port = 50000;
//
//    struct sockaddr_in addr;
//    socklen_t addr_size;
//    char buffer[1024];
//    int sock = socket(AF_INET,SOCK_STREAM,0);
//    if (sock==-1){
//        perror("Socket Error\n");
//        exit(1);
//    }
//    printf("Socket created\n");
//    memset(&addr,'\0',sizeof(addr));
//    addr.sin_family = AF_INET;
//    addr.sin_port = port;
//    addr.sin_addr.s_addr = inet_addr(ip);
//
//    //connect to the server
//    int con = connect(sock,(struct sockaddr *)&addr,sizeof(addr));
//    if (con<0){
//        perror("Connecting Error...\n");
//        exit(1);
//    }
//    printf("Connected to the server\n");
//
//    bzero(buffer,1024);
//    strcpy(buffer,"Hello, das ist Client\n");
//    printf("Client : %s \n",buffer);
//    //sending message
//    send(sock,buffer,strlen(buffer),0);
//
//    bzero(buffer,1024);
//    recv(sock,buffer,sizeof(buffer),0);
//    printf("Server : %s\n",buffer);
//
//    close(sock);
//    printf("Disconneced from Serever \n");
//
//
//    return 0;
//}