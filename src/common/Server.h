/*
 * @Author: Hrimfaxi 851957818@qq.com
 * @Date: 2022-06-16 12:34:08
 * @LastEditors: Hrimfaxi 851957818@qq.com
 * @LastEditTime: 2022-06-16 16:34:29
 * @FilePath: /yankewen/code/HttpServer/src/common/Server.h
 * @Description: 
 * 
 * Copyright (c) 2022 by Hrimfaxi 851957818@qq.com, All Rights Reserved. 
 */
#pragma once
#include <map>

class EventLoop;
class Socket;
class Server
{
private:
    EventLoop *loop_;
    // pair<fd, Socket*>
    std::map<int, Socket*> sock_mgr_;
public:
    Server(EventLoop* loop);
    ~Server();

    int init();
    // 处理可读事件，绑定到 clnt_channel, 对应到 clnt_fd
    int handle_read_event(const int fd);
    // 新客户端连接事件, 绑定到 server_channel, 对应到 serv_fd
    int new_connection(Socket& serv_sock);
};