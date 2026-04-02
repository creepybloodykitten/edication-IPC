#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    const char* fifo_c2s = "/tmp/fifo_c2s";
    const char* fifo_s2c = "/tmp/fifo_s2c";


    // открываем каналы
    int fd_write = open(fifo_c2s, O_WRONLY);
    if (fd_write == -1) {
        perror("cant open pipe for writing");
        return 1;
    }

    int fd_read= open(fifo_s2c, O_RDONLY);
    if (fd_read == -1) {
        perror("cant open pipe for reading");
        close(fd_read);
        return 1;
    }

    char buffer[256];
    std::string input;
    while (true) {
        std::cout << "input: ";
        std::getline(std::cin, input);

        write(fd_write, input.c_str(), input.length() + 1);//как и read() - блокирующая функция

        memset(buffer, 0, 256);
        read(fd_read, buffer, 255);
        std::cout << "delivered! response from server: " << buffer <<std::endl;
    }
    return 0;
}