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

int main() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serv_addr;
  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(8848);

  bind(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr));

  // 最大监听队列长度，默认 128
  listen(sockfd, SOMAXCONN);

  struct sockaddr_in clnt_addr;
  socklen_t clnt_addr_len = sizeof(clnt_addr);
  bzero(&clnt_addr, sizeof(clnt_addr));

  // accept 是阻塞调用
  int clnt_sockfd = accept(sockfd, (sockaddr *)&clnt_addr, &clnt_addr_len);

  printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
  return 0;
}