/*
 * @Author: Hrimfaxi 851957818@qq.com
 * @Date: 2022-06-14 20:23:01
 * @LastEditors: Hrimfaxi 851957818@qq.com
 * @LastEditTime: 2022-06-16 16:09:29
 * @FilePath: /yankewen/code/HttpServer/src/common/EventLoop.cpp
 * @Description:
 *
 * Copyright (c) 2022 by Hrimfaxi 851957818@qq.com, All Rights Reserved.
 */
#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include "utils.h"
#include <vector>

EventLoop::EventLoop() : ep_(nullptr), quit_(false) { 
  ep_ = new Epoll(); 
  int ret = ep_->create();
  FATAL_IF(ret != ALL_OK,"epoll create ret(%d)",ret);
}

EventLoop::~EventLoop() { delete ep_; }

void EventLoop::loop() {
  while (!quit_) {
    std::vector<Channel *> chs;
    int ret = ep_->poll(-1, chs);
    ERR_IF(ret != ALL_OK, "epoll loop ret(%d).", ret);

    for (auto it = chs.begin(); it != chs.end(); ++it) {
      LOG_INFO("start to handle fd(%d)",(*it)->get_fd());
      ret = (*it)->handle_event();
      ERR_IF(ret != ALL_OK, "handle event fd(%d) ret(%d)",(*it)->get_fd(), ret);
    }
  }
}

int EventLoop::update_channel(Channel *ch) { return ep_->update_channel(ch); }