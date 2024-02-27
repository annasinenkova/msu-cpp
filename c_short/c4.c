#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>

struct Message {
    long mtype;
    int num;
};

void
filling_field(int field[5][5])
{
    int pos;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            field[i][j] = 0;
        }
    }
    for (int i = 0; i < 4; ++i) {
        printf("enter position\n");
        scanf("%d", &pos);
        ++field[pos / 10][pos % 10];
    }
    return;
}

int
main(void)
{
    int count = 4;
    int field[5][5];
    filling_field(field);
    key_t key = ftok("file", 5);
    if (key == -1) {
        perror("key");
        _exit(1);
    }
    int msgid = msgget(key, 0);
    if (msgid == -1) {
        perror("msgget");
        _exit(1);
    }
    struct Message msg;
    msg.mtype = 3;
    msgsnd(msgid, &msg, sizeof(msg.num), 0);
    while (count > 0) {
        msgrcv(msgid, &msg, sizeof(msg.num), 1, 0);
        if (msg.num == -1) {
            break;
        }
        if (field[msg.num / 10][msg.num % 10]) {
            --field[msg.num / 10][msg.num % 10];
            --count;
            if (count == 0) {
                break;
            }
        }
        printf("your turn\n");
        scanf("%d", &msg.num);
        msg.mtype = 2;
        msgsnd(msgid, &msg, sizeof(msg.num), 0);
    }
    if (msg.num == -1) {
        printf("Game over! You win!\n");
    }
    else {
        printf("Game over! You lose!\n");
    }
    msg.num = -1;
    msg.mtype = 2;
    msgsnd(msgid, &msg, sizeof(msg.num), 0);
    return 0;
}

