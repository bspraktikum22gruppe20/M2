#include <sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "sub.h"
#include "keyValueStore.c"


void handle_shutdown(int server_socket) {
    if (server_socket)
        close(server_socket);
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Bye Bye ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    exit(0);
}

int main() {
    signal(SIGINT, handle_shutdown);
    int numOfClient = 0;
    //=============================================================================
    //                                  Create socket
    //=============================================================================
    int server_socket = open_server_socket();
    //=============================================================================
    //                                 Bind to  socket
    //=============================================================================
    bind_to_port(server_socket, 5678);

    //=============================================================================
    //                     listening to incoming connection
    //=============================================================================
    if (listen(server_socket, 10) == -1) error("Can't listen");

    //create an array of struct with 100 element
    int shID = createData(100);
    //=============================================================================
    //                     accept new connection
    //=============================================================================

    struct sockaddr_in client_addr;// store details about the client who's just connected
    unsigned int address_size = sizeof(client_addr);
    printf("[+]Escape character ^C\n");
    printf("[+]Waiting for Connection...\n");
    printf("=============================================\n");
    while (1) {
        //listen for a connection
        int new_client_socket = accept(server_socket, (struct sockaddr *) &client_addr,
                                       &address_size);//creates descriptor for a new client socket
        if (new_client_socket == -1) error("Can't open client socket");
        numOfClient++;
        char addr[] = "";
        strcpy(addr, inet_ntoa(client_addr.sin_addr));
        int port = ntohs(client_addr.sin_port);
        printf("===============Client Nr.%d =================\n", numOfClient);
        printf("Connection accepted from %s:%d\n", addr, port);
        printf("=============================================\n");
        //communication with ctelnet
        int pid;
        if ((pid = fork()) == 0) {
            //close(server_socket);
            //=============================================================================
            //                     Client = child process
            //=============================================================================
            communicate(new_client_socket);
            close(new_client_socket);
            detachData();
            exit(0);
        }
        close(new_client_socket);
        //waitpid(pid, NULL, 0);
    }
    close(server_socket);
    sharedMemoryDelete(shID);

}


