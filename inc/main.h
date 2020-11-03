/*!
 * @author  Syed Asad Amin
 * @date    Nov 1st, 2020
 * @file    main.h
 * @version v1.0.2
 *          | v1.0.0 -> Initial Commit
 *          | v1.0.1 -> Adding methods to read ADC data form MCP3008 using SPI
 *          | v1.0.2 -> Created reader and writer threads for synced operations
 *          | v1.0.3 -> Added reader and writer routines
 *          | v1.0.4 -> Completely removed the file access time by making it
 *                      global. Added a ring buffer operation instead of a
 *                      simple buffer accessed by locked states
 *          | v1.0.5 -> Text file is changed to binary file since creating and
 *                      writing text is too time consuming
 * 
 * @note    This is a program written in C++ to read ADC data from IC MCP3008
 *          via SPI. MCP3008 is 8 channel, 10bit with 200KSPS max sampling rate
 * 
 *          This application read data from 4 channels at greater than 40KSPS
 *          sampling rate and write it to a log file
 * 
 *          Time management for reader and writer threads is completed. To
 *          close the threads safely just reset the isReaderRunning and 
 *          isWriterRunning flags
 * 
 *          TIME RECORDED UPON TEST (STILL LOSSY BUT ACCESPTABLE)
 *          Binary file operation wrote 16384 samples in 2 seconds
 * 
 *          [PERSONAL NOTE] This could get better if I patch the RPI kernel
 *          with RTPremept kernel module
 * 
 *  A project for fiverr client
 */


#pragma once

//===== INCLUDES SECTION =====================================================//
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include <thread>
#include <mutex>
#include <chrono>

#include "adc.h"
//============================================================================//

//===== FUNCTIONS DECLARAITONS ===============================================//
/*!
 * @brief This is the reader thread function
 * @param None
 * @return None
 */
void adcReader(void);

/*!
 * @brief This is the writer thread function
 * @param None
 * @return None
 */
void fileWriter(void);
//============================================================================//
