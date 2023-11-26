// EXPERIMENTAL CODE - for development only presently
// needs - tinyusb_device tinyusb_board

#if !defined TC_TINY_USB_CDC_HANDLER_H
#define TC_TINY_USB_CDC_HANDLER_H

#include "tusb.h"
#include "bsp/board.h"
#include "RemoteConnector.h"
#include "SCCircularBuffer.h"
#include <remote/BaseRemoteComponents.h>

namespace tcremote {

    class TinyUsbCdcHandler : public TagValueTransport {
    private:
        int idx;
        bool deviceMounted = false;
        SCCircularBuffer inputBuffer;
    public:
        TinyUsbCdcHandler(int cdcNum, int readBufferSize);

        void flush() override { tud_cdc_write_flush(); }

        int writeChar(char data) override;

        int writeStr(const char *data) override;

        uint8_t readByte() override;

        bool readAvailable() override;

        bool available() override { return tud_cdc_write_available(); }

        bool connected() override { return true; }

        void close() override;

        void mounted(bool mountedNow) { deviceMounted = mountedNow; }

        void dataArrived(uint8_t *data, size_t len);
    };
}

#ifndef TC_MANUAL_NAMESPACING
using namespace tcremote;
#endif // TC_MANUAL_NAMESPACING

#endif //TC_TINY_USB_CDC_HANDLER_H
