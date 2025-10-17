#include "trap.h"
#define FLASH_BASE 0x30000000
#define FLASH_SIZE 0x1000000

char read_process(int addr) {
  char result = *(volatile char *)(uintptr_t)addr;
  return result;
}

int main() {
  for (int i = 0; i < FLASH_SIZE >> 8; i++) {
    char rdata = read_process(FLASH_BASE + i);
    check(rdata == i % 256);
  }
  return 0;
}
