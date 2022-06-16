/*
 * @Author: Hrimfaxi 851957818@qq.com
 * @Date: 2022-06-14 20:08:21
 * @LastEditors: Hrimfaxi 851957818@qq.com
 * @LastEditTime: 2022-06-16 14:33:23
 * @FilePath: /yankewen/code/HttpServer/src/common/Channel.h
 * @Description: 一个 Channel 实例监听一个 fd, 用 callback_ 进行处理
 *
 * Copyright (c) 2022 by Hrimfaxi 851957818@qq.com, All Rights Reserved.
 */
#pragma once
#include <sys/epoll.h>
#include <functional>
class EventLoop;
class Channel {
 private:
  EventLoop *loop_;
  int fd_;
  uint32_t events_;
  // epoll_wait 的返回 events
  uint32_t revents_;
  // 判断当前 channel 的 fd 是否在 epoll 中
  bool in_epoll_;
  // 对应到 handle_event(), 该函数在 Server 实例中被绑定
  std::function<int()> callback_;

 public:
  Channel(EventLoop *loop, int fd);
  ~Channel();

  // 处理 fd 上的事件
  int handle_event();
  // 将该 channel 添加到 loop 中被监听
  int enable_reading();

  int get_fd();
  uint32_t get_events();
  uint32_t get_revents();
  bool get_in_epoll();
  void set_in_epoll();

  void set_revents(uint32_t);
  void set_callback(std::function<int()>);
};