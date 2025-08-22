#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <hal/nrf_gpio.h>



/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000
#define GPIO_PIN NRF_GPIO_PIN_MAP(0,22)


int main(void)
{
	nrf_gpio_cfg_output(GPIO_PIN); // Set pin as output

	while (1) {
		nrf_gpio_pin_set(GPIO_PIN);		//set pin to HIGH (LED off)
		printk("LED is off\n");
		k_msleep(SLEEP_TIME_MS);
		nrf_gpio_pin_clear(GPIO_PIN);	//set pin to LOW (LED on)
		printk("LED is on\n");
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}
