cmake_minimum_required(VERSION 3.17)
project(Progr_darbs)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_HOST_WIN32)
    set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
endif ()

add_executable(Vinnijs_puks_dobumi vinnijs_puks_gr16014.cpp)
