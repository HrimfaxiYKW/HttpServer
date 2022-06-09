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
#include <cstring>
#include <iostream>
#include <unistd.h>
#include "../common/utils.h"
#include "../common/def.h"

int main() {
  using namespace utils;
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  defer(close(sockfd));
  FATAL_IF(sockfd == -1, "socket create.");

  struct sockaddr_in serv_addr;
  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(8848);

  FATAL_IF(bind(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)) == -1, "bind fail.");

  // 最大监听队列长度，默认 128
  FATAL_IF(listen(sockfd, SOMAXCONN) == -1, "listen fail.");

  struct sockaddr_in clnt_addr;
  socklen_t clnt_addr_len = sizeof(clnt_addr);
  bzero(&clnt_addr, sizeof(clnt_addr));

  // accept 是阻塞调用
  int clnt_sockfd = accept(sockfd, (sockaddr *)&clnt_addr, &clnt_addr_len);
  FATAL_IF(clnt_sockfd == -1, "socket(%d) accept.", sockfd);

  LOG_INFO("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr),
           ntohs(clnt_addr.sin_port));
  printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

  while (true) {
    char buf[BUFFER_SIZE];
    bzero(&buf, sizeof(buf));
    auto read_bytes = read(clnt_sockfd, buf, sizeof(buf));

    if (read_bytes > 0) {
      LOG_INFO("message from client sockfd(%d) :%s\n ", clnt_sockfd, buf);
      printf("message from client sockfd(%d) :%s\n", clnt_sockfd, buf);
      auto write_bytes = write(clnt_sockfd, buf, sizeof(buf));
      ERR_IF(write_bytes < 0, "write back msg(%s) fail", buf);
    }

    if (read_bytes == 0) {
      LOG_WARN("client sockfd(%d) disconnect.", sockfd);
      break;
    }
    FATAL_IF(read_bytes < 0, "sockfd(%d) read fail.", sockfd);
  }
  return 0;
}