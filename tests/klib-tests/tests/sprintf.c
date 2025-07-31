#include "trap.h"

#define BUF 128
#define ITEM 15

#define INT_MAX  0x7fffffff
#define INT_MIN  (-INT_MAX - 1)
#define UINT_MAX (INT_MAX * 2U + 1U)

int data[] = {0, INT_MAX / 17, INT_MAX, INT_MIN, INT_MIN + 1,
              UINT_MAX / 17, INT_MAX / 17, UINT_MAX};

int ret_ans[ITEM] = {12, 28, 43, 43, 52, 42, 51, 50, 51, 42, 40, 40, 41, 41, 29};
int ret_data[ITEM] = {};
              
char *ans[ITEM] = {
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
  "wdawdai231%Vdsada15$ta100%, 0"
};

char buf[ITEM][BUF];

void test_snprintf() {
  ret_data[0]  = sprintf(buf[ 0], "Hello world!");
  ret_data[1]  = sprintf(buf[ 1], "wdawdai%d %d, %s", data[0], data[1], "dwadasda");
  ret_data[2]  = sprintf(buf[ 2], "wdawdai231 %d + %d = %d, %c", data[2], data[3], data[2] + data[3], '0');
  ret_data[3]  = sprintf(buf[ 3], "wdawdai231 %d - %d = %d, %c", data[2], data[3], data[2] - data[3], '1');
  ret_data[4]  = sprintf(buf[ 4], "wdawdai231 %d * %d = %d, %c", data[2], data[3], data[2] * data[3], '2');
  ret_data[5]  = sprintf(buf[ 5], "wdawdai231 %d / %d = %d, %c", data[2], data[3], data[2] / data[3], '3');
  ret_data[6]  = sprintf(buf[ 6], "wdawdai231 %d + %d = %d, %c", data[4], data[5], data[4] + data[5], '4');
  ret_data[7]  = sprintf(buf[ 7], "wdawdai231 %d - %d = %d, %c", data[4], data[5], data[4] - data[5], '5');
  ret_data[8]  = sprintf(buf[ 8], "wdawdai231 %d * %d = %d, %c", data[4], data[5], data[4] * data[5], '6');
  ret_data[9]  = sprintf(buf[ 9], "wdawdai231 %d / %d = %d, %c", data[4], data[5], data[4] / data[5], 'e');
  ret_data[10] = sprintf(buf[10], "wdawdai231 %d + %d = %d, %c", data[6], data[7], data[6] + data[7], 'g');
  ret_data[11] = sprintf(buf[11], "wdawdai231 %d - %d = %d, %c", data[6], data[7], data[6] - data[7], 'v');
  ret_data[12] = sprintf(buf[12], "wdawdai231 %d * %d = %d, %c", data[6], data[7], data[6] * data[7], 'x');
  ret_data[13] = sprintf(buf[13], "wdawdai231 %d / %d = %d, %c", data[6], data[7], data[6] / data[7], 'z');
  for (int i = 0; i < 3; i++) {
    check(strcmp(ans[i], buf[i]) == 0);
    check(ret_data[i] == ret_ans[i]);
  }
}

int main(){
  test_snprintf();

  return 0;
}
