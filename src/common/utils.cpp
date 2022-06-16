/*
 * @Author: Hrimfaxi 851957818@qq.com
 * @Date: 2022-06-14 20:11:41
 * @LastEditors: Hrimfaxi 851957818@qq.com
 * @LastEditTime: 2022-06-14 20:18:32
 * @FilePath: /yankewen/code/HttpServer/src/common/utils.cpp
 * @Description: 业务无关的独立功能函数
 *
 * Copyright (c) 2022 by Hrimfaxi 851957818@qq.com, All Rights Reserved.
 */
#include "utils.h"
namespace utils {
void get_input(char buf[BUFFER_SIZE]) { std::cin.getline(buf, BUFFER_SIZE); }
}  // namespace utils