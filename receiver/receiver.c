#include "receiver/receiver.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "inttypes.h"
#include "global/global.h"
#include "stdio.h"
#include "stdint.h"
 void spi_write_register_rx(uint8_t reg, uint8_t value) {
    uint8_t buf[2] = { reg | 0x80, value };
    gpio_put(RX_CS, 0);
    spi_write_blocking(spi1, buf, 2);
    gpio_put(RX_CS, 1);
 }

 uint8_t spi_read_register_rx(uint8_t reg) {
    uint8_t tx = reg & 0x7F;
    uint8_t rx;
    gpio_put(RX_CS, 0);
    spi_write_blocking(spi1, &tx, 1);
    spi_read_blocking(spi1, 0x00, &rx, 1);
    gpio_put(RX_CS, 1);
    return rx;
 }

 void spi_write_burst_rx(uint8_t reg, const uint8_t *data, size_t len) {
    gpio_put(RX_CS, 0);
    uint8_t header = reg | 0x80;
    spi_write_blocking(spi1, &header, 1);
    spi_write_blocking(spi1, data, len);
    gpio_put(RX_CS, 1);
 }
 void spi_read_burst_rx(uint8_t reg, uint8_t *data, size_t len) {
    gpio_put(RX_CS, 0);
    uint8_t cmd = reg & 0x7F;
    spi_write_blocking(spi1, &cmd, 1);
    spi_read_blocking(spi1, 0x00, data, len);
    gpio_put(RX_CS, 1);
 }
 void init_receiver() {
    spi_init(spi1, 1 * 1000 * 1000); // 1 MHz
    gpio_set_function(RX_MISO, GPIO_FUNC_SPI);
    gpio_set_function(RX_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(RX_SCK, GPIO_FUNC_SPI);

    gpio_init(RX_CS);
    gpio_set_dir(RX_CS, GPIO_OUT);
    gpio_put(RX_CS, 1);

    gpio_init(RX_RST);
    gpio_set_dir(RX_RST, GPIO_OUT);
    gpio_put(RX_RST, 0);
    sleep_ms(10);
    gpio_put(RX_RST, 1);

    gpio_init(RX_EN);
    gpio_set_dir(RX_EN, GPIO_OUT);
    gpio_put(RX_EN, 1); // Power on LoRa module

    gpio_init(RX_IRQ);
    gpio_set_dir(RX_IRQ, GPIO_IN);
    gpio_pull_up(RX_IRQ);

    sleep_ms(100);

    // Put into LoRa + standby
    spi_write_register_rx(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);

    // Clear all IRQ flags
    spi_write_register_rx(REG_IRQ_FLAGS, IRQ_CLEAR_MASK);

    // Put into continuous receive mode
    spi_write_register_rx(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_CONTINUOUS);
 }


 void receive() {
    if ((spi_read_register_rx(REG_IRQ_FLAGS) & IRQ_RX_DONE_MASK) == 0) {
        return; // Nothing received yet
    }

    // 1. Read number of bytes received
    uint8_t len = spi_read_register_rx(REG_RX_NB_BYTES);

    // 2. Set FIFO read pointer
    uint8_t fifo_addr = spi_read_register_rx(REG_FIFO_RX_CURRENT);
    spi_write_register_rx(REG_FIFO_ADDR_PTR, fifo_addr);

    // 3. Read data from FIFO
    uint8_t buffer[4] = {0};
    spi_read_burst_rx(REG_FIFO, buffer, len);

    // 4. Clear IRQ flags
    spi_write_register_rx(REG_IRQ_FLAGS, IRQ_CLEAR_MASK);

    // 5. Parse joystick values
    uint16_t horizontal1 = (buffer[0] << 8) | buffer[1];
    uint16_t vertical1 = (buffer[2] << 8) | buffer[3];

    printf("Received: horizontal1=%d vertical1=%d\n", horizontal1, vertical1);
 }