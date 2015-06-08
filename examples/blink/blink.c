/* The classic "blink" example
 *
 * This sample code is in the public domain.
 */
#include "espressif/esp_common.h"
#include "espressif/sdk_private.h"
#include "FreeRTOS.h"
#include "task.h"
#include "esp8266.h"

const int gpio = 14;

/* This task uses the high level GPIO API (esp_gpio.h) to blink an LED.
 *
 * Even though it reads better than the register-level version in blinkenRegisterTask,
 * they compile to the exact same instructions (except gpio_enable also set the output type in
 * the GPIO control register).
 */
void blinkenTask(void *pvParameters)
{
    gpio_enable(gpio, GPIO_OUTPUT);
    while(1) {
	gpio_write(gpio, 1);
	vTaskDelay(1000 / portTICK_RATE_MS);
	gpio_write(gpio, 0);
	vTaskDelay(1000 / portTICK_RATE_MS);
    }
}


/* This task uses all raw register operations to set the pins.

   It's not fully parameterised, as the IOMUX_SET macro requires the pin number
   as part of the GPxx value.

   There is no significant performance benefit to this way over the
   blinkenTask version, so it's probably better to use the blinkenTask
   version.

   NOTE: This task isn't enabled by default, see the commented out line in user_init.
*/
void blinkenRegisterTask(void *pvParameters)
{
    GPIO_DIR_SET = BIT(gpio);
    IOMUX_SET(GP14,GPIO,IOMUX_OE); /* change this line if you change 'gpio' */
    while(1) {
	GPIO_OUT_SET = BIT(gpio);
	vTaskDelay(1000 / portTICK_RATE_MS);
	GPIO_OUT_CLEAR = BIT(gpio);
	vTaskDelay(1000 / portTICK_RATE_MS);
    }
}


void user_init(void)
{
    gpio_enable(13, GPIO_OUTPUT);

    sdk_uart_div_modify(0, UART_CLK_FREQ / 115200);
    xTaskCreate(blinkenTask, (signed char *)"blinkenTask", 256, NULL, 2, NULL);
    //xTaskCreate(blinkenRegisterTask, (signed char *)"blinkenRegisterTask", 256, NULL, 2, NULL);
}
