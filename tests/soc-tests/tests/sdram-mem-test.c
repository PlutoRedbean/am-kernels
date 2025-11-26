#include "trap.h"

#define START_ADDR 0xa0100000
#define MEM_SIZE   0x2000000
#define END_ADDR   START_ADDR + MEM_SIZE

void halt(int code);

volatile void byte_test() {
  uint8_t *start_addr = (uint8_t *)(uintptr_t)START_ADDR;
  uint8_t *end_addr = (uint8_t *)(uintptr_t)END_ADDR;

  volatile uint8_t *i;
  volatile uint8_t t;
  for (i = start_addr, t = 1; i < end_addr; i++, t++) {
    *i = t;
    if (*i != t) {
      printf("%d != %d\n", *i, t);
      halt(1);
    }
  }
}

volatile void halfword_test() {
  uint16_t *start_addr = (uint16_t *)(uintptr_t)START_ADDR;
  uint16_t *end_addr = (uint16_t *)(uintptr_t)END_ADDR;

  volatile uint16_t *i;
  volatile uint16_t t;
  for (i = start_addr, t = 1; i < end_addr; i++, t++) {
    *i = t;
    if (*i != t) {
      printf("%d != %d\n", *i, t);
      halt(1);
    }
  }
}

volatile void word_test() {
  uint32_t *start_addr = (uint32_t *)(uintptr_t)START_ADDR;
  uint32_t *end_addr = (uint32_t *)(uintptr_t)END_ADDR;

  volatile uint32_t *i;
  volatile uint32_t t;
  for (i = start_addr, t = 1; i < end_addr; i++, t++) {
    *i = t;
    if (*i != t) {
      printf("%d != %d\n", *i, t);
      halt(1);
    }
  }
}

volatile void dword_test() {
  uint64_t *start_addr = (uint64_t *)(uintptr_t)START_ADDR;
  uint64_t *end_addr = (uint64_t *)(uintptr_t)END_ADDR;

  volatile uint64_t *i;
  volatile uint64_t t;
  for (i = start_addr, t = 1; i < end_addr; i++, t++) {
    *i = t;
    if (*i != t) {
      printf("%d != %d\n", *i, t);
      halt(1);
    }
  }
}

volatile void mem_test() {
  byte_test();
  // halfword_test();
  // word_test();
  // dword_test();
}

int main() {
  mem_test();
  return 0;
}
