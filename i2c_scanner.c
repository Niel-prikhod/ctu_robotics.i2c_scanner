#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#define MAX_SENSORS 10
#define ADDR_STRLEN 5
// I2C reserves some addresses for special purposes. We exclude these from the scan.
// These are any addresses of the form 000 0xxx or 111 1xxx
bool reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

int main() {
	char detected_sensors[MAX_SENSORS][ADDR_STRLEN];
	int sensor_count = 0;

    // Enable UART so we can print status output
    stdio_init_all();
	gpio_init(PICO_DEFAULT_LED_PIN);
	gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    // This example will use I2C0 on the default SDA and SCL pins (GP4, GP5 on a Pico)
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));
	while (1)
	{
		sensor_count = 0;
		gpio_put(PICO_DEFAULT_LED_PIN, 1);
		printf("\nI2C Bus Scan\n");
		printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

		for (int addr = 0; addr < (1 << 7); ++addr) {
			if (addr % 16 == 0) {
				printf("%02x ", addr);
			}

        // Perform a 1-byte dummy read from the probe address. If a slave
        // acknowledges this address, the function returns the number of bytes
        // transferred. If the address byte is ignored, the function returns
        // -1.

        // Skip over any reserved addresses.
			int ret;
			uint8_t rxdata;
			if (reserved_addr(addr))
				ret = PICO_ERROR_GENERIC;
			else
				ret = i2c_read_blocking(i2c_default, addr, &rxdata, 1, false);

			printf(ret < 0 ? "." : "@");
			if (ret >= 0){
				snprintf(detected_sensors[sensor_count], ADDR_STRLEN, "0x%02X", addr);
				sensor_count++;
			}
			printf(addr % 16 == 15 ? "\n" : "  ");
		}
		printf("Done.\n");
		if (sensor_count == 0)
            printf("none\n");
        else
        {
            for (int i = 0; i < sensor_count; ++i)
                printf("%s%s", detected_sensors[i], i < sensor_count - 1 ? ", " : "\n");
        }
		sleep_ms(1000);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        sleep_ms(1000);
	}
    return 0;
//#endif
}
