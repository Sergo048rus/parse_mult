cmake_minimum_required(VERSION 3.19)
project(task2)

set(CMAKE_CXX_STANDARD 17)

set(Boost_INCLUDE_DIR C:/MinGW/include/)
set(Boost_LIBRARY_DIR C:/MinGW/lib)
find_package(Boost COMPONENTS system filesystem REQUIRED)
#include_directories(${Boost_INCLUDE_DIR})

#set(CMAKE_C_FLAGS "-static-libgcc -w -lpthread -lws2_32 -fopenmp -lwsock32")
if(Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})

endif()

#-lwsock32
add_executable(task2 main.cpp request.cpp request.h global_func.h obj_parse_c.cpp obj_parse_c.h global_func.cpp)

if(MINGW)
    target_link_libraries(task2 ws2_32)
endif()


if(Boost_FOUND)

    target_link_libraries(task2 ${Boost_LIBRARIES})

endif()