/*
 * @Author: Hrimfaxi 851957818@qq.com
 * @Date: 2022-06-14 20:12:17
 * @LastEditors: Hrimfaxi 851957818@qq.com
 * @LastEditTime: 2022-06-14 20:18:18
 * @FilePath: /yankewen/code/HttpServer/src/common/utils.h
 * @Description: 业务无关的独立功能函数
 *
 * Copyright (c) 2022 by Hrimfaxi 851957818@qq.com, All Rights Reserved.
 */
#pragma once
#include <iostream>
#include <cstring>
#include "scoped_guard.h"
#include "def.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

// 暂时将日志输出到控制台，方便调试
#define LOG_FATAL(M, ...) fprintf(stderr, "[FATAL] at %s:%d:" M "\n", __FILE__, __LINE__, ##__VA_ARGS__);

#define LOG_ERR(M, ...) fprintf(stderr, "[ERROR] at %s:%d:" M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define LOG_WARN(M, ...) fprintf(stderr, "[WARN] at %s:%d:" M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define LOG_INFO(M, ...) fprintf(stderr, "[INFO] at %s:%d:" M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define FATAL_IF_WITH_CLOSE(value, sockfd, M, arg...) \
  if (value) {                                        \
    if (sockfd > 0) close(sockfd);                    \
    LOG_FATAL(M, ##arg);                              \
    exit(EXIT_FAILURE);                               \
  };

#define FATAL_IF(value, M, arg...) \
  if (value) {                     \
    LOG_FATAL(M, ##arg);           \
    exit(EXIT_FAILURE);            \
  };

#define ERR_IF(value, M, arg...) \
  if (value) {                   \
    LOG_ERR(M, ##arg);           \
  };

#define ERR_IF_WITH_CLOSE(value, sockfd, M, arg...) \
  if (value) {                                      \
    if (sockfd > 0) close(sockfd);                  \
    LOG_ERR(M, ##arg);                              \
  };

#define WARN_IF(value, M, arg...) \
  if (value) {                    \
    LOG_WARN(M, ##arg);           \
  };

#define WARN_IF_WITH_CLOSE(value, sockfd, M, arg...) \
  if (value) {                                       \
    if (sockfd > 0) close(sockfd);                   \
    LOG_WARN(M, ##arg);                              \
  };

#define INFO_IF(value, M, arg...) \
  if (value) {                    \
    LOG_INFO(M, ##arg);           \
  };

#define INFO_IF_WITH_CLOSE(value, sockfd, M, arg...) \
  if (value) {                                       \
    if (sockfd > 0) close(sockfd);                   \
    LOG_INFO(M, ##arg);                              \
  };

namespace utils {
void get_input(char *buf);
inline void setnonblocking(int fd) { fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK); }
}  // namespace utils