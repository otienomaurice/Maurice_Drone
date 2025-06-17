cmake_minimum_required(VERSION 3.13)

# Import the Pico SDK
include(pico_sdk_import.cmake)

# Define the project
project(MAURICE_DRONE C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# Define your main executable and its sources
add_executable(maurice_drone  
    drone/main.c 
    Motors/motors.c 
    global/maurice_drone_io.c
    global/timer.c
    joysticks/joystick.c
    lcd_16x2/lcd_16x2.c
    lcd_16x2/lcd_16x2_execute.c
    receiver/receiver.c
    transmitter/transmitter.c
    global/global.c
    lcd/ts_lcd.c
    lcd/button.c
    lcd/glcdfont.c
    lcd/tftmaster.c
    lcd/touchscreen.c
    Pico_to_pico_direct/pico_to_pico_direct.c
    imu/imu.c
    imu/MadgwickAHRS.c
    barometer/barometer.c
   # ✅ renamed and included correctly
)

# Link standard and peripheral libraries
target_link_libraries(maurice_drone 
    pico_stdlib
    hardware_gpio 
    hardware_pwm
    pico_time 
    hardware_timer
    hardware_uart
    hardware_watchdog
    hardware_pio
    hardware_irq
    hardware_interp
    hardware_i2c
    hardware_adc
    hardware_spi
    pico_bootsel_via_double_reset
)

# Generate PIO header for spi_cpha0_cs.pio
pico_generate_pio_header(maurice_drone ${CMAKE_CURRENT_LIST_DIR}/lcd/SPIPIO.pio)  # <-- and t

# Add extra output formats (UF2, bin, etc.)
pico_add_extra_outputs(maurice_drone)

# Add include directories
include_directories(${CMAKE_CURRENT_LIST_DIR})

# Optional: Doxygen (skip if not using it)
pico_add_doxygen(${CMAKE_CURRENT_LIST_DIR} "C:/Users/otienom/Documents/pico/pico-sdk")

# Flash config and debugging setup
pico_set_program_name(maurice_drone "maurice_drone")
pico_enable_stdio_usb(maurice_drone 1)
pico_enable_stdio_uart(maurice_drone 1)
