set(UTILS_LIB_SRC utils.cpp)
add_library(libutils ${UTILS_LIB_SRC})

set(INET_LIB_SRC InetAddress.cpp)
add_library(libinet ${INET_LIB_SRC})

set(SOCKET_LIB_SRC Socket.cpp)
add_library(libsocket ${SOCKET_LIB_SRC})

set(EPOLL_LIB_SRC Epoll.cpp)
add_library(libepoll ${EPOLL_LIB_SRC})