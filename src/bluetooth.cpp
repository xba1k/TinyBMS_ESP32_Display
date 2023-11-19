#include "bluetooth.h"

#include <Arduino.h>

#include "util.h"

BluetoothSerial SerialBT;

int init_bluetooth(void) {
  int result = 1;

  DEBUGP("init_bt");

  if (!SerialBT.begin("TinyBMSDisplay", true)) {
    DEBUGP("Bluetooth initialization failure\r\n");
    return -1;
  }

  scan_devices();

  SerialBT.setPin(TINYBMS_PIN);

  if (SerialBT.connect(BTAddress(TINYBMS_ADDRESS), 1,
                       ESP_SPP_SEC_ENCRYPT | ESP_SPP_SEC_AUTHENTICATE,
                       ESP_SPP_ROLE_MASTER)) {
    DEBUGP("BT connected\r\n");
    result = 1;
  } else {
    DEBUGP("BT connection failed\r\n");
    result = -1;
  }

  return result;
}

int scan_devices(void) {
  int result = 1;

  BTScanResults *btDeviceList = SerialBT.discover(10000);

  if (btDeviceList && btDeviceList->getCount() > 0) {
    BTAddress addr;
    int channel = 0;
    Serial.printf("Discovered devices:\r\n");
    for (int i = 0; i < btDeviceList->getCount(); i++) {
      BTAdvertisedDevice *device = btDeviceList->getDevice(i);
      Serial.printf("%s\t%s\t%d\r\n", device->getAddress().toString().c_str(),
                    device->getName().c_str(), device->getRSSI());
    }
  }

  return result;
}
