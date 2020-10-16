
/**
 * @file gpiodev.h
 * @author Sunip K. Mukherjee (sunipkmukherjee@gmail.com)
 * @brief Header file containing function prototypes and properties of
 * the GPIO sysfs access module.
 * @version 0.1
 * @date 2020-08-30
 * 
 * @copyright Copyright (c) 2020
 * 
 * @license GPL v3
 */

#ifndef SH_GPIODEV_H
#define SH_GPIODEV_H

#include <macros.h>
#include <stdint.h>
#include <pthread.h>

#define GPIO_LOW 0  /// Low voltage on GPIO
#define GPIO_HIGH 1 /// High voltage on GPIO
#define GPIO_IN 0
#define GPIO_OUT 1
#define GPIO_INOUT 2
#define GPIO_FNAME_MAX_LEN 256

#ifdef GPIODEV_INTERNAL
/**
 * @brief GPIO look up table for easy access. Currently mapped to Raspberry Pi
 * system. The indices map the BCM pins to physical pins
 */
int __gpiodev_gpio_lut_pins[] = {-1,
    -1, // Pin 1, 3V3
    -1, // Pin 2, 5V
    -1, // Pin 3, I2C1 SDA
    -1, // Pin 4, GND
    -1, // Pin 5, I2C1 SCL
    -1, // Pin 6, GND
    4,  // Pin 7, GPCLK0
    -1, // Pin 8, UART TX
    -1, // Pin 9, GND
    -1, // Pin 10, UART RX
    17, // Pin 11
    18, // Pin 12, PCM CLK
    27, // Pin 13
    -1, // Pin 14, GND
    22, // Pin 15
    23, // Pin 16
    -1, // Pin 17, 3V3 Power
    24, // Pin 18
    -1, // Pin 19, SPI0 MOSI
    -1, // Pin 20, GND
    -1, // Pin 21, SPI0 MISO
    25, // Pin 22
    -1, // Pin 23, SPI0 SCLK
    -1, // Pin 24, SPI0 CE0
    -1, // Pin 25, GND
    -1, // Pin 26, SPI0 CE1
    0,  // Pin 27, EEPROM SDA
    1,  // Pin 28, EEPROM SCL
    5,  // Pin 29
    -1, // Pin 30, GND
    6,  // Pin 31
    12, // Pin 32, PWM0
    13, // Pin 33, PWM1
    -1, // Pin 34, GND
    19, // Pin 35, PCM FS
    16, // Pin 36
    26, // Pin 37
    20, // Pin 38, PCM DIN
    -1, // Pin 39, GND
    21, // Pin 40, PCM DOUT
};

/**
 * @brief Number of avaliable GPIO pins in the system.
 */
const int NUM_GPIO_PINS = sizeof(__gpiodev_gpio_lut_pins) / sizeof(int);

/**
 * @brief Structure containing complete definition of the GPIO pin system, used
 * only by the init and destroy functions. Read and write functions use a subset
 * for cache purposes.
 */
typedef struct
{
    int *fd_val;   /// File descriptors containing the GPIO inout values
    uint8_t *mode; /// I/O modes of the GPIO pin, can assume values GPIO_IN, GPIO_OUT and GPIO_INOUT
    uint8_t *val;  /// Last read/set value of the GPIO pin
    int *fd_mode;  /// File descriptors to IO modes of the GPIO pins
    int fd_export;               /// File descriptor for GPIO export
    int fd_unexport;             /// File descriptor for GPIO unexport
} gpioprops;

/** @brief Structure containing information for GPIO pin access.
 * Small footprint for cache optimizations. The pointers must be
 * populated at initialization.
 */
typedef struct
{
    int *fd;       /// File descriptions for accessing GPIO inout
    uint8_t *val;  /// Value of last read/set values
    uint8_t *mode; /// Mode of the GPIO pin
} gpiopins;
#endif // GPIODEV_INTERNAL

/**
 * @brief Initialize GPIO sysfs subsystem. Must be called before calling gpioSetMode().
 */
int gpioInitialize(void);
/**
 * @brief Free allocated memory for GPIO pins etc.
 */
void gpioDestroy(void);

/**
 * @brief Set mode of GPIO Pins.
 * 
 * @param pin of type int, corresponds to the LUT index
 * @param mode of type int, either GPIO_IN or GPIO_OUT
 * 
 * @returns Positive on success, negative on failure
 */
int gpioSetMode(int pin, int mode);

/**
 * @brief Write values GPIO_HIGH or GPIO_LOW to the GPIO pin indicated.
 * gpioRead or gpioWrite WILL NOT WORK if this function is not called
 * prior to GPIO use.
 * 
 * @param pin of type int, corresponds to the LUT index
 * @param val of type int, either GPIO_HIGH or GPIO_LOW
 * 
 * @returns Positive on success, negative on failure
 */
int gpioWrite(int pin, int val);

/**
 * @brief Read value of the GPIO pin indicated.
 * 
 * @param pin of type int, corresponds to the LUT index
 * 
 * @returns The state of the pin, or error if not GPIO_LOW or GPIO_HIGH
 */
int gpioRead(int pin);

#endif // SH_GPIODEV_H