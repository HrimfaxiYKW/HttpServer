/*****************************************************************************
 * Copyright (c) 2022, Yan Kewen. All rights reserved.
 *
 * @file Epoll.h
 *
 * Define class Epoll
 *
 * Created by Yan Kewen (ykwhrimfaxi@gmail.com), 2022/06/10.
 *
 * Managed by Yan Kewen
 *****************************************************************************/
#pragma once
#include <sys/epoll.h>
#include <vector>

class Epoll {
 public:
  Epoll();
  ~Epoll();

  // 新建 epfd
  int create();

  // 新增监听一个 fd
  int add_fd(const int fd, const uint32_t op);

  // 获取 active event
  int poll(const int timeout, std::vector<epoll_event> &active_vec);

 private:
  int epfd_;
  // events 数组
  epoll_event *events_;
};