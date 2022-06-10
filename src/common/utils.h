/*****************************************************************************
 * Copyright (c) 2022, Yan Kewen. All rights reserved.
 *
 * @file utils.h
 *
 * some common utils
 *
 * Created by Yan Kewen (ykwhrimfaxi@gmail.com), 2022/06/09.
 *
 * Managed by Yan Kewen
 *****************************************************************************/
#pragma once
#include <iostream>
#include <cstring>
#include "scoped_guard.h"
#include "def.h"
#include <stdlib.h>
#include <stdio.h>

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
  };                               \
  exit(EXIT_FAILURE);

#define ERR_IF(value, M, arg...) \
  if (value) {                   \
    LOG_ERR(M, ##arg);           \
  };

#define WARN_IF(value, M, arg...) \
  if (value) {                    \
    LOG_WARN(M, ##arg);           \
  };

#define INFO_IF(value, M, arg...) \
  if (value) {                    \
    LOG_INFO(M, ##arg);           \
  };

namespace utils {
void get_input(char *buf);
}  // namespace utils