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

int main() {
  using namespace utils;
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  defer(close(sockfd));
  FATAL_IF_WITH_CLOSE(sockfd == -1, sockfd, "socket create fail");

  struct sockaddr_in serv_addr;
  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(8848);

  FATAL_IF_WITH_CLOSE(bind(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)) == -1, sockfd,
                      "sockfd(%d) bind port(%d) fail.", sockfd, static_cast<int32_t>(serv_addr.sin_port));

  FATAL_IF_WITH_CLOSE(listen(sockfd, MAX_EVENTS) == -1, sockfd, "sockfd(%d) listen fali.", sockfd);

  int epfd = epoll_create1(0);
  FATAL_IF_WITH_CLOSE(epfd == -1, sockfd, "epoll create error.");

  epoll_event events[MAX_EVENTS];  // 等待队列
  epoll_event ev;                  // 监听事件
  bzero(&events, sizeof(events));
  bzero(&ev, sizeof(ev));

  ev.data.fd = sockfd;
  ev.events = EPOLLIN | EPOLLET;  // ET 模式
  utils::setnonblocking(sockfd);  // ET 模式必须是非阻塞io
  epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

  while (true) {
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);  // wait unlimit time
    FATAL_IF_WITH_CLOSE(nfds == -1, sockfd, "epoll wait fail.");

    for (int i = 0; i < nfds; ++i) {
      if (events[i].data.fd == sockfd) {  // 处理新连接
        sockaddr_in clnt_addr;
        bzero(&clnt_addr, sizeof(clnt_addr));
        socklen_t clnt_addr_len = sizeof(clnt_addr);

        int clnt_sockfd = accept(sockfd, (sockaddr *)&clnt_addr, &clnt_addr_len);
        ERR_IF_WITH_CLOSE(clnt_sockfd == -1, sockfd, "socket accept error.");
        LOG_INFO("accept new client fd(%d), IP:%s, Port:%d.", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr),
                 ntohs(clnt_addr.sin_port));

        bzero(&ev, sizeof(ev));
        ev.data.fd = clnt_sockfd;
        ev.events = EPOLLIN;
        setnonblocking(clnt_sockfd);  // 重要，必须设置成非阻塞 io
        epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sockfd, &ev);
      } else if (events[i].events & EPOLLIN) {  // 可读事件
        char buf[BUFFER_SIZE];

        while (true) {
          bzero(&buf, sizeof(buf));
          ssize_t bytes_read = read(events[i].data.fd, buf, sizeof(buf));
          if (bytes_read > 0) {
            LOG_INFO("msg from client fd(%d):%s", events[i].data.fd, buf);
            ERR_IF(write(events[i].data.fd, buf, sizeof(buf)) < 0, "write back msg(%s) to fd(%d) fali", buf,
                   events[i].data.fd);
            // write 失败只打日志，始终通过读判断连接的正常性
          } else if (bytes_read == -1 && errno == EINTR) {  // 客户端正常中断，继续读
            LOG_INFO("continue reading.");
          } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {  // 读取完毕
            LOG_INFO("read complete.");
            break;
          }

          INFO_IF_WITH_CLOSE(bytes_read == 0, events[i].data.fd, "EOF, client fd(%d) disconnected.",
                             events[i].data.fd);  // EOF 断开了
        }
      } else {
        LOG_INFO("unknown events");
      }
    }
  }

  return 0;
}