/*
 * @Author: Hrimfaxi 851957818@qq.com
 * @Date: 2022-06-16 14:10:54
 * @LastEditors: Hrimfaxi 851957818@qq.com
 * @LastEditTime: 2022-06-16 16:02:26
 * @FilePath: /yankewen/code/HttpServer/src/server/main.cpp
 * @Description: 
 * 
 * Copyright (c) 2022 by Hrimfaxi 851957818@qq.com, All Rights Reserved. 
 */
#include "../common/EventLoop.h"
#include "../common/Server.h"
#include "../common/utils.h"
#include <unistd.h>

int main() {
    EventLoop *loop = new EventLoop();
    Server *server = new Server(loop);
    FATAL_IF(server->init() != ALL_OK,"init server.");
    LOG_INFO("server init success, start loop!");    
    loop->loop();
    return 0;
}