//#include <sys/socket.h>
//#include<arpa/inet.h>
//#include<string.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include "sub.h"
//
//
//int main() {
//    //=============================================================================
//    //                                  Create socket
//    //=============================================================================
//    /***
//     * PF_INET : protocol family for TCP/IP protocol
//     * SOCK_STREAM : Stream sockets(TCP): reliable byte-stream service
//     * protocol : socket protocol
//     */
//    int sock = socket(PF_INET, SOCK_STREAM, 0);
//    if (sock == -1) error("Cannot open socket\n");
//    //=============================================================================
//    //                                 Bind to  socket
//    //=============================================================================
//    //bind need two things: 1- socket descriptor 2-address
//    struct sockaddr_in server_addr;
//    server_addr.sin_family = PF_INET; //Internet protocol (AF_INET)
//    server_addr.sin_port = htons(5678); //address port 16 bit
//    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //Internet address 32bit , INADDR_ANY = any incoming interface
//    int c = bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
//    if (c == -1) error("Cannot bind to socket\n");
//
//    //=============================================================================
//    //                     listening to incoming connection
//    //=============================================================================
//    if (listen(sock, 10) < 0) error("Can't listen\n");
//    puts("Waiting for Connection....\n");
//    //=============================================================================
//    //                     accept new connection
//    //=============================================================================
//
//    char *msgs[] = {"Hi This is socket",
//                    "Das ist socket programmierung",
//                    "Hello World",
//                    "Wie geht es dir",
//                    "Was machst du so ",
//                    "How are you"
//    };
//
//    while (1){
//        char *msg = msgs[rand()%6];
//        struct sockaddr_storage client_addr;// store details about the client who's just connected
//        unsigned int address_size = sizeof(client_addr);
//        int connect_d = accept(sock, (struct sockaddr *) &client_addr, &address_size);//creates descriptor for a new socket
//        if (connect_d == -1) error("Can't open secondary socket\n");
//
//        //Sending message
//        if (send(connect_d,msg,strlen(msg),0)==-1) error("Can't send message");
//        close(connect_d);
//
//    }
//
//    return 0;
//}
//
//void error(char *msg) {
//
//}
