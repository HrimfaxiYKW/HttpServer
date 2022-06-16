/*
 * @Author: Hrimfaxi 851957818@qq.com
 * @Date: 2022-06-14 20:16:16
 * @LastEditors: Hrimfaxi 851957818@qq.com
 * @LastEditTime: 2022-06-14 20:16:16
 * @FilePath: /yankewen/code/HttpServer/src/client/client.cpp
 * @Description:
 *
 * Copyright (c) 2022 by Hrimfaxi 851957818@qq.com, All Rights Reserved.
 */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "../common/utils.h"
#include "../common/def.h"
#include "../common/Socket.h"
int main() {
  // IPV4 & TCP
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  defer(close(sockfd));
  FATAL_IF_WITH_CLOSE(sockfd == -1, sockfd, "socket create fail.");

  struct sockaddr_in serv_addr;
  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;  // IPV4
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(8848);

  FATAL_IF_WITH_CLOSE(connect(sockfd, reinterpret_cast<sockaddr *>(&serv_addr), sizeof(serv_addr)) == -1, sockfd,
                      "connect fail.");

  while (true) {
    char buf[BUFFER_SIZE];
    bzero(&buf, sizeof(buf));
    utils::get_input(buf);
    ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
    FATAL_IF_WITH_CLOSE(write_bytes < 0, sockfd, "sockfd(%d) already disconnected.", sockfd);

    // clear buffer
    bzero(&buf, sizeof(buf));
    auto read_bytes = read(sockfd, buf, sizeof(buf));
    INFO_IF(read_bytes > 0, "sockfd(%d) recieve:%s\n", sockfd, buf);
    if (read_bytes == 0) {
      LOG_WARN("server socket(%d) disconnect.", sockfd);
      break;
    }

    FATAL_IF_WITH_CLOSE(read_bytes == -1, sockfd, "read fail.");
  }

  return 0;
}