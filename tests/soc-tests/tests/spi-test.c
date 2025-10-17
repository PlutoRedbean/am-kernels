#include "trap.h"

#define TEST_NUM 4

#define SPI_BASE 0x10001000
#define SPI_RX0  0x00
#define SPI_TX0  0x00
#define SPI_CTRL 0x10
#define SPI_DIVI 0x14
#define SPI_SS   0x18

#define CHAR_LEN  0x10
#define TX_NEG    1
#define RX_NEG    0
#define LSB       1
#define ASS       1
#define CTRL_DATA (0x00000000 | (ASS << 13) | (LSB << 11) | (TX_NEG << 10) | (RX_NEG << 9) | (0 << 8) | (CHAR_LEN))
#define CTRL_ON   (0x00000000 | (ASS << 13) | (LSB << 11) | (TX_NEG << 10) | (RX_NEG << 9) | (1 << 8) | (CHAR_LEN))

char lut[TEST_NUM] = { 0b10010100, 0b11010100, 0b10101100, 0b10111100 }; // 0b11111111
char ans[TEST_NUM] = { 0b00101001, 0b00101011, 0b00110101, 0b00111101 };

void spi_init() {
  *(volatile uint8_t *)(SPI_BASE + SPI_SS  ) = (uint8_t)0b10000000;
  *(volatile uint32_t *)(SPI_BASE + SPI_CTRL) = (uint32_t)CTRL_DATA;
  *(volatile uint32_t *)(SPI_BASE + SPI_DIVI) = (uint32_t)1;
}

void spi_send(uint32_t data) {
  *(volatile uint32_t *)(SPI_BASE + SPI_RX0 ) = (uint32_t)data;
  *(volatile uint32_t *)(SPI_BASE + SPI_CTRL) = (uint32_t)CTRL_ON;
  while (((*(volatile uint32_t *)(SPI_BASE + SPI_CTRL)) & 0x00000100) == 0x00000100);
}

int main() {
  uint8_t rdata;
  spi_init();
  for (int i = 0; i < TEST_NUM; i++) {
    spi_send(lut[i]);
    rdata = *(volatile uint8_t *)(SPI_BASE + SPI_TX0 + 1);
    check(rdata == ans[i]);
  }
  
  return 0;
}
