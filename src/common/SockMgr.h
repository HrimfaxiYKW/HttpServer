/*
 * @Author: Hrimfaxi 851957818@qq.com
 * @Date: 2022-06-22 16:12:39
 * @LastEditors: Hrimfaxi 851957818@qq.com
 * @LastEditTime: 2022-06-22 16:35:31
 * @FilePath: /yankewen/code/HttpServer/src/common/SockMgr.h
 * @Description: 管理 socket 的生命周期
 *
 * Copyright (c) 2022 by Hrimfaxi 851957818@qq.com, All Rights Reserved.
 */
#pragma once
#include "Socket.h"
#include <unistd.h>
#include <map>
#include "utils.h"

class SockMgr {
 public:
  SockMgr() {}
  ~SockMgr() {
    for (const auto &it : socks_) {
      if (it.second != nullptr) delete it.second;
    }

    socks_.clear();
    LOG_INFO("clear all socket");
  }

  void add_socket(int fd, Socket *sock) {
    auto it = socks_.find(fd);
    if (it == socks_.end()) {
      socks_[fd] = sock;
    } else {
      erase_socket(fd);
      socks_[fd] = sock;
    }
  }

  void erase_socket(int fd) {
    auto it = socks_.find(fd);
    if (it != socks_.end()) {
      if (it->second != nullptr) delete it->second;
      socks_.erase(it);
    }
  }

 private:
  std::map<int, Socket *> socks_;
};