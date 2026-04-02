#include <iostream>
#include <sys/msg.h>
#include <string.h>
#include "msg_common.h"

int main() {
    key_t key = ftok("/tmp/server_fifo", 'A'); //генерация общего ключа для клиента и сервера
    int msgid = msgget(key, 0666 | IPC_CREAT);
    MsgBuf msg;
    std::string input;

    while (true) {
        std::cout << "input: ";
        std::getline(std::cin, input);

        msg.mtype = 1; // тип 1 - для сервера
        strcpy(msg.mtext, input.c_str());
        msgsnd(msgid, &msg, sizeof(msg.mtext), 0); //отправляем в очередь сообщение

        // ждем сообщение с типом 2 (от сервера)
        msgrcv(msgid, &msg, sizeof(msg.mtext), 2, 0);
        std::cout << "delivered! response from server:: " << msg.mtext << std::endl;
    }
    return 0;
}