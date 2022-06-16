/*
 * @Author: Hrimfaxi 851957818@qq.com
 * @Date: 2022-06-14 20:09:55
 * @LastEditors: Hrimfaxi 851957818@qq.com
 * @LastEditTime: 2022-06-16 14:32:26
 * @FilePath: /yankewen/code/HttpServer/src/common/Epoll.h
 * @Description: class Epoll
 *
 * Copyright (c) 2022 by Hrimfaxi 851957818@qq.com, All Rights Reserved.
 */
#pragma once
#include <sys/epoll.h>
#include <vector>

class Channel;
class Epoll {
 public:
  Epoll();
  ~Epoll();

  // 新建 epfd
  int create();

  // 新增监听一个 fd
  int add_fd(const int fd, const uint32_t op);

  // 获取活跃的 channel
  int poll(const int timeout, std::vector<Channel *> &active_channel);

  // 将一个 channel 注册到 epoll 中
  int update_channel(Channel *channel);

 private:
  int epfd_;
  epoll_event *events_;
};