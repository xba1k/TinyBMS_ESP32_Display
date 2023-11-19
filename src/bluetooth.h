#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>
#include <BluetoothSerial.h>

#include "tinybms.h"

extern BluetoothSerial SerialBT;

#define BT_BAUD 115200
#define BT_DISCOVER_TIME 10000

int init_bluetooth(void);
int scan_devices(void);

#endif
