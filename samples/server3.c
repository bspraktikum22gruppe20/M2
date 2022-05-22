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
//
//    int sock = open_server_socket();
//    //=============================================================================
//    //                                 Bind to  socket
//    //=============================================================================
//    bind_to_port(sock, 5678);
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
//    while (1) {
//        char *msg = msgs[rand() % 6];
//        struct sockaddr_storage client_addr;// store details about the client who's just connected
//        unsigned int address_size = sizeof(client_addr);
//        int connect_d = accept(sock, (struct sockaddr *) &client_addr,
//                               &address_size);//creates descriptor for a new socket
//        if (connect_d == -1) error("Can't open secondary socket\n");
//
//        //Sending message
//        say(connect_d,"Hello Das ist neue Socket");
//        close(connect_d);
//
//    }
//
//    return 0;
//}
