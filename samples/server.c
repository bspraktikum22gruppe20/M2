//#include <arpa/inet.h>
//#include<stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <unistd.h>
//
//int main(){
//    char *ip = "127.0.0.1";
//    int port = 5678;
//
//
//    int server_socket,client_socket;
//    struct sockaddr_in addr,client_addr;
//    socklen_t addr_size;
//    char buffer[1024];
//
//    server_socket = socket(AF_INET, SOCK_STREAM, 0);
//    if (server_socket == -1){
//        perror("Socket Error\n");
//        exit(1);
//    }
//    printf("Socket created\n");
//
//    memset(&addr,'\0',sizeof(addr));
//    addr.sin_family = AF_INET;
//    addr.sin_port = port;
//    addr.sin_addr.s_addr = inet_addr(ip);
//
//    //bind address with port number
//    int n = bind(server_socket, (struct sockaddr*)&addr, sizeof(addr));
//    if(n<0){
//        perror("Bind Error\n");
//        exit(1);
//    }
//    printf("Bind to the port number: %d\n",port);
//
//    //listen to client
//    listen(server_socket, 5);
//    printf("Listening...\n");
//    //server waiting for client
//    while (1){
//        //accept connection from client
//        addr_size = sizeof(client_addr);
//        client_socket = accept(server_socket, (struct sockaddr*) &client_addr, &addr_size);
//        printf("Client connected\n");
//
//        bzero(buffer,1024);//clean buffer
//        recv(client_socket,buffer,sizeof(buffer),0);
//        printf("Client : %s\n",buffer);
//
//        bzero(buffer,1024);
//        strcpy(buffer,"Hi This is server \n");
//        printf("Server : %s\n",buffer);
//        send(client_socket,buffer,strlen(buffer),0);
//        close(client_socket);
//        printf("Client disconnected\n");
//
//
//    }
//
//    return 0;
//}