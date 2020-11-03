/*!
 * @author  Syed Asad Amin
 * @date    Nov 1st, 2020
 * @file    main.cpp
 * @version v1.0.5
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
 *          Binary file operation wrote 16384 samples in approx 2 seconds
 * 
 *  A project for fiverr client
 */

#include "main.h"

using namespace std;

bool isReaderRunning = false;
bool isWriterRunning = false;

pthread_t readerThread;
pthread_t writerThread;

static const char logPath[] = "/home/pi/Desktop/MCP3008.bin";

static const uint8_t NO_OF_CHANNEL = 4;
static const uint16_t SAMPLE_PER_CHANNEL = 10000;
static uint16_t adcData[NO_OF_CHANNEL][SAMPLE_PER_CHANNEL];

static uint8_t curChannel = 0;
static uint16_t curSample = 0;

// Ring buffer for IO stream
bool isFullFlag = false;
static uint16_t writePTR = 0;
static uint16_t readPTR = 0;

// Filestream
ofstream mFile;

int main(void) {
    fprintf(stdout, "Application started...\r\n");
    
    initGPIO();
    initSPI(SPI_CHANNEL_1, SPI_CLK_SPD);

    mFile.open(logPath, ios::binary | ios::out | ios::app);

    thread readerThread(adcReader);
    thread writerThread(fileWriter);

    if(readerThread.joinable()) { readerThread.join(); }
    if(writerThread.joinable()) { writerThread.join(); }
    
    delay(1000);
    deinitSPI();
    deinitGPIO();
    mFile.close();
    
    fprintf(stdout, "Application finished...\r\n");
    return EXIT_SUCCESS;
}

void adcReader(void) {
    fprintf(stdout, "Reader thread started...\r\n");
    isReaderRunning = true;
    while(isReaderRunning) {
        if(isFullFlag) { continue; }

        int16_t val = readADC(SPI_CHANNEL_1, CHAN_CONF_SING, curChannel);
        adcData[curChannel][curSample] = val;

        // Buffer the data into RAM before writing to file
        curChannel++;
        if(curChannel >= NO_OF_CHANNEL) {
            curChannel = 0;
            curSample++;
            if(curSample >= SAMPLE_PER_CHANNEL) { curSample = 0; }

            writePTR = (writePTR + 1) % SAMPLE_PER_CHANNEL;
            if(writePTR == readPTR) { isFullFlag = true; }
        }

        // This time management routine will compensate for sample capacitor
        // charge time as well
        // Time = 5Tau = 5RC = 5 * 1e3 * 20e-12 = 100ns
        std::this_thread::sleep_for(std::chrono::nanoseconds(100));
    }

    fprintf(stdout, "Reader thread finished...\r\n");
}

void fileWriter(void) {
    fprintf(stdout, "Writer thread started...\r\n");

    char msgStr[64];
    memset(msgStr, 0, sizeof(msgStr));
    isWriterRunning = true;
    while(isWriterRunning) {
        if((writePTR == readPTR) && !isFullFlag) { continue; }

        msgStr[0] = 0;
        for(int i = 0; i < NO_OF_CHANNEL; i++) {
            //sprintf(msgStr, "%s%d,", msgStr, adcData[i][readPTR]);
            mFile.write((char *) &adcData[i][readPTR], sizeof(uint16_t));
        }
        readPTR = (readPTR + 1) % SAMPLE_PER_CHANNEL;
        isFullFlag = false;
    }
    fprintf(stdout, "Writer thread finished...\r\n");
}
