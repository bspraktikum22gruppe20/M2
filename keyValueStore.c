#include <string.h>
#include <sys/shm.h>

struct entry {
    char key[500];
    char value[500];
} entry;
int pointer = 0;
int numberOfentries;

struct entry *data;

/**
 *
 * @param length
 * @return gibt id von shared memory zurück
 */

int createData(int length) {
    //                     create/attach shared memory
    int shID = shmget(IPC_PRIVATE, length * sizeof(struct entry), IPC_CREAT | 0777);
    if (shID < 0) error("shared memory request error\n");
    data = shmat(shID, NULL, 0);
    if (data < 0) error("attach error\n");
    numberOfentries = length;
    return shID;
}

void detachData() {
    int res = shmdt(data);
    if (res < 0) error("detach error\n");
}

void sharedMemoryDelete(int shID) {
    int res = shmctl(shID, IPC_RMID, 0);
    if (res < 0) error("shared memory remove error");
}

/**
 *
 * @param key
 * @param value
 * @return -1 if key and value already exist and we override them
 *         1  if key and value does't exist and we insert them in array
 */
int put(char *key, char *value) {
    int len = numberOfentries;
    char k[500];
    strcpy(k, key);
    trim(k);
    char v[500];
    strcpy(v, value);
    trim(v);

    //check if the key already exist
    for (int i = 0; i < len; i++) {
        if ((strcmp(data[i].key, key) == 0)) {
            strcpy(data[i].key, key);
            strcpy(data[i].value, value);
            return -1;
        }
    }
    strcpy(data[pointer].key, key);
    strcpy(data[pointer].value, value);
    pointer++;
}

/**
 *
 * @param key
 * @param res
 * @return   1 if key eixits  -1 if key dose not exists
 */
int get(char *key, char *res) {
    char k[30];
    strcpy(k, key);
    trim(key);
    int len = numberOfentries;
    for (int i = 0; i < len; i++) {
        if (strcmp(data[i].key, k) == 0) {
            strcpy(res, data[i].value);
            return 1;
        }
    }
    strcpy(res, "key not exists");
    return -1;
}

/**
 *
 * @param key
 * @return 1 if key and value is deleted -1 if key dose not exists
 */
int del(char *key) {
    //1-key exists
    int len = numberOfentries;
    for (int i = 0; i < len; i++) {
        if (strcmp(data[i].key, key) == 0) {
            //key was found so we have to delete it together with value
            for (int j = i; j < len; j++) {
                strcpy(data[j].key, data[j + 1].key);
                strcpy(data[j].value, data[j + 1].value);
            }
            pointer--;
            return 1;
        }
    }
    //2 keynot exists
    return -1;
}
