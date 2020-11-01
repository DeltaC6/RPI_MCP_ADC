################################################################################
MCP3008 ADC APPLICAITON
################################################################################

Dependencies:
    WiringPi library: sudo apt-get install wiringPi

Usage:
    Call the following command in the terminal of RPi.
        'make'
        or
        'make run'

HOW IT WORKS?
    This program uses two threads running in the background with the main
    application.
    
    Reader thread reads all the no of channels and sets a flag to tell the
    writer thread that data is ready to access.
    
    Writer thread on the other hand uses the data until the reader is asleep to
    create a CSV string and frees the buffer to get overwritten again and writes
    the file until next data sample is ready.

    Possiblitity of losses entirely depends on the time sleep changes.

HARDWARE TESTS?
    Still in progress...

TODO LIST:
    ...
