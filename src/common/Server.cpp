#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <cstring>
#include <iostream>
#include <functional>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>
#include "utils.h"
#include "def.h"
#include "Epoll.h"
#include "InetAddress.h"
#include "Socket.h"
#include "Channel.h"
#include "Server.h"
#include "Acceptor.h"
int Server::handle_read_event(const int fd) {  // process ET & nonblocking
  int ret = ALL_OK;
  char buf[BUFFER_SIZE];
  while (true) {
    bzero(&buf, sizeof(buf));
    ssize_t bytes_read = read(fd, buf, sizeof(buf));
    if (bytes_read > 0) {
      LOG_INFO("msg from client fd(%d):%s", fd, buf);
      ERR_IF(write(fd, buf, sizeof(buf)) < 0, "write back msg(%s) to fd(%d) fali", buf, fd);
      LOG_INFO("write back success.");
      // write 失败只打日志，始终通过读判断连接的正常性
    } else if (bytes_read == -1 && errno == EINTR) {  // 客户端正常中断，继续读
      LOG_INFO("continue reading.");
    } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {  // 读取完毕
      LOG_INFO("read complete.");
      break;
    } else if (bytes_read == 0) {
      LOG_INFO("EOF, client fd(%d) disconnected, close it.", fd);  // EOF 断开了
      sock_mgr_->erase_socket(fd);
      break;
    } else {
      LOG_ERR("unknonw condition bytes_read(%d).", static_cast<int>(bytes_read));
      sock_mgr_->erase_socket(fd);
      ret = SERVER_HANDLE_EVENT_READ_ERROR;

      break;
    }
  }

  return ret;
}

int Server::new_connection(Socket &serv_sock) {
  LOG_INFO("start process new connection fd(%d)", serv_sock.get_fd());
  using namespace utils;
  int ret = ALL_OK;

  InetAddress *clnt_addr = new InetAddress();
  defer(delete clnt_addr);

  int clnt_fd = -1;
  ret = serv_sock.accept(*clnt_addr, clnt_fd);
  if (ret != ALL_OK) {
    LOG_ERR("server accept fd(%d) ret(%d)", serv_sock.get_fd(), ret);
    return ret;
  }

  Socket *clnt_sock = new Socket();
  ret = clnt_sock->create(sock_mgr_, false, clnt_fd);
  if (ret != ALL_OK) {
    LOG_ERR("server create client socket cltn_fd(%d) ret(%d)", clnt_fd, ret);
    return ret;
  }

  LOG_INFO("new client fd(%d)! IP(%s) Port(%d)", clnt_sock->get_fd(), inet_ntoa(clnt_addr->addr_.sin_addr),
           ntohs(clnt_addr->addr_.sin_port));

  clnt_sock->setnonblocking();  // ET 模式必须
  // 对于新连接产生的客户端 fd, 如果活跃，说明有可读事件发生，所以应该绑定 handle_read_event()
  auto cb = std::bind(&Server::handle_read_event, this, clnt_sock->get_fd());
  Channel *ch = new Channel(loop_, clnt_fd);
  ch->set_callback(cb);

  ret = ch->enable_reading();
  if (ret != ALL_OK) {
    LOG_ERR("Add fd(%d) to loop ret(%d).", ch->get_fd(), ret);
    return ret;
  }

  LOG_INFO("create new connection success, IP(%s) Post(%d) clnt_fd(%d)", inet_ntoa(clnt_addr->addr_.sin_addr),
           ntohs(clnt_addr->addr_.sin_port), ch->get_fd());
  return ALL_OK;
}

Server::Server(EventLoop *loop) {
  loop_ = loop;
  acceptor_ = new Acceptor(loop);
  sock_mgr_ = new SockMgr();
}

Server::~Server() {}

int Server::init() {
  LOG_INFO("server init start.");
  int ret = ALL_OK;

  ret = acceptor_->init(sock_mgr_);
  FATAL_IF(ret != ALL_OK, "init acceptor");
  acceptor_->set_new_connection_callback(std::bind(&Server::new_connection, this, std::placeholders::_1));

  LOG_INFO("init server success");
  return ret;
}