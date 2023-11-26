
#include <PlatformDeterminationWire.h>
#include "AdafruitSSD1306I2c.h"

void AdafruitSSD1306I2c::command(uint8_t c) {
    uint8_t buff[2];
    buff[0] = 0; // Command Mode
    buff[1] = c;
    ioaWireWriteWithRetry(wireInstance, mi2cAddress, buff, sizeof(buff));
}

void AdafruitSSD1306I2c::data(uint8_t c) {
    uint8_t buff[2];
    buff[0] = 0x40; // Data Mode
    buff[1] = c;
    ioaWireWriteWithRetry(wireInstance, mi2cAddress, buff, sizeof(buff));
}

void AdafruitSSD1306I2c::sendDisplayBuffer() {
    uint8_t buff[133];

    auto rows = uint8_t(HEIGHT / 8);
    uint16_t col = 0;
    for(uint8_t row=0; row < rows; row++) {
        // for each row we go into command mode and send the new row offset, SH1106 cannot do this
        // automatically, so we need to do it, and it's compatible with SSD1306 too.
        command(0xB0 + row);//set page address
        command(col & 0xfU);//set lower column address
        command(0x10U | (col >> 4U));//set higher column address

        buff[0] = 0x40; // start data mode
        // now we prepare a whole row of data, no need to mess around with smaller segments, then dump
        // the lot into i2c.
        for(int i=0;i<WIDTH;i++) {
            buff[i + 1] = buffer[(row * WIDTH) + i];
        }
        ioaWireWriteWithRetry(wireInstance, mi2cAddress, buff, WIDTH);

#ifndef TC_OLED_I2C_TIGHT_LOOP
        // by default assume we are running in a task manager / menu environment where the amount of latency provided
        // by I2C would be too high, and let other tasks run. This doesn't speed anything up, just makes it look faster.
        taskManager.yieldForMicros(1);
#endif

    }
}
