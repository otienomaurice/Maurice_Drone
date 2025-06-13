

#define UART_ID uart0
#define BAUD_RATE 9600

#define UART_TX_PIN 0
#define UART_RX_PIN 1  // Unused here, but needed to init UART

void uart_send();
void uart_read();
void init_uart();