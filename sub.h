//
// Created by abhei on 06.04.2022.
//

#ifndef SUB_H
#define SUB_H


int read_in(int, char *, int);

int open_server_socket();

int bind_to_port(int, int);

int say(int, char *);

void error(char *);

void trim(char *);

void performGET(char *, int);

void performDEL(char *, int);

void performPUT(char *, char *, int);

void disconnetFormServer(int);

int printStartingMessage(int);

void printMessageToClient(char *, char *, char *, int);

void communicate(int);


#endif
