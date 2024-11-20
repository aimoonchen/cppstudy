cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(MyApp VERSION 1.0.0 LANGUAGES CXX)

add_subdirectory(Coroutine)
add_subdirectory(Singleton)