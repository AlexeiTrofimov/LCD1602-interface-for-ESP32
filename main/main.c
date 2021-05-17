
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#define E_GPIO 23
#define RS_GPIO 19
#define D4_GPIO 18
#define D5_GPIO 17
#define D6_GPIO 16
#define D7_GPIO 21
char ldata = 0;
int pins[6] = {E_GPIO, RS_GPIO, D4_GPIO, D5_GPIO, D6_GPIO, D7_GPIO};

void initialization();
void lcd_command(unsigned char cmd, bool type);
void set_nibble(unsigned char bits);
void write_word(const char *word);

void app_main(void)
{

	for (int i = 0; i < 6; ++i)
	{
		/* Setting pins as GPIO */
		gpio_pad_select_gpio(pins[i]);

		/* Set the GPIO as a push/pull output */
		gpio_set_direction(pins[i], GPIO_MODE_OUTPUT);

		gpio_set_level(pins[i], 0);
	}

	initialization();

	write_word("Hello There");
}

void lcd_command(unsigned char cmd, bool type)
{

	ldata = (cmd & 0xf0);
	ldata = ldata >> 4;
	set_nibble(ldata); //Sending higher nibble of command

	gpio_set_level(RS_GPIO, type);

	gpio_set_level(E_GPIO, 1);
	vTaskDelay(10 / portTICK_PERIOD_MS);
	gpio_set_level(E_GPIO, 0);

	vTaskDelay(1 / portTICK_PERIOD_MS);

	ldata = (cmd & 0x0f);

	set_nibble(ldata); //Sending lower nibble of command

	gpio_set_level(E_GPIO, 1);
	vTaskDelay(10 / portTICK_PERIOD_MS);
	gpio_set_level(E_GPIO, 0);

	vTaskDelay(3 / portTICK_PERIOD_MS);
}

void initialization()
{
	vTaskDelay(15 / portTICK_PERIOD_MS); //power on delay

	lcd_command(0x02, false); // 4bit mode on

	lcd_command(0x28, false); // init 5*7 matrix
	lcd_command(0x01, false); // clear display
	lcd_command(0x0C, false); // cursor off
	lcd_command(0x06, false); // shift cursor to right
}

void set_nibble(unsigned char c)
{
	for (int i = 0; i < 4; ++i)
	{
		int b = ((c >> i) & 1);
		gpio_set_level(pins[i + 2], b);
	}
}

void write_word(const char *word)
{
	while ((*word) != 0)
	{
		lcd_command(*word, true);
		word++;
	}
}
