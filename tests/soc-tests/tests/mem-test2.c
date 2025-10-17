#include "trap.h"

void halt(int code);

volatile void byte_test() {
  uint8_t *start_addr = (uint8_t *)(uintptr_t)0x80000000;
  uint8_t *end_addr = (uint8_t *)(uintptr_t)0x80001000;

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
  uint16_t *start_addr = (uint16_t *)(uintptr_t)0x80000000;
  uint16_t *end_addr = (uint16_t *)(uintptr_t)0x80001000;

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
  uint32_t *start_addr = (uint32_t *)(uintptr_t)0x80000000;
  uint32_t *end_addr = (uint32_t *)(uintptr_t)0x80001000;

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
  uint64_t *start_addr = (uint64_t *)(uintptr_t)0x80000000;
  uint64_t *end_addr = (uint64_t *)(uintptr_t)0x80001000;

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
