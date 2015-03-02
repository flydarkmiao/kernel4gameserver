#cmake file for project kernel
#author:T
#created:2015-01-12 15:07:04

SET(CMAKE_CXX_FLAGS "-g -std=c++0x -Wall -lpthread -rdynamic -ldl -Dlinux -D_DEBUG")
SET(CMAKE_CXX_FLAGS_DEBUG "-g -std=c++0x -lpthread -rdynamic -ldl -Dlinux -D_DEBUG")
SET(CMAKE_CXX_FLAGS_RELEASE "-g -std=c++0x -lpthread -rdynamic -ldl -Dlinux")
MESSAGE(STATUS "Linux")

INCLUDE_DIRECTORIES(
        "./src/Public"
        "./src/API"
        "./src/interface"
        "./src/epoller"
        )

SET(SOURCE_FILES
        "./src/Public/MultiSys.cc"
        "./src/Public/CThread.cpp"
        "./src/epoller/ITcpServer.cpp"
        "./src/epoller/epoll_waiter.cpp"
        "./src/epoller/epoll_worker.cpp"
        "./src/epoller/epoller.cpp"
        "./src/logicmgr/Logicmgr.cpp"
        "./src/main.cpp"
   )

SET(EXECUTABLE_OUTPUT_PATH "./bin")

ADD_EXECUTABLE(kernel
        ${SOURCE_FILES}
        )

TARGET_LINK_LIBRARIES(kernel ${STATIC_LIBRARYS})
