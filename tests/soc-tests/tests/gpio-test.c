#include "trap.h"
#define GPIO_BASE   0x10002000
#define GPIO_LED    GPIO_BASE + 0x0
#define GPIO_SWITCH GPIO_BASE + 0x4
#define GPIO_SEG    GPIO_BASE + 0x8
#define GPIO_SIZE   0xf

#define PASSWORD    0xf125

static uint32_t get_id() {
  uint32_t archid;
  asm volatile ("csrr %0, 0xF12" : "=r"(archid));
  return archid;
}

static void volatile wait(volatile int n) {
  while (n-- > 0);
}

int main() {
  volatile uint16_t n = 0x0;
  volatile uint32_t id = get_id();
  volatile uint16_t code = 0;
  while (1) {
    code = *(volatile uint16_t *)(GPIO_SWITCH);
    if (code != PASSWORD) continue;
    if (n == 0x0) n = 0x1;
    *(volatile uint16_t *)(GPIO_LED) = n;
    *(volatile uint32_t *)(GPIO_SEG) = id;
    n <<= 1;
    wait(10000);
  }
  
  return 0;
}
