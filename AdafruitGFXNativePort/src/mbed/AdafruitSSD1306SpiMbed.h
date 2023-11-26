
#ifndef _ADAFRUIT_MBEDFORK_SSD1306_SPI_H
#define _ADAFRUIT_MBEDFORK_SSD1306_SPI_H

#include "IoAbstraction.h"
#include "../Adafruit_GFX.h"
#include "../Adafruit_SSD1306.h"

/** This is the SPI SSD1306 display driver transport class
 *
 */
class Adafruit_SSD1306_Spi : public Adafruit_SSD1306
{
public:
    /** Create a SSD1306 SPI transport display driver instance with the specified DC, RST, and CS pins, as well as the display dimentions
     *
     * Required parameters
     * @param spi - a reference to an initialized SPI object
     * @param DC (Data/Command) pin name
     * @param RST (Reset) pin name
     * @param CS (Chip Select) pin name
     *
     * Optional parameters
     * @param rawHeight - the vertical number of pixels for the display, defaults to 32
     * @param rawWidth - the horizonal number of pixels for the display, defaults to 128
     */
    Adafruit_SSD1306_Spi(SPI &spi, pinid_t DC, pinid_t RST, pinid_t CS, uint16_t rawHieght = 32, uint16_t rawWidth = 128, OledType displayType = SSD_1306)
            : Adafruit_SSD1306(RST, rawHieght, rawWidth)
            , mDisplayType(displayType)
            , mspi(spi)
    {
        dcPin = DC;
        csPin = CS;
        internalDigitalDevice().pinMode(dcPin, OUTPUT);
        internalDigitalDevice().pinMode(csPin, OUTPUT);
        internalDigitalDevice().digitalWriteS(dcPin, HIGH);
        internalDigitalDevice().digitalWriteS(csPin, HIGH);
    }

    void toggleDC(int dcState)
    {
        internalDigitalDevice().digitalWriteS(csPin, HIGH);
        internalDigitalDevice().digitalWriteS(dcPin, dcState);
        delayMicroseconds(1);
        internalDigitalDevice().digitalWriteS(csPin, LOW);
    }

    void command(uint8_t c) override
    {
        toggleDC(0);
        mspi.write(c);
        internalDigitalDevice().digitalWriteS(csPin, HIGH);
    };

    void data(uint8_t c) override
    {
        toggleDC(1);
        mspi.write(c);
        internalDigitalDevice().digitalWriteS(csPin, HIGH);
    };

protected:
    void sendDisplayBuffer() override
    {
        auto rows = HEIGHT / 8;
        auto col = 0U;
        for(uint8_t row=0; row < rows; row++) {
            // for each row we go into command mode and send the new row offset, SH1106 cannot do this
            // automatically, so we need to do it for it.
            command(0xB0 + row);//set page address
            command(col & 0xfU);//set lower column address
            command(0x10U | (col >> 4U));//set higher column address

            toggleDC(1);

            // now we prepare a whole row of data to send over SPI.
            for(int i=0;i<WIDTH;i++) {
                mspi.write(buffer[(row * WIDTH) + i]);
            }
        }

        // make sure we de-select the chip
        internalDigitalDevice().digitalWriteS(csPin, HIGH);

    };

    pinid_t csPin, dcPin;
    OledType mDisplayType;
    SPI &mspi;
};

#endif //_ADAFRUIT_MBEDFORK_SSD1306_SPI_H
