cmake_minimum_required(VERSION 3.10.0)
project(lab4 VERSION 0.1.0 LANGUAGES C CXX)

add_executable(lab4 main.cpp)

add_executable(unnamed_pipes_example unnamed_pipes.cpp)

add_executable(client_fifo named_pipes/client_fifo.cpp)
add_executable(server_fifo named_pipes/server_fifo.cpp)

add_executable(server_mq message_queues/server_msg.cpp)
add_executable(client_mq message_queues/client_msg.cpp)