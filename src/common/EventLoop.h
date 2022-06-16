/*
 * @Author: Hrimfaxi 851957818@qq.com
 * @Date: 2022-06-14 20:22:40
 * @LastEditors: Hrimfaxi 851957818@qq.com
 * @LastEditTime: 2022-06-16 11:47:15
 * @FilePath: /yankewen/code/HttpServer/src/common/EventLoop.h
 * @Description: 封装一个 EventLoop 类，来实现对 epoll 的循环访问
 *
 * Copyright (c) 2022 by Hrimfaxi 851957818@qq.com, All Rights Reserved.
 */
#pragma once
class Epoll;
class Channel;
class EventLoop {
 private:
  Epoll *ep_;
  bool quit_;

 public:
  EventLoop();
  ~EventLoop();

  void loop();
  int update_channel(Channel *);
};