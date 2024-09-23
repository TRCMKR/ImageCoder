cmake_minimum_required(VERSION 3.26)
project(ImageCoder)

set(CMAKE_CXX_STANDARD 17)


FIND_PACKAGE(X11 REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

include_directories(
        src/
        lab1/
        lab2/
        lab3/
        lab4/
        /Users/trcmkr/Личное/Coding/C++/
        /usr/local/include/
)

add_executable(ImageCoder main.cpp
        CImg-3.3.5/CImg.h

        lab1/Codings.h
        lab2/Cyphers.h

        src/BST.h
        src/Nodes.h
        src/suppFuncs.h
        lab3/Coder.h
        src/suppClasses.h
        src/labFuncs.h
        lab4/Steganography.h
)

target_link_libraries(ImageCoder
        ${X11_LIBRARIES}
        /Users/trcmkr/Личное/Coding/C++/ntl.a
        /opt/homebrew/Cellar/gmp/6.3.0/lib/libgmp.a
        /opt/homebrew/Cellar/gmp/6.3.0/lib/libgmpxx.a
)

if(APPLE)
    include_directories(AFTER "/opt/X11/include")
endif()