This is a demo of a data logging sketch that is capable of logging serial
data to an SD card at up to 115200 baud.

Normally the maximum rate for a 328 Arduino is 57600 baud.

It is possible to log data at 115200 baud with a 328 Arduino if you use a high
quality SD card.  I used a 4 GB SanDisk Extreme HD video card(30 MB/s version).

Lower quality cards can have an occasional write latency of over 100
milliseconds.  This will cause a receive data overrun at 115200 baud since
the RX buffer on a 328 Arduino is 1024 bytes.  You need to reduce the baud
rate to 57600 if your SD card causes receive overruns.

Most SD cards will work with a Mega Arduino at 115200 baud since the RX
buffer is set to 4096 bytes.

It is possible to use software SPI on a Mega by setting MEGA_SOFT_SPI nonzero
in SdFatConfig.h.  This will allow a shield like the Adafruit Data Logging
shield to be used on a Mega without jumper wires.

This folder contains two libraries:

A beta version of SerialPort, a new library for Arduino USART serial ports.
This library allows the RX buffer size to be set in the SerialPort object.

A beta version of SdFat that allows uses of SerialPort with SdFat.

Extract these libraries from their zip files and install them.

The two programs, SerialPortLogger.pde and SerialDataSource.pde, demonstrate
high speed logging of serial data.

To run this demo, you need two Arduino boards and an SD shield or SD module.

The board with the SD shield should have an LED and series resistor connected
from pin 3 to ground.  This LED will blink an error code if an error occurs.

Make sure BAUD_RATE has the same value in SerialPortLogger.pde and
SerialDataSource.pde.

Load SerialPortLogger.pde into the board with an SD shield or module and
SerialDataSource.pde into the second board.

Connect a wire between GND pins on the two boards.

Connect a wire between the serial RX pin (Pin 0) on the SD board and the
serial TX pin (pin 1) on the data source board.

It is best to power the two boards with an external 9V supply.

After the boards are powered up, wait until the pin 13 LED on the data source
board goes out.  This will take up to 50 seconds.

Insert an SD into the logger board.  press reset on the logger board.  The
error LED connected to pin 3 should not light.

Press reset on the data source board.  The pin 13 LED on the data source
board should light for about 50 seconds at 57600 baud or 25 seconds at
115200 baud while 280,000 bytes are transferred.

If an error occurs, the error LED on the SD board will flash an error code.
See the SerialPortLogger.pde source for definitions of error codes.

Remove the SD and check the file SERIAL.BIN.  In this case it is a text file.