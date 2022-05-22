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

int tokenize(char *,  char *, char *, char *);

void trim(char *);

void performDelete(char *, char *, int);

void performGet(char *, char *, int);

void performPut(char *, char *, char *, int);

void disconnetFormServer(int);

int printStartingMessage(int);

void communicate(int);


#endif
