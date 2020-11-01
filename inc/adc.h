/*!
 * @author  Syed Asad Amin
 * @date    Nov 1st, 2020
 * @file    adc.h
 * @version v1.0.1
 *          | v1.0.0 -> Initial Commit
 *          | v1.0.1 -> Complete library
 * 
 * @note    This is a library written in c++ for reading data froi MCP3008 IC
 *          using SPI
 * 
 *          Usage example:
 *              initGPIO();
 *              initSPI();
 *              val = readADC(...);
 * 
 */

#pragma once

//===== INCLUDES SECTION =====================================================//
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cerrno>
#include <unistd.h>

#include <wiringPi.h>
#include <wiringPiSPI.h>
//============================================================================//

//===== CONSTANT AND VARIABLE DEFINITIONS SECTION ============================//
// SPI Channel
#define SPI_CHANNEL_0   0U
#define SPI_CHANNEL_1   1U

// SPI Clock Speed
#define SPI_CLK_MIN     500000UL
#define SPI_CLK_MAX     32000000UL
#define SPI_CLK_SPD     1000000UL

// SPI Mode
#define SPI_MODE_0      0U
#define SPI_MODE_1      1U
#define SPI_MODE_2      2U
#define SPI_MODE_3      3U

// MCP3008 Mode
#define CHAN_CONF_SING  8U
#define CHAN_CONF_DIFF  0U
//============================================================================//

//===== FUNCTION DEFINITIONS SECTION =========================================//
/*!
 * @brief This function initializes the GPIO using wiring pi library for RPi
 * @param None
 * @return None
 */
void initGPIO(void);

/*!
 * @brief This function initializes the SPI using wiring pi library for RPi
 * @param [uint8_t] SPI Channel
 * @param [uint32_t] SPI Speed
 * @return None
 */
void initSPI(uint8_t, uint32_t);

/*!
 * @brief This function reads the adc chip MCP3008 via SPI
 * @param [uint8_t] SPI channel used
 * @param [uint8_t] Config mode single or differential
 * @param [uint8_t] Channel selection from 0 to 7
 * @return [int16_t] Digital data value of converted analog signal
 */
int16_t readADC(uint8_t, uint8_t, int8_t);

/*!
 * @brief This function deinit the SPI
 * @param None
 * @return None
 */
void deinitSPI(void);

/*!
 * @brief This function deinit the GPIO
 * @param None
 * @return None
 */
void deinitGPIO(void);
//============================================================================//
