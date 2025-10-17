// Standard SPI instructions use the unidirectional DI (input) pin
// to serially write to the device on the rising edge of CLK.
// Standard SPI also uses the unidirectional DO (output)
// to read data from the device on the falling edge of CLK.

#include "trap.h"
#define FLASH_BASE 0x30000000
#define FLASH_SIZE 0x1000000
#define READ_INST  0x03

#define SPI_BASE 0x10001000
#define SPI_RX0  0x00
#define SPI_RX1  0x04
#define SPI_RX2  0x08
#define SPI_RX3  0x0c
#define SPI_TX0  0x00
#define SPI_TX1  0x04
#define SPI_TX2  0x08
#define SPI_TX3  0x0c
#define SPI_CTRL 0x10
#define SPI_DIVI 0x14
#define SPI_SS   0x18

#define SLAVE_ID  0b00000001
#define DIVI_NUM  1
#define CHAR_LEN  64
#define TX_NEG    1
#define RX_NEG    0
#define LSB       0
#define ASS       1
#define CTRL_DATA (0x00000000 | (ASS << 13) | (LSB << 11) | (TX_NEG << 10) | (RX_NEG << 9) | (0 << 8) | (CHAR_LEN))
#define CTRL_ON   (0x00000000 | (ASS << 13) | (LSB << 11) | (TX_NEG << 10) | (RX_NEG << 9) | (1 << 8) | (CHAR_LEN))

#define SLAVE_VALID_MASK 0x00ffffff

void spi_init() {
  *(volatile uint8_t  *)(SPI_BASE + SPI_SS  ) = (uint8_t )SLAVE_ID;
  *(volatile uint32_t *)(SPI_BASE + SPI_CTRL) = (uint32_t)CTRL_DATA;
  *(volatile uint32_t *)(SPI_BASE + SPI_DIVI) = (uint32_t)DIVI_NUM;
}

void spi_send(uint32_t data) {
  *(volatile uint32_t *)(SPI_BASE + SPI_RX0 ) = (uint32_t)0xffffffff;
  *(volatile uint32_t *)(SPI_BASE + SPI_RX1 ) = (uint32_t)data;
  *(volatile uint32_t *)(SPI_BASE + SPI_CTRL) = (uint32_t)CTRL_ON;
  while (((*(volatile uint32_t *)(SPI_BASE + SPI_CTRL)) & 0x00000100) == 0x00000100);
}

uint32_t spi_read() {
  return *(volatile uint32_t *)(SPI_BASE + SPI_TX0);
}

uint32_t flash_read(uint32_t addr) {
  uint32_t spi_addr = addr & SLAVE_VALID_MASK;
  uint32_t wdata = (READ_INST << 24) | spi_addr;
  spi_send(wdata);
  // the return 4-byte data is in an order of byte0 byte1 byte2 byte3
  return spi_read();
}

int main() {
  spi_init();
  for (int i = 0; i < FLASH_SIZE >> 8; i++) {
    uint32_t result = flash_read(FLASH_BASE + i);
    uint8_t rdata = result >> (((i % 4) << 3));
    check(rdata == i % 256);
  }
  return 0;
}
