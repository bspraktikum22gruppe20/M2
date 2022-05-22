//
// Created by abhei on 06.04.2022.
//

#ifndef KEYVALUESTORE_H
#define KEYVALUESTORE_H
struct entry;

struct entry *getData(int);

int put(char *key, char *value);

int get(char *key, char *res);

int del(char *key);

void detachData();

void sharedMemoryDelete(int);

#endif
