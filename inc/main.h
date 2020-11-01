/*!
 * @author  Syed Asad Amin
 * @date    Nov 1st, 2020
 * @file    main.h
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

#pragma once

//===== INCLUDES SECTION =====================================================//
#include <cstdio>
#include <cstdlib>

#include "adc.h"
//============================================================================//
