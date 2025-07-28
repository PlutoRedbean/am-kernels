#include "trap.h"

#define N 32

char data[N];
char str[N];

void reset() {
  int i;
  for (i = 0; i < N - 1; i ++) {
    data[i] = i + 1 ;
  }
}

void check_seq(int l, int r, int val) {
  int i;
  for (i = l; i < r; i ++) {
    check(data[i] == val + i - l);
  }
}

void test_strcpy() {
  int l;
  for (l = 0; l < N; l ++) {
    reset();
    data[l] = '\0';
    strcpy(str, data);
    check_seq(0, l, 1);
    check(str[l] == '\0');
  }
}

int main(){
  test_strcpy();

  return 0;
}
