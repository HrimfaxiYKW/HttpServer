/*
 * @Author: Hrimfaxi 851957818@qq.com
 * @Date: 2022-06-14 20:10:13
 * @LastEditors: Hrimfaxi 851957818@qq.com
 * @LastEditTime: 2022-06-14 20:10:13
 * @FilePath: /yankewen/code/HttpServer/src/common/InetAddress.cpp
 * @Description:class InetAddress
 *
 * Copyright (c) 2022 by Hrimfaxi 851957818@qq.com, All Rights Reserved.
 */
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