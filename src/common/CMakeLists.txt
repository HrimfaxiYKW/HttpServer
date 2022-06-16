aux_source_directory(. ALL_SRC)
add_library(libcommon STATIC ${ALL_SRC})

#set(UTILS_LIB_SRC utils.cpp)
#add_library(libutils ${UTILS_LIB_SRC})

#set(INET_LIB_SRC InetAddress.cpp)
#add_library(libinet ${INET_LIB_SRC})

#set(SOCKET_LIB_SRC Socket.cpp)
#add_library(libsocket ${SOCKET_LIB_SRC})

#set(EPOLL_LIB_SRC Epoll.cpp)
#add_library(libepoll ${EPOLL_LIB_SRC})

#set(CHANNEL_LIB_SRC Channel.cpp)
#add_library(libchannel ${CHANNEL_LIB_SRC})

#set(EVENTLOOP_LIB_SRC EventLoop.cpp)
#add_library(libeventloop ${EVENTLOOP_LIB_SRC})

#set(SERVER_LIB_SRC Server.cpp)
#add_library(libserver ${SERVER_LIB_SRC})