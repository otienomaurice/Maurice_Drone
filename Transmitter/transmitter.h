 #define REG_FIFO             0x00
 #define REG_OP_MODE          0x01
 #define REG_FIFO_ADDR_PTR    0x0D
 #define REG_PAYLOAD_LENGTH   0x22
 #define REG_IRQ_FLAGS        0x12
 #define MODE_LONG_RANGE_MODE 0x80
 #define MODE_SLEEP           0x00
 #define MODE_STDBY           0x01
 #define MODE_TX              0x03
 #define IRQ_TX_DONE_MASK     0x08
 #define TX_MISO 16
 #define TX_MOSI 19
 #define TX_SCK  18
 #define TX_CS   17
 #define TX_RST  22
 #define TX_EN   20
 #define TX_IRQ  21
 //
 void spi_write_register(uint8_t reg, uint8_t value) ;
 uint8_t spi_read_register(uint8_t reg);
 void spi_write_burst(uint8_t reg, const uint8_t *data, size_t len);
 //
 void init_transmitter();
 void transmit();