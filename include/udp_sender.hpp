#pragma once

#include <concepts>

#include <sys/socket.h>
#include <netinet/in.h>

class UDPSender {
  int sock_;
  sockaddr_in addr_ = {0};
  bool is_open_ = false;

public:
  UDPSender() = default;

  ~UDPSender() {
    close();
  }

  bool open(const char *ip_addr, uint16_t port) {
    if (is_open_) {
      return false;
    }
    if ((sock_ = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
      return false;
    }
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = inet_addr(ip_addr);
    addr_.sin_port = htons(port);
    is_open_ = true;
    return true;
  }

  bool close() {
    if (!is_open_) {
      return false;
    }
    if (shutdown(sock_, 0) != 0) {
      return false;
    }
    if (closesocket(sock_) != 0) {
      return false;
    }
    is_open_ = false;
    return true;
  }

  bool send(const void *data, size_t size) {
    if (!is_open_) {
      return false;
    }
    if (sendto(sock_, data, size, 0, reinterpret_cast<sockaddr *>(&addr_),
               sizeof(addr_)) == -1) {
      return false;
    }
    return true;
  }

  template <std::integral T> bool send(T data) {
    return send(&data, sizeof(data));
  }
};
