#include <iostream>
#include <unistd.h>
#include <string.h>
#include <algorithm>

int main()
{   
    //fd[0] на чтение, fd[1] на запись
    int fd_c2s[2]; // клиент -> сервер
    int fd_s2c[2]; // сервер -> клиент

    //два канала для двухсторонней связи
    pipe(fd_c2s); 
    pipe(fd_s2c);
    std::cout << "client and sever pipes created"<<std::endl;

    //создаем новый дочерний процесс c помощью fork() и получаем его pid 
    //(pid<0) - ошибка создания
    //pid==0 - id дочернего процесса
    //pid >0 - id родительского процесса
    pid_t pid = fork(); 

    if(pid<0)
    {
        std::cout<<"error: cant create child process"<<std::endl;
    }
    else if(pid==0) //блок кода который будет выполнятся дочерним процессом(клиент)
    {
        close(fd_c2s[0]); // клиент только пишет серверу
        close(fd_s2c[1]); // клиент только читает от сервера
        char buffer[256];
        std::string input;
        while(true)
        {
            std::cout << "[client] enter a line: ";
            std::getline(std::cin, input);
            write(fd_c2s[1],input.c_str(), input.length() + 1);
            memset(buffer, 0, 256);
            read(fd_s2c[0], buffer, 255);
            std::cout << "[client] delivered! response from server: " << buffer <<std::endl;
        }
    }
    else //блок кода который будет выполнятся родительским процессом(сервер)
    {
        close(fd_c2s[1]); // сервер только читает от клиента
        close(fd_s2c[0]); // сервер только пишет клиенту
        char buffer[256];
        while(true)
        {
            memset(buffer, 0, 256);
            int bytes = read(fd_c2s[0], buffer, 255);
            std::string msg(buffer);
            std::cout<<"received message from client: "<<msg<<std::endl;
            std::reverse(msg.begin(), msg.end());
            write(fd_s2c[1], msg.c_str(), msg.length() + 1);
        }
    }

    return 0;

}