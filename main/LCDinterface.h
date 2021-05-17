#ifndef LCDINTERFACE
#define LCDINTERFACE

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

int8_t E_GPIO;
int8_t RS_GPIO;
int8_t D4_GPIO;
int8_t D5_GPIO;
int8_t D6_GPIO;
int8_t D7_GPIO;

int8_t pins[6];

char ldata = 0;

void initialization();
void lcd_command(unsigned char cmd, bool type);
void set_nibble(unsigned char bits);
void write_word(const char *word);
void setupLCD(int8_t pins[6]);

#endif