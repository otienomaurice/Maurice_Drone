#include "stdbool.h"
#include "inttypes.h"
#include "stdlib.h"
    // Define SPI0 pins
#define RX_MISO 8
#define RX_MOSI 7
#define RX_SCK  6
#define RX_CS   9

// Define control pins
#define RX_RST  12
#define RX_EN   11
#define RX_IRQ 10
//
#define REG_FIFO             0x00
#define REG_OP_MODE          0x01
#define REG_FIFO_ADDR_PTR    0x0D
#define REG_FIFO_RX_CURRENT  0x10
#define REG_IRQ_FLAGS        0x12
#define REG_RX_NB_BYTES      0x13

#define MODE_LONG_RANGE_MODE 0x80
#define MODE_SLEEP           0x00
#define MODE_STDBY           0x01
#define MODE_RX_CONTINUOUS   0x05

#define IRQ_RX_DONE_MASK     0x40
#define IRQ_VALID_HEADER     0x10
#define IRQ_CLEAR_MASK       0xFF
     void spi_read_burst_rx(uint8_t reg, uint8_t *data, size_t len) ;
     void spi_write_register_rx(uint8_t reg, uint8_t value) ;
     uint8_t spi_read_register_rx(uint8_t reg);
     void spi_write_burst_rx(uint8_t reg, const uint8_t *data, size_t len);
     void init_receiver();
     void receive();