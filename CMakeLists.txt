cmake_minimum_required(VERSION 4.0)
project(DTStructProject C)

set(CMAKE_C_STANDARD 11)

add_executable(DTStructProject
    main.c
    queue.c
    slist.c
    stack.c
    rules.c
    station_index.c
    nary.c
    events.c
    csv_loader.c
    json_loader.c
)
