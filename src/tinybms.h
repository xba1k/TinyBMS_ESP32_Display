#ifndef TINYBMS_H
#define TINYBMS_H

#include <Arduino.h>
#include <ModbusMaster.h>

#include "bluetooth.h"

#define serial (&SerialBT)

#define TINYBMS_ADDRESS "00:18:e4:08:20:1b"
#define TINYBMS_PIN "0516"

#define MODBUS_BAUD 115200
#define READ_TIMEOUT 2000

#define TINYBMS_DEVICE_ID 0xAA
#define MODBUS_INTERVAL 100
#define MODBUS_RETRY_INTERVAL 100
#define MODBUS_RETRY_COUNT 10

#define CELL_COUNT_REGISTER 53
#define PACK_VOLTAGE_REGISTER_0 36
#define PACK_CURRENT_REGISTER_0 38
#define MIN_CELL_VOLTAGE_REGISTER 40
#define MAX_CELL_VOLTAGE_REGISTER 41
#define PACK_CAPACITY_REGISTER 306
#define PACK_SOC_REGISTER_0 46

#define TEMP_SENSOR_1_REGISTER 42
#define TEMP_SENSOR_2_REGISTER 43
#define TEMP_SENSOR_0_REGISTER 48

extern ModbusMaster *modbus;

typedef struct _battery_temp {
  uint16_t temp_sensor0;
  uint16_t temp_sensor1;
  uint16_t temp_sensor2;

  unsigned long last_success;

} Battery_temp;

typedef struct _battery_voltage {

  union _pack_voltage {
    float fvoltage;
    uint16_t ivoltage[2];

  } pack_voltage;

  uint16_t min_cell_voltage;
  uint16_t max_cell_voltage;

  unsigned long last_success;

} Battery_voltage;

typedef struct _battery_current {
  float pack_current;

  unsigned long last_success;

} Battery_current;

typedef struct _battery_config {
  uint16_t cell_count;
  uint16_t capacity;

  unsigned long last_success = 0;

} Battery_config;

typedef struct _battery_soc {
  uint16_t stateOfCharge;
  uint32_t stateOfChargeHp;

  unsigned long last_success;

} Battery_soc;

void init_tinybms();
int readRegistersWithRetry(uint16_t idx, uint16_t count, uint16_t *dest,
                           uint8_t retrcnt);
int load_battery_voltage(Battery_config *config, Battery_voltage *voltage);
int load_battery_current(Battery_current *current);
int load_battery_config(Battery_config *config);
int load_battery_soc(Battery_soc *soc);
int load_battery_temp(Battery_temp *temp);
int read_register(uint16_t idx, uint8_t count, uint16_t *dest);

#endif
