
#include "sub.h"
#include <sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<errno.h>
#include "keyValueStore.h"

int read_in(int socket_client, char *buf, int len) {
    char *s = buf;
    int slen = len;
    int c = recv(socket_client, s, slen, 0);
    while ((c > 0) && (s[c - 1] != '\n')) {
        s += c;
        slen -= c;
        c = recv(socket_client, s, slen, 0);
    }
    if (c < 0) return c;
    else if (c == 0) buf[0] = '\0';
    else s[c - 1] = '\0';
    return len - slen;
}

void error(char *msg) {
    fprintf(stderr, "%s:%s\n", msg, strerror(errno));
    exit(1);
}

int open_server_socket() {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1) error("Can't opent socket");
    printf("[+] Server Socket created \n");
    return s;
}

int bind_to_port(int socket, int port) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET; //Internet protocol (AF_INET)
    server_addr.sin_port = htons(port); //address port 16 bit
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //Internet address 32bit , INADDR_ANY = any incoming interface
    //--------------------
    int reuse = 1;
    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse, sizeof(int)) == -1)
        error("Can't set the reuse option on the socket");


    int c = bind(socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (c == -1) error("Cannot bind to socket");
    printf("[+] Bind to port :%d\n", ntohs(server_addr.sin_port));
}

//send a string to a socket
int say(int client_socket, char *s) {
    int result = send(client_socket, s, strlen(s), 0);
    if (result == -1)
        fprintf(stderr, "%s:%s\n", "Error talking to client", strerror(errno));
    return result;
}


void trim(char *s) {
    //trim leading
    int count = 0;
    char str[1000];
    while (s[count] == ' ' ||
           s[count] == '\t' ||
           s[count] == '\n' ||
           s[count] == '\r')
        count++;
    if (count != 0) {
        int i = 0;
        while (s[i + count] != '\0') {
            str[i] = s[i + count];
            i++;
        }
        s[i] = '\0';

    }
    //trim tailing
    int i = strlen(s);
    while (i > 0) {
        if (s[i] == '\0' || s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r') i--;
        else break;
    }
    s[i + 1] = '\0';
}

void printMessageToClient(char *com, char *key, char *value, int sockId) {
    char msg[1024];
    bzero(msg, 1024);
    strcat(msg, "Server: ");
    strcat(msg, com);
    strcat(msg, ":");
    strcat(msg, key);
    strcat(msg, ":");
    strcat(msg, value);
    strcat(msg, "\r\n");
    say(sockId, msg);
}

void disconnetFormServer(int sockId) {
    say(sockId, "[-]Disconnected from server\n");
    puts("Client disconnected from Server\n");
    close(sockId);
    exit(0);
}

int printStartingMessage(int sockId) {
    int m1 = say(sockId, "[+]Programm started\r\n");
    int m2 = say(sockId, "[+]Conneced to Server\r\n");
    int m3 = say(sockId, "[+]To exit from Programm use exit command\r\n");
    int m4 = say(sockId, "[+]Client....\r\n");
    if (m1 != -1 && m2 != -1 && m3 != -1 && m4 != -1) return 1;
    return -1;
}

void performPUT(char *key, char *value, int sockId) {
    if (key != NULL && value != NULL) {
        int res = put(key, value);
        if (res == -1) {
            //put with override
            strcat(value, ":overrided");
        }
        printMessageToClient("PUT", key, value, sockId);
        say(sockId, "\n");
    } else
        say(sockId, "command not exists1\r\n");
}

void performDEL(char *key, int sockId) {
    if (key != NULL) {
        if (del(key) == 1) {
            //key exists
            printMessageToClient("DEL", key, "deleted\r\n", sockId);
        } else say(sockId, "key not exists2\r\n");

    } else say(sockId, "command not exists3\r\n");
}

void performGET(char *key, int sockId) {
    if (key != NULL) {
        char v[100];
        get(key, v);
        printMessageToClient("GET", key, v, sockId);
        say(sockId, "\n");
    } else say(sockId, "command not exists3\r\n");
}


void communicate(int new_client_socket) {
    int m = printStartingMessage(new_client_socket);
    if (m != -1) {
        //Read data from the client
        while (1) {
            char buf[2048];
            read_in(new_client_socket, buf, sizeof(buf));
            char dl[] = " ";
            strtok(buf, dl);
            char *key = strtok(NULL, dl);
            strcpy(dl, "\0");
            char *value = strtok(NULL, dl);
            if (key != NULL) trim(key);
            if (value != NULL) trim(value);
            if (strncasecmp(buf, "put", 3) == 0) {
                performPUT(key, value, new_client_socket);
            } else if (strncasecmp(buf, "del", 3) == 0) {
                performDEL(key, new_client_socket);

            } else if (strncasecmp(buf, "get", 3) == 0) {
                performGET(key, new_client_socket);

            } else if (strncasecmp(buf, "exit", 4) == 0) {
                disconnetFormServer(new_client_socket);

            } else {
                say(new_client_socket, "command not exists5\r\n");
            }
        }
    }
}