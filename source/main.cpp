#include <cstdio>
#include <cstdlib>

#include <nds.h>
#include <dswifi9.h>

#include "num_input.hpp"
#include "udp_sender.hpp"

int main() {
  char ip_addr[] = "192.168.000.000";
  char port[] = "54321";

  consoleDemoInit();

  iprintf("\n\n  NDS UDP Controller\n\n"
          "                   by @rin4046\n\n\n\n\n\n");

  // IPアドレス入力
  NumInput ip_input{ip_addr, 10, 13};
  iprintf("    IP Addr:");
  ip_input.show();

  {
    // 先頭の0を詰める
    bool flag = false;
    int j = 0;
    for (int i = 0; i < 16; i++) {
      if (flag && ip_addr[i] == '0')
        continue;
      else if (ip_addr[i] == '.')
        flag = true;
      else
        flag = false;
      if (j != i) ip_addr[j] = ip_addr[i];
      j++;
    }
  }

  // ポート番号入力
  NumInput port_input{port, 12, 13};
  iprintf("\n       Port:");
  port_input.show();

  UDPSender sender;
  Wifi_InitDefault(WFC_CONNECT);
  sender.open(ip_addr, atoi(port));

  // UDP送信ループ
  while (1) {
    swiWaitForVBlank();
    scanKeys();
    uint16_t keys = keysHeld();
    if (keys & KEY_START && keys & KEY_SELECT) break;
    iprintf("\e[%d;0H  Keys: %5d", 18, keys);
    sender.send(keys);
  }

  sender.close();

  return 0;
}
