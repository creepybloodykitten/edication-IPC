#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

int main() {
    //даем доступ клиенту к созданным семафорам и общей памяти
    sem_t* sem_c2s = sem_open("/sem_c2s", 0);
    sem_t* sem_s2c = sem_open("/sem_s2c", 0);

    int shm_fd = shm_open("/my_shm", O_RDWR, 0666);
    char* shared_mem = (char*)mmap(0, 256, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    
    std::string input;
    while (true) {
        std::cout << "input: ";
        std::getline(std::cin, input);

        strcpy(shared_mem, input.c_str()); // кладем строку в память
        
        sem_post(sem_c2s); // разблокировка сервера
        sem_wait(sem_s2c); // ждем пока сервер не ответит

        std::cout << "delivered! response from server: " << shared_mem <<std::endl;
    }
    return 0;
}