#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {

    char buf[200];
    strcpy(buf , "put key1 value1\n");
    char d[] = " ";

    int count = 0;
    char *portion = strtok(buf, d);
    while (portion != NULL) {
        count = count + 1;
        printf("%s\n", portion);
        portion = strtok(NULL, d);
    }

    printf("COunt = %d\n", count);


    return 0;

}
