/*****************************************************************************
 * Copyright (c) 2022, Yan Kewen. All rights reserved.
 *
 * @file InetAddress.h
 *
 * Define class InetAddress
 *
 * Created by Yan Kewen (ykwhrimfaxi@gmail.com), 2022/06/10.
 *
 * Managed by Yan Kewen
 *****************************************************************************/
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