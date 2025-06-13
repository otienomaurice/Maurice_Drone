#include "Pico_to_Pico_direct.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "global/global.h"
#include <stdio.h>
void init_uart() {
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART); // Optional
}

void uart_send(){
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "%d %d\n", x_1, y_1);
        uart_puts(UART_ID, buffer);  // Send over UART
        sleep_ms(1000);
    }

 void uart_read(){
    char buffer[64];
    int idx = 0;
        while (uart_is_readable(UART_ID)) {
        char ch = uart_getc(UART_ID);
        putchar(ch);  // Echo raw char

        if (ch == '\n') {
            buffer[idx] = '\0';
            if (sscanf(buffer, "%d %d", &horizontal1, &vertical1) == 2) {
               // printf("\nParsed: horizontal = %d, vertical = %d\n", horizontal1, vertical1);
            } else {
               // printf("\nInvalid data: %s\n", buffer);
            }
            idx = 0;
        } else if (idx < sizeof(buffer) - 1) {
            buffer[idx++] = ch;
        }
}
 }
 
