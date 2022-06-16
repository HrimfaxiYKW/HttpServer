/*
 * @Author: Hrimfaxi 851957818@qq.com
 * @Date: 2022-06-14 20:10:49
 * @LastEditors: Hrimfaxi 851957818@qq.com
 * @LastEditTime: 2022-06-14 20:10:49
 * @FilePath: /yankewen/code/HttpServer/src/common/scoped_guard.h
 * @Description: defer function on language level support
 *
 * Copyright (c) 2022 by Hrimfaxi 851957818@qq.com, All Rights Reserved.
 */
#pragma once
#include <utility>

namespace utils {

template <typename F>
struct scoped_guard_t {
  scoped_guard_t(F &&f) : f_(std::forward<F>(f)) {}
  ~scoped_guard_t() { f_(); }
  scoped_guard_t(const scoped_guard_t &) = delete;
  scoped_guard_t(scoped_guard_t &&) = default;  // no need until "-std=c++17"
  scoped_guard_t &operator=(const scoped_guard_t &) = delete;
  scoped_guard_t &operator=(scoped_guard_t &&) = delete;
  F f_;
};

template <typename F>
scoped_guard_t<F> make_defer(F &&f) {
  // required RVO, otherwise the dtor will call twice
  return scoped_guard_t<F>(std::forward<F>(f));
}

#define CAT1(x, y) x##y
#define CAT2(x, y) CAT1(x, y)
#define defer_obj_name CAT2(defer_obj, __COUNTER__)
#define defer(expr) auto defer_obj_name = utils::make_defer([&]() { expr; });
}  // namespace utils