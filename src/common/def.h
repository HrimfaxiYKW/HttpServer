/*
 * @Author: Hrimfaxi 851957818@qq.com
 * @Date: 2022-06-14 20:09:09
 * @LastEditors: Hrimfaxi 851957818@qq.com
 * @LastEditTime: 2022-06-16 12:39:03
 * @FilePath: /yankewen/code/HttpServer/src/common/def.h
 * @Description: 宏定义
 *
 * Copyright (c) 2022 by Hrimfaxi 851957818@qq.com, All Rights Reserved.
 */
#define BUFFER_SIZE 1024
#define RETRY_READ_TIME_INTERVAL_S 2
#define MAX_EVENTS 32

#define ALL_OK 0

#define SOCKET_CREATE_ERROR -1001
#define SOCKET_BIND_ERROR -1002
#define SOCKET_LISTEN_ERROR -1003
#define SOCKET_ACCEPT_ERROR -1004

#define EPOLL_CREATE_ERROR -2001
#define EPOLL_ADDFD_ERROR -2002
#define EPOLL_WAIT_ERROR -2003

#define UPDATE_CHANNEL_ERROR -3001

#define SERVER_HANDLE_EVENT_READ_ERROR -4001