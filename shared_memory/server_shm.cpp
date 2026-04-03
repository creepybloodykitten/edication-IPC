/*
существует в системном программировании 2 стандарта для работы с разделяемой памятью 
- POSIX IPC (более новый)
- System V IPC
И shmat(System V), и mmap(POSIX) делают под капотом одно и то же - 
отображают физическую оперативную память в виртуальное пространство процесса. 
POSIX использует futex(Fast Userspace Mutex) в отличии от System V который делает системные вызовы к ядру
В данном случае используется стандарт POSIX
*/

#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <algorithm>

int main() {
    // создаем общие семафоры(они хранятся в ram и обращаемся к ним через дескрипторы)
    sem_t* sem_c2s = sem_open("/sem_c2s", O_CREAT, 0666, 0); // ждет сервер
    sem_t* sem_s2c = sem_open("/sem_s2c", O_CREAT, 0666, 0); // ждет клиент

    // создание общей области разделяемой памяти 
    int shm_fd = shm_open("/my_shm", O_CREAT | O_RDWR, 0666);
    //задаем размер этой разделяемой памяти
    ftruncate(shm_fd, 256);

    //системный вызов который превращает файловый дескриптор в указатель char* то есть
    //отображает файл или область памяти устройства прямо в виртуальное адресное пространство процесса. 
    //это позволяет работать с данными файла как с обычным массивом в оперативной памяти (через указатели), минуя медленные системные вызовы read() и write()
    char* shared_mem = (char*)mmap(0, 256, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    std::cout << "shared memory server is activated.."<<std::endl;
    while (true) {
        sem_wait(sem_c2s); // ждем пока клиент положит данные в память

        std::string str(shared_mem);
        std::cout<<"received message from client: "<<str<<std::endl;
        std::reverse(str.begin(), str.end());
        strcpy(shared_mem, str.c_str()); //обновляем данные в общей памяти

        sem_post(sem_s2c); // разблокировка клиента
    }
    return 0;
}