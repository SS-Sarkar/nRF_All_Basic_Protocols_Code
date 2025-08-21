/*
 * Reading ADC Value over Analog pin A0(P0.04 For nRF5340DK and P0.02 for other boards)
 * Converting RAW ADC value into mV
 * And printing/LOGGING it over Terminal
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/adc.h>

// Define a variable of type adc_dt_spec for each channel
static const struct adc_dt_spec adc_channel = ADC_DT_SPEC_GET(DT_PATH(zephyr_user));

// LOG_MODULE_REGISTER( Reading_ADC_Value, LOG_LEVEL_DBG);	//Enable if you Want LOGGING

int main(void)
{
	int err;
	uint32_t count = 0;

	int16_t buf;
	struct adc_sequence sequence = {
		.buffer = &buf,
		/* buffer size in bytes, not number of samples */
		.buffer_size = sizeof(buf),
	};

	/* validate that the ADC peripheral (SAADC) is ready */
	if (!adc_is_ready_dt(&adc_channel)) {

		// LOG_ERR("ADC controller devivce %s not ready", adc_channel.dev->name);
		printk("ADC controller devivce %s not ready \n", adc_channel.dev->name);

		return 0;
	}
	/* Setup the ADC channel */
	err = adc_channel_setup_dt(&adc_channel);
	if (err < 0) {

		// LOG_ERR("Could not setup channel #%d (%d)", 0, err);
		printk("Could not setup channel #%d (%d)\n", 0, err);

		return 0;
	}
	/* Initialize the ADC sequence */
	err = adc_sequence_init_dt(&adc_channel, &sequence);
	if (err < 0) {

		// LOG_ERR("Could not initalize sequnce");
		printk("Could not initalize sequnce\n");

		return 0;
	}

	while (1) {
		int val_mv;

		/* Read a sample from the ADC */
		err = adc_read(adc_channel.dev, &sequence);
		if (err < 0) {

			// LOG_ERR("Could not read (%d)", err);
			printk("Could not read (%d)\n", err);

			continue;
		}

		val_mv = (int)buf;

		// LOG_INF("ADC reading[%u]: %s, channel %d: Raw: %d", count++, adc_channel.dev->name,adc_channel.channel_id, val_mv);
		printk("ADC reading[%u]: %s, channel %d: Raw ADC: %d ", count++, adc_channel.dev->name,adc_channel.channel_id, val_mv);


		/* Convert raw ADC value to mV*/
		err = adc_raw_to_millivolts_dt(&adc_channel, &val_mv);
		/* conversion to mV may not be supported, skip if not */
		if (err < 0) {

			// LOG_WRN(" (value in mV not available)\n");
			printk(" (value in mV not available)\n");
		} else {

			// LOG_INF(" = %d mV", val_mv);
			printk(" = %d mV\n", val_mv);
		}


		k_sleep(K_MSEC(100));
	}
	return 0;
}
