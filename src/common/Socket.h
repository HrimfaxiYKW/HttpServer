/*****************************************************************************
 * Copyright (c) 2022, Yan Kewen. All rights reserved.
 *
 * @file Socket.h
 *
 * Define class Socket
 *
 * Created by Yan Kewen (ykwhrimfaxi@gmail.com), 2022/06/10.
 *
 * Managed by Yan Kewen
 *****************************************************************************/
#pragma once
#include <fcntl.h>

class InetAddress;
class Socket {
 public:
  Socket();
  ~Socket();

  int create(bool create_new = true, const int fd = -1);
  int bind(const InetAddress &inet_addr);
  int listen();
  int accept(const InetAddress &inet_addr, int &clnt_sockfd);

  inline void setnonblocking() { fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL) | O_NONBLOCK); }

  inline int get_fd() { return fd_; }

 private:
  int fd_;
};