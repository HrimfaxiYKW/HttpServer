/*
 * @Author: Hrimfaxi 851957818@qq.com
 * @Date: 2022-06-14 20:10:26
 * @LastEditors: Hrimfaxi 851957818@qq.com
 * @LastEditTime: 2022-06-14 20:10:26
 * @FilePath: /yankewen/code/HttpServer/src/common/InetAddress.h
 * @Description: class InetAddress
 *
 * Copyright (c) 2022 by Hrimfaxi 851957818@qq.com, All Rights Reserved.
 */
#pragma once
#include <arpa/inet.h>
#include <cstring>
#include <iostream>

class InetAddress {
 public:
  InetAddress();
  InetAddress(const std::string &ip, uint16_t port);
  ~InetAddress();

  sockaddr_in addr_;
  socklen_t addr_len_;
};