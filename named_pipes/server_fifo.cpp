#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <algorithm>
#include <errno.h>

int main()
{
    const char* fifo_c2s = "/tmp/fifo_c2s";
    const char* fifo_s2c = "/tmp/fifo_s2c";

    //0666 - права доступа для всех
    if(mkfifo(fifo_c2s, 0666) == -1)
    {
        if (errno != EEXIST) { // если ошибка не в том, что файл уже есть
            perror("cant create fifo_c2s");
            return 1;
        }
    }

    if(mkfifo(fifo_s2c, 0666) == -1)
    {
        if (errno != EEXIST) { 
            perror("cant create fifo_c2s");
            return 1;
        }
    }

    std::cout<<"server is started. wait to client(server will block on open until client off)"<<std::endl;

    // открываем каналы
    int fd_read = open(fifo_c2s, O_RDONLY);
    if (fd_read == -1) {
        perror("cant open pipe for reading");
        return 1;
    }

    int fd_write = open(fifo_s2c, O_WRONLY);
    if (fd_write == -1) {
        perror("cant open pipe for writing");
        close(fd_read);
        return 1;
    }

    char buffer[256];
    while(true)
    {
        memset(buffer, 0, 256);
        //read - блокирующая функция которая ждет данные
        //если ничего не приходит - процесс ждет
        //0 - когда client закрывает канал
        //>0 - кол во байт записанных со стороны клиента в канал
        ssize_t bytes_read = read(fd_read, buffer, sizeof(buffer) - 1);
 
        if (bytes_read > 0) 
        {
            std::string msg(buffer);
            std::cout<<"received message from client: "<<msg<<std::endl;
            std::reverse(msg.begin(), msg.end());
            
            if (write(fd_write, msg.c_str(), msg.length()) == -1) 
            {
                perror("error to send response to client");
                break;
            }
        } 
        else if (bytes_read == 0) 
        {
            // read вернул 0 — значит, клиент закрыл свою сторону канала
            std::cout << "client disappear. end process..." << std::endl;
            break;
        } 
        else 
        {
            perror("reading error");
            break;
        }
    }
    close(fd_read);
    close(fd_write);

    return 0;
}