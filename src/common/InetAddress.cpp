/*****************************************************************************
 * Copyright (c) 2022, Yan Kewen. All rights reserved.
 *
 * @file InetAddress.cpp
 *
 * Define class InetAddress
 *
 * Created by Yan Kewen (ykwhrimfaxi@gmail.com), 2022/06/10.
 *
 * Managed by Yan Kewen
 *****************************************************************************/
#include "InetAddress.h"

InetAddress::InetAddress() : addr_len_(sizeof(addr_)) { bzero(&addr_, sizeof(addr_)); }
InetAddress::InetAddress(const std::string &ip, uint16_t port) : addr_len_(sizeof(addr_)) {
  bzero(&addr_, sizeof(addr_));
  addr_.sin_family = AF_INET;
  addr_.sin_addr.s_addr = inet_addr(ip.c_str());
  addr_.sin_port = htons(port);
  addr_len_ = sizeof(addr_);
}

InetAddress::~InetAddress() {}