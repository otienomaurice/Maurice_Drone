
// Updated for PIC32 by Matthew Watkins
// Updated for RP2040 by John Nestor

// Touch screen library with X Y and Z (pressure) readings as well
// as oversampling to avoid 'bouncing'
// (c) ladyada / adafruit
// Code under MIT License

#include "stdio.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "TouchScreen.h"
#include "stdbool.h"


// increase or decrease the touchscreen oversampling. This is a little different than you may think:
// 1 is no oversampling, whatever data we get is immediately returned
// 2 is double-sampling and we only return valid data if both points are the same
// 3+ uses insert sort to get the median value.
// Adafruit: We found 2 is precise yet not too slow so we suggest sticking with it!
// MWatkins: With parallel communication found 3 samples more reliable

#define NUMSAMPLES 3

#if (NUMSAMPLES > 2)
static void insert_sort(int array[], uint8_t size)
{
  uint8_t j;
  int s; // was save

  int i;
  for (i = 1; i < size; i++)
  {
    s = array[i];
    for (j = i; j >= 1 && s < array[j - 1]; j--)
      array[j] = array[j - 1];
    array[j] = s;
  }
}
#endif

void setTSPoint(struct TSPoint *p, int16_t x0, int16_t y0, int16_t z0)
{
  p->x = x0;
  p->y = y0;
  p->z = z0;
}

void getPoint(struct TSPoint *p)
{
  int x = 0, y = 0, z = 0;
  int samples[NUMSAMPLES];
  uint8_t i, valid;

  valid = 1;
  // read the X position
  gpio_init(YMbit);
  gpio_set_dir(YMbit, false); // set YM as input (high impedance
  adc_gpio_init(YPbit);       // set YP as analog input
  adc_select_input(YPchan);

  gpio_init(XPbit);
  gpio_set_dir(XPbit, true);
  gpio_put(XPbit, 1); // drive H on XP
  gpio_init(XMbit);
  gpio_set_dir(XMbit, true);
  gpio_put(XMbit, 0); // drive L on XM
  for (i = 0; i < NUMSAMPLES; i++)
  {
    samples[i] = adc_read();
  }
#if NUMSAMPLES > 2
  insert_sort(samples, NUMSAMPLES);
#endif
#if NUMSAMPLES == 2
  if (samples[0] != samples[1])
  {
    valid = 0;
  }
#endif
  x = (4095 - samples[NUMSAMPLES / 2]);

  // now read the y position
  gpio_init(XPbit);
  gpio_set_dir(XPbit, false); // set XP as input (high impedance
  adc_gpio_init(XMbit);       // set XM as analog input
  adc_select_input(XMchan);

  gpio_init(YPbit);
  gpio_set_dir(YPbit, true);
  gpio_put(YPbit, 1); // drive H on YP
  gpio_init(YMbit);
  gpio_set_dir(YMbit, true);
  gpio_put(YMbit, 0); // drive L on YM

  for (i = 0; i < NUMSAMPLES; i++)
  {
    samples[i] = adc_read();
  }

#if NUMSAMPLES > 2
  insert_sort(samples, NUMSAMPLES);
#endif
#if NUMSAMPLES == 2
  if (samples[0] != samples[1])
  {
    valid = 0;
  }
#endif

  y = (4095 - samples[NUMSAMPLES / 2]);

  // Now read the "z" value

  // Set X+ to ground
  gpio_init(XPbit);
  gpio_set_dir(XPbit, true);
  gpio_put(XPbit, 0); // drive L on XP

  // Set Y- to VCC
  gpio_init(YMbit);
  gpio_set_dir(YMbit, true);
  gpio_put(YMbit, 1);

  // Make X- and Y+ analog inputs
  adc_gpio_init(XMbit); // set XM as analog input
  adc_gpio_init(YPbit); // set XM as analog input

  adc_select_input(XMchan);
  int z1 = adc_read();
  adc_select_input(YPchan);
  int z2 = adc_read();

  if (RXPLATE != 0)
  {
    // now read the x
    float rtouch;
    rtouch = z2;
    rtouch /= z1;
    rtouch -= 1;
    rtouch *= x;
    rtouch *= RXPLATE;
    rtouch /= 4096;
    if (rtouch > 32767.0)
      z = 32767;
    else
      z = rtouch;
  }
  else
  {
    z = (4095 - (z2 - z1));
  }

  if (!valid)
  {
    z = 0;
  }

  printf("done?\n");
  setTSPoint(p, x, y, z);
}
