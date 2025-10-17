#include "trap.h"

extern Area heap;

void halt(int code);

volatile void byte_test() {
  char *start_addr = heap.start;
  char *end_addr = heap.end;

  volatile char *i;
  volatile char t;
  for (i = start_addr, t = 0; i < end_addr; i++, t++) {
    *i = t;
    check(*i == t);
  }
}

volatile void halfword_test() {
  uint16_t *start_addr = heap.start;
  uint16_t *end_addr = heap.end;

  volatile uint16_t *i;
  volatile uint16_t t;
  for (i = start_addr, t = 0; i < end_addr; i++, t++) {
    *i = t;
    check(*i == t);
  }
}

volatile void word_test() {
  uint32_t *start_addr = heap.start;
  uint32_t *end_addr = heap.end;

  volatile uint32_t *i;
  volatile uint32_t t;
  for (i = start_addr, t = 0; i < end_addr; i++, t++) {
    *i = t;
    check(*i == t);
  }
}

volatile void dword_test() {
  uint64_t *start_addr = heap.start;
  uint64_t *end_addr = heap.end;

  volatile uint64_t *i;
  volatile uint64_t t;
  for (i = start_addr, t = 0; i < end_addr; i++, t++) {
    *i = t;
    check(*i == t);
  }
}

volatile void mem_test() {
  printf("start\n");
  byte_test();
  printf("byte done\n");
  halfword_test();
  printf("halfword done\n");
  word_test();
  printf("word done\n");
  dword_test();
  printf("dword done\n");
}

int main() {
  mem_test();
  printf("all done\n");
  return 0;
}
