/*
 * @Author: Hrimfaxi 851957818@qq.com
 * @Date: 2022-06-22 15:17:19
 * @LastEditors: Hrimfaxi 851957818@qq.com
 * @LastEditTime: 2022-06-22 16:45:36
 * @FilePath: /yankewen/code/HttpServer/src/common/Acceptor.h
 * @Description:专门用于事件分发的类，处理新连接
 *
 * Copyright (c) 2022 by Hrimfaxi 851957818@qq.com, All Rights Reserved.
 */
#pragma once
#include <functional>
#include <map>

class EventLoop;
class Socket;
class InetAddress;
class Channel;
class SockMgr;
class Acceptor {
 private:
  EventLoop *loop_;
  Socket *sock_;
  InetAddress *addr_;
  Channel *accept_channel_;
  std::function<int(Socket &)> new_connection_callback_;

 public:
  Acceptor(EventLoop *loop);
  ~Acceptor();

  // 新建一个 channel 并且添加到 epoll 监听队列中， 该 channel 仅处理新连接请求
  int init(SockMgr *socket_mgr);
  int accept_connection();
  void set_new_connection_callback(std::function<int(Socket &)>);
};