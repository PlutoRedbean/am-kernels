#include "trap.h"

#define BUF 128
#define ITEM 14

#define INT_MAX  0x7fffffff
#define INT_MIN  (-INT_MAX - 1)
#define UINT_MAX (INT_MAX * 2U + 1U)

int data[] = {0, INT_MAX / 17, INT_MAX, INT_MIN, INT_MIN + 1,
              UINT_MAX / 17, INT_MAX / 17, UINT_MAX};

char *ans[14] = {
  "Hello world!",
  "wdawdai0 126322567, dwadasda",
  "wdawdai231 2147483647 + -2147483648 = -1, 0",
  "wdawdai231 2147483647 - -2147483648 = -1, 1",
  "wdawdai231 2147483647 * -2147483648 = -2147483648, 2",
  "wdawdai231 2147483647 / -2147483648 = 0, 3",
  "wdawdai231 -2147483647 + 252645135 = -1894838512, 4",
  "wdawdai231 -2147483647 - 252645135 = 1894838514, 5",
  "wdawdai231 -2147483647 * 252645135 = -1894838513, 6",
  "wdawdai231 -2147483647 / 252645135 = -8, e",
  "wdawdai231 126322567 + -1 = 126322566, g",
  "wdawdai231 126322567 - -1 = 126322568, v",
  "wdawdai231 126322567 * -1 = -126322567, x",
  "wdawdai231 126322567 / -1 = -126322567, z",
};

char buf[ITEM][BUF];

void test_snprintf() {
  sprintf(buf[0], "Hello world!");
  sprintf(buf[1], "wdawdai%d %d, %s", data[0], data[1], "dwadasda");
  sprintf(buf[2], "wdawdai231 %d + %d = %d, %c", data[2], data[3], data[2] + data[3], '0');
  sprintf(buf[3], "wdawdai231 %d - %d = %d, %c", data[2], data[3], data[2] - data[3], '1');
  sprintf(buf[4], "wdawdai231 %d * %d = %d, %c", data[2], data[3], data[2] * data[3], '2');
  sprintf(buf[5], "wdawdai231 %d / %d = %d, %c", data[2], data[3], data[2] / data[3], '3');
  sprintf(buf[6], "wdawdai231 %d + %d = %d, %c", data[4], data[5], data[4] + data[5], '4');
  sprintf(buf[7], "wdawdai231 %d - %d = %d, %c", data[4], data[5], data[4] - data[5], '5');
  sprintf(buf[8], "wdawdai231 %d * %d = %d, %c", data[4], data[5], data[4] * data[5], '6');
  sprintf(buf[9], "wdawdai231 %d / %d = %d, %c", data[4], data[5], data[4] / data[5], 'e');
  sprintf(buf[10], "wdawdai231 %d + %d = %d, %c", data[6], data[7], data[6] + data[7], 'g');
  sprintf(buf[11], "wdawdai231 %d - %d = %d, %c", data[6], data[7], data[6] - data[7], 'v');
  sprintf(buf[12], "wdawdai231 %d * %d = %d, %c", data[6], data[7], data[6] * data[7], 'x');
  sprintf(buf[13], "wdawdai231 %d / %d = %d, %c", data[6], data[7], data[6] / data[7], 'z');
  for (int i = 0; i < 3; i++) {
    check(strcmp(ans[i], buf[i]) == 0);
  }
}

int main(){
  test_snprintf();

  return 0;
}
