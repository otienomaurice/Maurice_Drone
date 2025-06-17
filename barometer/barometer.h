#ifndef BAROMETER_H
#define BAROMETER_H

#include <stdint.h>
#include <stdbool.h>

/* === Public API === */

// Initialize the BMP180 barometer over I2C
void barometer_init(void);

// Read and update altitude & vertical speed
void barometer_update(void);

#endif
