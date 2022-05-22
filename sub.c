
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


int tokenize(char *str, char *p1, char *p2, char *p3) {
    int count = 0;
    char *s = str;
    if (*s != '\0') count++;

    while (*s != ' ' && *s != '\0') {
        *p1 = *s;
        s++;
        p1++;
    }
    *p1 = '\0';

    if (*s != '\0') {
        count++;
        s++;
    }
    while (*s != ' ' && *s != '\0') {
        *p2 = *s;
        p2++;
        s++;
    }
    *p2 = '\0';

    if (*s != '\0') {
        s++;
        count++;
    }
    while (*s != '\0') {
        *p3 = *s;
        p3++;
        s++;
    }
    *p3 = '\0';
    trim(p1);
    trim(p2);
    trim(p3);
    return count;

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

void performDelete(char *p0, char *p1,  int sockId) {
    //delete successful
    if (del(p1) == 1) { //geändert
        char msg[1024] = "Server: ";
        strcat(msg, p0);
        strcat(msg, ":");
        strcat(msg, p1);
        strcat(msg, ":");
        strcat(msg, "deleted\r\n");
        say(sockId, msg);
    } else {//delete unsuccessful
        char msg[1024] = "Server: ";
        strcat(msg, p0);
        strcat(msg, ":");
        strcat(msg, p1);
        strcat(msg, ":");
        strcat(msg, "key not exists\r\n");
        say(sockId, msg);

    }
    //=====================end of del===========================
}

void performGet(char *p0, char *p1, int sockId) {
    //=====================GET==================================
    char value[100];
    int res = get(p1, value);
    if (res == 1) {
        //get successful
        char msg[1024] = "Server: ";

        strcat(msg, p0);
        strcat(msg, ":");
        strcat(msg, p1);
        strcat(msg, ":");
        strcat(msg, value);
        strcat(msg, "\r\n");
        say(sockId, msg);//to client

    } else {//get unsuccessful
        char msg[1024] = "Server: ";
        strcat(msg, p0);
        strcat(msg, ":");
        strcat(msg, p1);
        strcat(msg, ":");
        strcat(msg, "key not exists");
        strcat(msg, "\r\n");
        say(sockId, msg);
    }
    //=====================end of GET===========================
}

void performPut(char *p0, char *p1, char *p2, int sockId) {
    int res = put(p1, p2);
    //put without override
    if (res == 1) {
        char msg[1024] = "Server: ";
        strcat(msg, p0);
        strcat(msg, ":");
        strcat(msg, p1);
        strcat(msg, ":");
        strcat(msg, p2);
        strcat(msg, "\r\n");
        say(sockId, msg);
    } else {//put with override
        char msg[1024];
        bzero(msg, 1024);
        strcat(msg, "Server: ");
        strcat(msg, p0);
        strcat(msg, ":");
        strcat(msg, p1);
        strcat(msg, ":");
        strcat(msg, p2);
        strcat(msg, "\r\n");
        say(sockId, msg);
    }
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


void communicate(int new_client_socket) {
    int m = printStartingMessage(new_client_socket);
    if (m != -1) {
        //Read data from the client
        while (1) {
            char buf[2048];
            char p0[100];
            char p1[100];
            char p2[100];
            read_in(new_client_socket, buf, sizeof(buf));
            int count = tokenize(buf, p0, p1, p2);
            /**
             * GET key1 ------->count = 2
             * PUT key1 value1-------->count = 3
             * DEL key1----->count = 2;
             */
            trim(p0);
            trim(p1);
            trim(p2);
            if (count == 0) {
                say(new_client_socket, "command not exists1\r\n");

            } else if (count == 1) {
                //if client write exit => exit from programm
                if (strcasecmp(p0, "exit") == 0) {
                    disconnetFormServer(new_client_socket);
                } else {
                    say(new_client_socket, "command not exists1\r\n");
                }
            } else if (count == 2) {

                //=====================DEL===========================
                if (strcasecmp(p0, "DEL") == 0) {
                    performDelete(p0, p1, new_client_socket);
                }
                    //=====================GET===========================
                else if (strcasecmp(p0, "GET") == 0) {
                    performGet(p0, p1, new_client_socket);

                } else {
                    char server[] = "Server: ";
                    strcat(server, "command not exists\r\n");
                    say(new_client_socket, server);
                }
            }
                //=====================PUT===========================
            else if (count == 3) {
                if (strcasecmp(p0, "PUT") == 0) {
                    performPut(p0, p1, p2, new_client_socket);
                } else {
                    char server[] = "Server: ";
                    strcat(server, buf);
                    strcat(server, "command not exists\r\n");
                    say(new_client_socket, server);
                }
            } else {
                say(new_client_socket, "command not exists5\r\n");
            }
        }
    }
}