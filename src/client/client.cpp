/*****************************************************************************
 * Copyright (c) 2022, Yan Kewen. All rights reserved.
 *
 * @file client.cpp
 *
 * Created by Yan Kewen (ykwhrimfaxi@gmail.com), 2022/06/08.
 *
 * Managed by Yan Kewen
 *****************************************************************************/
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "../common/utils.h"
#include "../common/scoped_guard.h"
int main() {
  // IPV4 & TCP
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serv_addr;
  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;  // IPV4
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(8848);

  int ret = connect(sockfd, reinterpret_cast<sockaddr *>(&serv_addr), sizeof(serv_addr));
  std::cout << "connect" << ret << std::endl;
  close(sockfd);

  // using namespace utils;
  defer(utils::fuck());
  return 0;
}