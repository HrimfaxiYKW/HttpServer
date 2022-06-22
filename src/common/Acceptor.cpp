/*
 * @Author: Hrimfaxi 851957818@qq.com
 * @Date: 2022-06-22 15:19:02
 * @LastEditors: Hrimfaxi 851957818@qq.com
 * @LastEditTime: 2022-06-22 16:42:04
 * @FilePath: /yankewen/code/HttpServer/src/common/Acceptor.cpp
 * @Description:
 *
 * Copyright (c) 2022 by Hrimfaxi 851957818@qq.com, All Rights Reserved.
 */
#include "Acceptor.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Server.h"
#include "utils.h"
#include "SockMgr.h"

Acceptor::Acceptor(EventLoop *loop) { loop_ = loop; }

int Acceptor::init(SockMgr *socket_mgr) {
  // 这里不建议用 fatal, 以后可改为反复尝试（要注意内存泄漏）
  LOG_INFO("start init acceptor");
  int ret = ALL_OK;

  sock_ = new Socket();
  FATAL_IF(sock_->create(socket_mgr, true) != ALL_OK, "create sock");

  addr_ = new InetAddress("127.0.0.1", 8848);
  FATAL_IF(sock_->bind(*addr_) != ALL_OK, "bind sock");
  FATAL_IF(sock_->listen() != ALL_OK, "listen sock");
  sock_->setnonblocking();
  accept_channel_ = new Channel(loop_, sock_->get_fd());
  auto cb = std::bind(&Acceptor::accept_connection, this);
  accept_channel_->set_callback(cb);
  FATAL_IF(accept_channel_->enable_reading() != ALL_OK, "enable reading fd(%d) ret(%d)", sock_->get_fd(), ret);

  return ret;
}

Acceptor::~Acceptor() {
  delete sock_;
  delete addr_;
  delete accept_channel_;
}

int Acceptor::accept_connection() { return new_connection_callback_(*sock_); }

void Acceptor::set_new_connection_callback(std::function<int(Socket &)> cb) { new_connection_callback_ = cb; }