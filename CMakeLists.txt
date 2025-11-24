cmake_minimum_required(VERSION 3.13)

# Import the Pico SDK (assumes PICO_SDK_PATH is set, or copy the import file)
include(pico_sdk_import.cmake)

project(i2c_scanner)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialize the Pico SDK
pico_sdk_init()

# Create executable from our source
add_executable(i2c_scanner
    i2c_scanner.c
	display.c
)

# Link the Pico standard library (GPIO, timing, etc.)
target_link_libraries(i2c_scanner
	hardware_i2c
	pico_stdlib
	pico_stdio_usb
)

# Optional: enable USB stdio if needed (comment out if not used)
pico_enable_stdio_usb(i2c_scanner 1)
pico_enable_stdio_uart(i2c_scanner 0)

# Generate UF2, HEX, BIN, MAP, DIS outputs for easy flashing
pico_add_extra_outputs(i2c_scanner)  # creates blink.uf2:contentReference[oaicite:7]{index=7}
