/*
 * @Author: Hrimfaxi 851957818@qq.com
 * @Date: 2022-06-14 20:20:36
 * @LastEditors: Hrimfaxi 851957818@qq.com
 * @LastEditTime: 2022-06-16 16:06:27
 * @FilePath: /yankewen/code/HttpServer/src/common/Channel.cpp
 * @Description:
 *
 * Copyright (c) 2022 by Hrimfaxi 851957818@qq.com, All Rights Reserved.
 */
#include "Channel.h"
#include "EventLoop.h"

Channel::Channel(EventLoop *loop, int fd) : loop_(loop), fd_(fd), events_(0), revents_(0), in_epoll_(false) {}

Channel::~Channel() {}

int Channel::handle_event() { return callback_(); }

int Channel::enable_reading() {
  events_ = EPOLLIN | EPOLLET;
  return loop_->update_channel(this);
}

int Channel::get_fd() { return fd_; }

uint32_t Channel::get_events() { return events_; }
uint32_t Channel::get_revents() { return revents_; }

bool Channel::get_in_epoll() { return in_epoll_; }

void Channel::set_in_epoll() { in_epoll_ = true; }

void Channel::set_revents(uint32_t ev) { revents_ = ev; }

void Channel::set_callback(std::function<int()> cb) { callback_ = cb; }