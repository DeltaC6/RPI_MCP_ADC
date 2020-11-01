/*!
 * @author  Syed Asad Amin
 * @date    Nov 1st, 2020
 * @file    adc.cpp
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

#include "adc.h"

static int gpiofd;
static int spifd;

void initGPIO(void) {
    if((gpiofd = wiringPiSetupGpio()) < 0) {
        fprintf(stderr, "GPIO initialization failed.\r\n%s", strerror(errno));
        return;
    }
    fprintf(stdout, "GPIO initialization complete.\r\n");
}

void initSPI(uint8_t channle, uint32_t speed) {
    if((spifd = wiringPiSPISetup(channle, speed)) < 0) {
        fprintf(stderr, "SPI initialization failed.\r\n%s", strerror(errno));
        return;
    }
    fprintf(stdout, "SPI initialization complete.\r\n");
}

int16_t readADC(uint8_t spiChannel, uint8_t confMode, int8_t adcChannel) {
    int16_t res = 0;
    if(adcChannel < 0 || adcChannel > 7) return -1;
    
    uint8_t buf[3] = {1};                                                       // Start Bit
    buf[1] = (confMode + adcChannel) << 4;                                      // Config bit and channel
    wiringPiSPIDataRW(spiChannel, buf, sizeof(buf));                            // Single mode: [0x10, 0x20...] Diff mode: [0x90, 0xA0....]
    res = ((buf[1] & 0x03) << 8) + buf[2];                                      // Result byte received duing the cycle of mode writing as well as next cycle.
    return res;
}

void deinitSPI(void) {
    close(spifd);
    fprintf(stdout, "SPI deinit complete...\r\n");
}

void deinitGPIO(void) {
    close(gpiofd);
    fprintf(stdout, "GPIO deinit complete...\r\n");
}
