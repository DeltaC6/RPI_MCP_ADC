/*!
 * @author  Syed Asad Amin
 * @date    Nov 1st, 2020
 * @file    main.cpp
 * @version v1.0.0
 *          | v1.0.0 -> Initial Commit
 *          | v1.0.1 -> Adding methods to read ADC data form MCP3008 using SPI
 *          | v1.0.2 -> 
 *          |
 * 
 * @note    This is a program written in C++ to read ADC data from IC MCP3008
 *          via SPI. MCP3008 is 8 channel, 10bit with 200KSPS max sampling rate
 * 
 *          This application read data from 4 chanels at greater than 40KSPS
 *          sampling rate and write it to a log file
 * 
 *          Time management for reader and writer threads needed
 * 
 *  A project for fiverr client.
 * 
 */

#include "main.h"

using namespace std;

bool isRunning = true;

int main(void) {
    fprintf(stdout, "Application started...\r\n");
    
    initGPIO();
    initSPI(SPI_CHANNEL_1, SPI_CLK_SPD);

    // TODO CODE HERE... Create reader thread

    // TODO CODE HERE... Create writer thread

    // Program should never reach here when using thread until they are closed
    while(isRunning);
    deinitSPI();
    deinitGPIO();
    
    fprintf(stdout, "Application finished...\r\n");
    return EXIT_SUCCESS;
}
