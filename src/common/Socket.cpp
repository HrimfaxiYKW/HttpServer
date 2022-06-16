/*
 * @Author: Hrimfaxi 851957818@qq.com
 * @Date: 2022-06-14 20:11:14
 * @LastEditors: Hrimfaxi 851957818@qq.com
 * @LastEditTime: 2022-06-16 16:07:03
 * @FilePath: /yankewen/code/HttpServer/src/common/Socket.cpp
 * @Description: Class Socket
 *
 * Copyright (c) 2022 by Hrimfaxi 851957818@qq.com, All Rights Reserved.
 */
#include "Socket.h"
#include "InetAddress.h"
#include "../common/utils.h"
#include <unistd.h>
#include <sys/socket.h>

Socket::Socket() : fd_(-1) {}

Socket::~Socket() {
  LOG_INFO("close sockfd(%d).", fd_);
  if (fd_ >= 0) {
    // close(fd_);
  }
}

int Socket::create(bool create_new, const int fd) {
  fd_ = create_new ? socket(AF_INET, SOCK_STREAM, 0) : fd;
  if (fd_ < 0) {
    LOG_ERR("creat sockfd(%d).", fd_);
    return SOCKET_CREATE_ERROR;
  }

  LOG_INFO("create sockfd(%d).", fd_);
  return ALL_OK;
}

int Socket::bind(const InetAddress &inet_addr) {
  LOG_INFO("star bind");
  int ret = ::bind(fd_, reinterpret_cast<const sockaddr *>(&(inet_addr.addr_)), inet_addr.addr_len_);

  const auto &addr = inet_addr.addr_;
  if (ret == -1) {
    LOG_ERR("sockfd(%d) bind ip(%s) port(%d).", fd_, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    return SOCKET_BIND_ERROR;
  }

  LOG_INFO("sockfd(%d) bind ip(%s) port(%d).", fd_, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
  return ALL_OK;
}

int Socket::listen() {
  if (::listen(fd_, SOMAXCONN) == -1) {
    LOG_ERR("sockfd(%d) listen.", fd_);
    return SOCKET_LISTEN_ERROR;
  }

  LOG_INFO("sockfd(%d) listen.", fd_);
  return ALL_OK;
}

int Socket::accept(const InetAddress &inet_addr, int &clnt_sockfd) {
  clnt_sockfd = ::accept(fd_, const_cast<sockaddr *>(reinterpret_cast<const sockaddr *>(&inet_addr.addr_)),
                         const_cast<socklen_t *>(&inet_addr.addr_len_));

  const auto &addr = inet_addr.addr_;
  if (clnt_sockfd < 0) {
    LOG_ERR("sockfd(%d) accept ip(%s) port(%d).", fd_, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    return SOCKET_ACCEPT_ERROR;
  }

  LOG_INFO("sockfd(%d) accept ip(%s) port(%d).", fd_, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
  return ALL_OK;
}
