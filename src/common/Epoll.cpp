/*
 * @Author: Hrimfaxi 851957818@qq.com
 * @Date: 2022-06-14 20:09:33
 * @LastEditors: Hrimfaxi 851957818@qq.com
 * @LastEditTime: 2022-06-22 14:54:41
 * @FilePath: /yankewen/code/HttpServer/src/common/Epoll.cpp
 * @Description: class Epoll
 *
 * Copyright (c) 2022 by Hrimfaxi 851957818@qq.com, All Rights Reserved.
 */
#include "Epoll.h"
#include "def.h"
#include "utils.h"
#include "Channel.h"
#include <unistd.h>
#include <cstring>

Epoll::Epoll() : epfd_(-1), events_(nullptr) {}

Epoll::~Epoll() {
  if (epfd_ != -1) {
    close(epfd_);
    epfd_ = -1;
  }

  delete[] events_;
  LOG_INFO("close epfd(%d)", epfd_);
}

int Epoll::create() {
  epfd_ = epoll_create1(0);

  if (epfd_ == -1) {
    LOG_ERR("epoll create.");
    return EPOLL_CREATE_ERROR;
  }

  events_ = new epoll_event[MAX_EVENTS];
  bzero(events_, sizeof(*events_) * MAX_EVENTS);
  LOG_INFO("epfd(%d) created.", epfd_);

  return ALL_OK;
}

int Epoll::add_fd(const int fd, const uint32_t op) {
  epoll_event ev;
  bzero(&ev, sizeof(ev));
  ev.data.fd = fd;
  ev.events = op;

  if (epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) == -1) {
    LOG_ERR("epoll add event(fd=%d, op=%u).", fd, op);
    return EPOLL_ADDFD_ERROR;
  }

  LOG_INFO("epoll add event(fd=%d, op=%u).", fd, op);
  return ALL_OK;
}

int Epoll::poll(const int timeout, std::vector<Channel *> &active_channel) {
  int nfds = epoll_wait(epfd_, events_, MAX_EVENTS, timeout);

  if (nfds == -1) {
    LOG_ERR("epoll wait.");
    return EPOLL_WAIT_ERROR;
  }

  for (int i = 0; i < nfds; ++i) {
    // 将 epoll wait 返回的 events[i] 封装成一个 Channel 对象
    Channel *ch = reinterpret_cast<Channel *>(events_[i].data.ptr);
    ch->set_revents(events_[i].events);
    active_channel.push_back(ch);
  }

  LOG_INFO("epoll wait nfds(%d).", nfds);
  return ALL_OK;
}

int Epoll::update_channel(Channel *channel) {
  int ret = ALL_OK;
  int fd = channel->get_fd();
  struct epoll_event ev;
  bzero(&ev, sizeof(ev));
  ev.data.ptr = channel;
  ev.events = channel->get_events();

  if (!channel->get_in_epoll()) {
    ret = epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev);
    if (ret == -1) {
      LOG_ERR("epoll add epfd(%d) fd(%d).", epfd_, fd);
      return UPDATE_CHANNEL_ERROR;
    }

    channel->set_in_epoll();
    LOG_INFO("add Channel fd(%d) to epoll epfd(%d) tree success", fd, epfd_);
  } else {
    ret = epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev);
    if (ret == -1) {
      LOG_ERR("epoll(%d) modify fd(%d).", epfd_, fd);
      return UPDATE_CHANNEL_ERROR;
    }
  }

  return ret;
}