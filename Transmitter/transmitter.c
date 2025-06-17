#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "stdio.h"
#include "inttypes.h"
#include "transmitter.h"
#include "global/global.h"

void spi_write_register(uint8_t reg, uint8_t value) {
    uint8_t buf[2] = { reg | 0x80, value };
    gpio_put(TX_CS, 0);
    spi_write_blocking(spi1, buf, 2);
    gpio_put(TX_CS, 1);
}

uint8_t spi_read_register(uint8_t reg) {
    uint8_t tx = reg & 0x7F;
    uint8_t rx;
    gpio_put(TX_CS, 0);
    spi_write_blocking(spi1, &tx, 1);
    spi_read_blocking(spi1, 0x00, &rx, 1);
    gpio_put(TX_CS, 1);
    return rx;
}

void spi_write_burst(uint8_t reg, const uint8_t *data, size_t len) {
    gpio_put(TX_CS, 0);
    uint8_t header = reg | 0x80;
    spi_write_blocking(spi1, &header, 1);
    spi_write_blocking(spi1, data, len);
    gpio_put(TX_CS, 1);
}

void set_lora_frequency(uint64_t freq_hz) {
    uint64_t frf = (freq_hz << 19) / 32000000;
    spi_write_register(0x06, (frf >> 16) & 0xFF); // REG_FRF_MSB
    spi_write_register(0x07, (frf >> 8) & 0xFF);  // REG_FRF_MID
    spi_write_register(0x08, frf & 0xFF);         // REG_FRF_LSB
}

void init_transmitter() {
    spi_init(spi1, 1000000); // 1 MHz
    gpio_set_function(TX_MISO, GPIO_FUNC_SPI);
    gpio_set_function(TX_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(TX_SCK, GPIO_FUNC_SPI);

    gpio_init(TX_CS);
    gpio_set_dir(TX_CS, GPIO_OUT);
    gpio_put(TX_CS, 1);

    gpio_init(TX_RST);
    gpio_set_dir(TX_RST, GPIO_OUT);
    gpio_put(TX_RST, 0);
    sleep_ms(10);
    gpio_put(TX_RST, 1);

    gpio_init(TX_EN);
    gpio_set_dir(TX_EN, GPIO_OUT);
    gpio_put(TX_EN, 1);

    gpio_init(TX_IRQ);
    gpio_set_dir(TX_IRQ, GPIO_IN);
    gpio_pull_up(TX_IRQ);

    sleep_ms(100);

    // Enter sleep mode before setting frequency
    spi_write_register(0x01, 0x80 | 0x00);  // REG_OP_MODE = LoRa + SLEEP
    sleep_ms(5);

    // Set frequency to 915 MHz
    set_lora_frequency(915000000);

    // Set to standby
    spi_write_register(0x01, 0x80 | 0x01);  // REG_OP_MODE = LoRa + STDBY
    sleep_ms(5);

    uint8_t version = spi_read_register(0x42); // REG_VERSION
    printf("LoRa chip version: 0x%02X\n", version);
    if (version != 0x12) {
        printf("⚠️ Warning: Unexpected chip version (expected 0x12).\n");
    }
}

void transmit() {
     uint8_t buffer[4] = {
        (x_1 >> 8) & 0xFF,
        x_1 & 0xFF,
        (y_1 >> 8) & 0xFF,
        y_1 & 0xFF
     };
     sleep_ms(100);
     spi_write_register(0x01, 0x80 | 0x01); // REG_OP_MODE = LoRa + STDBY
     spi_write_register(0x0D, 0x00);        // REG_FIFO_ADDR_PTR = 0
     spi_write_burst(0x00, buffer, sizeof(buffer)); // REG_FIFO
     spi_write_register(0x22, sizeof(buffer));      // REG_PAYLOAD_LENGTH = 4
     spi_write_register(0x01, 0x80 | 0x03); // REG_OP_MODE = LoRa + TX
     printf("Transmitting...\n");
         spi_write_register(0x12, 0x08); // Clear TX_DONE flag in REG_IRQ_FLAGS
         printf("Sent: x_1=%d y_1=%d\n", x_1, y_1);
}
