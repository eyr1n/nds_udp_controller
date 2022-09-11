#pragma once

#include <cstring>
#include <cctype>

#include <nds.h>

class NumInput {
  char *value_;
  size_t cur_ = 0;
  int y_;
  int x_;

public:
  NumInput(char *value, int y, int x) : value_(value), y_(y), x_(x) {}

  void show() {
    while (1) {
      swiWaitForVBlank();
      iprintf("\e[%d;%dH[%s]\n", y_, x_, value_);
      iprintf("\e[K");
      scanKeys();
      switch (keysDown()) {
        case KEY_UP:
          up();
          break;
        case KEY_DOWN:
          down();
          break;
        case KEY_LEFT:
          left();
          break;
        case KEY_RIGHT:
          right();
          break;
        case KEY_A:
          return;
      }
      iprintf("\e[%d;%dH^", y_ + 1, x_ + cur_ + 1);
    }
  }

private:
  void up() {
    if (value_[cur_] == '9') {
      value_[cur_] = '0';
    } else {
      value_[cur_]++;
    }
  }

  void down() {
    if (value_[cur_] == '0') {
      value_[cur_] = '9';
    } else {
      value_[cur_]--;
    }
  }

  void left() {
    if (cur_ != 0) {
      do {
        cur_--;
      } while (!isdigit(value_[cur_]));
    }
  }

  void right() {
    if (cur_ != strlen(value_) - 1) {
      do {
        cur_++;
      } while (!isdigit(value_[cur_]));
    }
  }
};
