/*
очередь сообщений хранится внутри ядра ОС(RAM) 
с помощью системных вызовов(msgsnd) происходит специальное прерывание в kernel space
и оно записывает сообщение в ram
считывание происходит с использованием системного вызовов(msgrcv) 
по нужным правам uid и специальному ключу
*/

#include <iostream>
#include <sys/msg.h>
#include <string.h>
#include <algorithm>
#include "msg_common.h"

int main() {
    key_t key = ftok("/tmp/server_fifo", 'A'); //генерация общего ключа для клиента и сервера
    int msgid = msgget(key, 0666 | IPC_CREAT); //создание очереди в ядре ОС, где другие прооцессы по ключу можно добраться до этой очереди
    //IPC_CREAT - если очереди с таким ключом нет - создаем

    MsgBuf msg;

    std::cout << "message queues server is activated.."<<std::endl;
    while (true) {
        //ожидание сообщения в очереди, процесс засыпает пока не появится сообщение
        //достаем сообщение с типом 1 из этой очереди и кладем в структуру msg
        //0 - поведение по умолчанию(блокирующее чтение). если данных нет — ждем
        msgrcv(msgid, &msg, sizeof(msg.mtext), 1, 0);

        std::string str(msg.mtext);
        std::cout<<"received message from client: "<<msg.mtext<<std::endl;
        std::reverse(str.begin(), str.end());

        // отправляем ответ с типом 2
        msg.mtype = 2;
        strcpy(msg.mtext, str.c_str()); //из строки в массив char
        msgsnd(msgid, &msg, sizeof(msg.mtext), 0);//отправка сообщения в очередь 
    }
    return 0;
}