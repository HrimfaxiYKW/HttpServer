/*****************************************************************************
 * Copyright (c) 2022, Yan Kewen. All rights reserved.
 *
 * @file Epoll.cpp
 *
 * Define class Epoll
 *
 * Created by Yan Kewen (ykwhrimfaxi@gmail.com), 2022/06/10.
 *
 * Managed by Yan Kewen
 *****************************************************************************/
#include "Epoll.h"
#include "def.h"
#include "utils.h"
#include <unistd.h>
#include <cstring>

Epoll::Epoll() : epfd_(-1), events_(nullptr) {}

Epoll::~Epoll() {
  if (epfd_ != -1) {
    close(epfd_);
    epfd_ = -1;
  }

  delete[] events_;
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

int Epoll::poll(const int timeout, std::vector<epoll_event> &active_events) {
  int nfds = epoll_wait(epfd_, events_, MAX_EVENTS, timeout);

  if (nfds == -1) {
    LOG_ERR("epoll wait.");
    return EPOLL_WAIT_ERROR;
  }

  for (int i = 0; i < nfds; ++i) {
    active_events.push_back(events_[i]);
  }

  LOG_INFO("epoll wait nfds(%d).", nfds);
  return ALL_OK;
}