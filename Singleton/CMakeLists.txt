cmake_minimum_required(VERSION 3.5)

project(shapes)

add_executable(the_executable
    main.cpp
    impl.cpp
    foo.cpp
    bar.cpp
)

add_library(demo SHARED
    lib.cpp
    foo.cpp
)
