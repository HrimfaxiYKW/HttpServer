/*****************************************************************************
 * Copyright (c) 2022, Yan Kewen. All rights reserved.
 *
 * @file server.cpp
 *
 * Created by Yan Kewen (ykwhrimfaxi@gmail.com), 2022/06/08.
 *
 * Managed by Yan Kewen
 *****************************************************************************/
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>
#include "../common/utils.h"
#include "../common/def.h"
#include "../common/Epoll.h"
#include "../common/InetAddress.h"
#include "../common/Socket.h"

static int handle_read_event(const int fd) {  // process ET & nonblocking
  int ret = ALL_OK;
  char buf[BUFFER_SIZE];
  while (true) {
    bzero(&buf, sizeof(buf));
    ssize_t bytes_read = read(fd, buf, sizeof(buf));
    if (bytes_read > 0) {
      LOG_INFO("msg from client fd(%d):%s", fd, buf);
      ERR_IF(write(fd, buf, sizeof(buf)) < 0, "write back msg(%s) to fd(%d) fali", buf, fd);
      // write 失败只打日志，始终通过读判断连接的正常性
    } else if (bytes_read == -1 && errno == EINTR) {  // 客户端正常中断，继续读
      LOG_INFO("continue reading.");
    } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {  // 读取完毕
      LOG_INFO("read complete.");
      break;
    } else if (bytes_read == 0) {
      LOG_INFO("EOF, client fd(%d) disconnected.", fd);  // EOF 断开了
      break;
    } else {
      LOG_FATAL("unknonw condition bytes_read(%d).", static_cast<int>(bytes_read));
    }
  }

  return ret;
}

int main() {
  using namespace utils;
  int ret = ALL_OK;

  InetAddress *serv_addr = new InetAddress("127.0.0.1", 8848);
  defer(delete serv_addr);

  Socket *serv_sock = new Socket();
  defer(delete serv_sock);
  ret = serv_sock->create();
  FATAL_IF(ret != ALL_OK, "create ret(%d).", ret);

  ret = serv_sock->bind(*serv_addr);
  FATAL_IF(ret != ALL_OK, "bind ret(%d).", ret);

  ret = serv_sock->listen();
  FATAL_IF(ret != ALL_OK, "listen ret(%d)", ret);

  serv_sock->setnonblocking();  // 采用 ET, 所以非阻塞

  Epoll *ep = new Epoll();

  ret = ep->create();
  FATAL_IF(ret != ALL_OK, "epoll create ret(%d).", ret);

  ep->add_fd(serv_sock->get_fd(), EPOLLIN | EPOLLET);

  LOG_INFO("Server Init success.");

  while (true) {
    std::vector<epoll_event> active_events;
    ret = ep->poll(-1, active_events);
    FATAL_IF(ret != ALL_OK, "get active events ret(%d).", ret);

    for (const auto &event : active_events) {
      if (event.data.fd == serv_sock->get_fd()) {  // 新连接
        InetAddress *clnt_addr = new InetAddress();
        defer(delete clnt_addr);
        Socket *clnt_sock = new Socket();
        // defer(delete clnt_sock);
        // 这里析构会导致 close，从而等于没建立连接。但不析构会造成内存泄漏，需要开发一个 Socket mgr 统一管理每个 Socket
        // 的生命周期

        ret = clnt_sock->create(false, [&]() -> int {
          int acc_fd = 0;
          ret = serv_sock->accept(*clnt_addr, acc_fd);
          return acc_fd;
        }());

        if (ret != ALL_OK) {
          LOG_ERR("new client fd %d! IP: %s Port: %d", clnt_sock->get_fd(), inet_ntoa(clnt_addr->addr_.sin_addr),
                  ntohs(clnt_addr->addr_.sin_port));
          continue;
        }

        LOG_INFO("new client fd(%d)! IP:(%s) Port:(%d)", clnt_sock->get_fd(), inet_ntoa(clnt_addr->addr_.sin_addr),
                 ntohs(clnt_addr->addr_.sin_port));

        clnt_sock->setnonblocking();

        ret = ep->add_fd(clnt_sock->get_fd(), EPOLLIN | EPOLLET);
        ERR_IF(ret != ALL_OK, "epoll add fd ret(%d).", ret);
      } else if (event.events & EPOLLIN) {  // 可读
        ret = handle_read_event(event.data.fd);
        ERR_IF(ret != ALL_OK, "handle read evetn ret(%d).", ret);
      } else {
        LOG_INFO("unknown condition.");
      }
    }
  }

  return 0;
}