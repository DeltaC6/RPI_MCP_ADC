/*!
 * @author  Syed Asad Amin
 * @date    Nov 1st, 2020
 * @file    main.cpp
 * @version v1.0.3
 *          | v1.0.0 -> Initial Commit
 *          | v1.0.1 -> Adding methods to read ADC data form MCP3008 using SPI
 *          | v1.0.2 -> Created reader and writer threads for synced operations
 *          | v1.0.3 -> Added reader and writer routines
 *          |
 * 
 * @note    This is a program written in C++ to read ADC data from IC MCP3008
 *          via SPI. MCP3008 is 8 channel, 10bit with 200KSPS max sampling rate
 * 
 *          This application read data from 4 chanels at greater than 40KSPS
 *          sampling rate and write it to a log file
 * 
 *          Time management for reader and writer threads is completed. To
 *          close the threads safely just reset the isReaderRunning and 
 *          isWriterRunning flags.
 * 
 *  A project for fiverr client
 * 
 */

#include "main.h"

using namespace std;

bool isReaderRunning = true;
bool isWriterRunning = true;

pthread_t readerThread;
pthread_t writerThread;
mutex dataMutex;

static const char logPath[] = "/home/pi/Desktop/MCP3008.log";
static const uint8_t NO_OF_CHANNEL = 4;
bool dataRdyFlag = false;
int16_t adcData[NO_OF_CHANNEL];

int main(void) {
    fprintf(stdout, "Application started...\r\n");
    
    initGPIO();
    initSPI(SPI_CHANNEL_1, SPI_CLK_SPD);

    thread readerThread(adcReader);
    thread writerThread(fileWriter);

    if(readerThread.joinable()) { readerThread.join(); }
    if(writerThread.joinable()) { writerThread.join(); }

    // Program should never reach here when using thread until they are closed
    deinitSPI();
    deinitGPIO();
    
    fprintf(stdout, "Application finished...\r\n");
    return EXIT_SUCCESS;
}

void adcReader(void) {
    fprintf(stdout, "Reader thread started...\r\n");

    static uint8_t curChannel = 0;
    while(isReaderRunning) {
        int16_t val = readADC(SPI_CHANNEL_1, CHAN_CONF_SING, curChannel);

        dataMutex.lock();
        adcData[curChannel] = val;
        dataMutex.unlock();

        curChannel++;
        if(curChannel >= NO_OF_CHANNEL) { 
            curChannel = 0;
            dataRdyFlag = true;
        }
        
        // This time management routine will compensate for sample capacitor
        // charge time as well
        // Time = 5Tau = 5RC = 5 * 1e3 * 20e-12 = 0.12us
        // Chnage time as low as 1us to increase sampled data
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }

    fprintf(stdout, "Reader thread finished...\r\n");
}

void fileWriter(void) {
    fprintf(stdout, "Writer thread started...\r\n");

    char msgStr[64];
    char tempStr[32];
    memset(msgStr, 0, sizeof(msgStr));

    // Creating header for CSV file
    sprintf(msgStr, "CH1,CH2,CH3,CH4,\r\n");
    writeFile(logPath, msgStr);

    while(isWriterRunning) {
        if(dataRdyFlag) {
            memset(msgStr, 0, sizeof(msgStr));

            dataMutex.lock();
            for(int i = 0; i < NO_OF_CHANNEL; i++) {
                sprintf(tempStr, "%d,", adcData[i]);
                strcat(msgStr, tempStr);
            }
            dataMutex.unlock();

            writeFile(logPath, msgStr);
            dataRdyFlag = false;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    fprintf(stdout, "Writer thread finished...\r\n");
}

void writeFile(const char *file, const char *data) {
    ofstream mFile;
    mFile.open(file, ios::out | ios::app);
    mFile << data << endl;
    mFile.close();
}
