/*
 * @Author: Hrimfaxi 851957818@qq.com
 * @Date: 2022-06-14 20:11:28
 * @LastEditors: Hrimfaxi 851957818@qq.com
 * @LastEditTime: 2022-06-16 16:45:03
 * @FilePath: /yankewen/code/HttpServer/src/common/Socket.h
 * @Description: Class Socket
 *
 * Copyright (c) 2022 by Hrimfaxi 851957818@qq.com, All Rights Reserved.
 */

#pragma once
#include <fcntl.h>
#include <map>

class InetAddress;
class Socket {
 public:
  Socket();
  ~Socket();

  int create(std::map<int, Socket*>& sock_mgr_, bool create_new = true, const int fd = -1);
  int bind(const InetAddress &inet_addr);
  int listen();
  int accept(const InetAddress &inet_addr, int &clnt_sockfd);

  inline void setnonblocking() { fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL) | O_NONBLOCK); }

  inline int get_fd() { return fd_; }

 private:
  int fd_;
};