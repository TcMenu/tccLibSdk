// EXPERIMENTAL CODE - for development only presently
// needs - tinyusb_device tinyusb_board

#include "tusb.h"
#include "TinyUsbCdcHandler.h"

TinyUsbCdcHandler* handler[2] = {nullptr};

int TinyUsbCdcHandler::writeStr(const char *data) {
    auto dl = strlen(data);
    tud_cdc_write(&data, dl);
    return (int)dl;
}

uint8_t TinyUsbCdcHandler::readByte() {
    if (inputBuffer.available()) {
        return inputBuffer.get();
    }
    return -1;
}

void TinyUsbCdcHandler::close() {
    currentField.msgType = UNKNOWN_MSG_TYPE;
    currentField.fieldType = FVAL_PROCESSING_AWAITINGMSG;
}

void TinyUsbCdcHandler::dataArrived(uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        inputBuffer.put(data[i]);
    }
    serlogHexDump(SER_NETWORK_DEBUG, "CDC data arrived ", data, len);
}

int TinyUsbCdcHandler::writeChar(char data) {
    tud_cdc_write(&data, 1);
    return 1;
}

TinyUsbCdcHandler::TinyUsbCdcHandler(int cdcNum, int readBufferSize) : idx(cdcNum), inputBuffer(readBufferSize),
                                                                       TagValueTransport(TVAL_UNBUFFERED) {
    handler[cdcNum] = this;
}

bool TinyUsbCdcHandler::readAvailable() {
    tud_cdc_write("abc", 3);
    if (tud_cdc_available()) {
        uint8_t buf[64];
        uint32_t count = tud_cdc_read(buf, sizeof(buf));
        for (size_t i = 0; i < count; i++) {
            inputBuffer.put(buf[i]);
        }
    } else {
        return false;
    }
    return inputBuffer.available();
}
