#include <Arduino.h>
#include <U8g2lib.h>
#include <U8x8lib.h>
#include <unistd.h>

#include "bluetooth.h"
#include "tinybms.h"
#include "util.h"
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define SERIAL_BAUD 9600
#define POLL_INTERVAL 1000

Battery_config battery_config;
Battery_voltage battery_voltage;
Battery_current battery_current;
Battery_soc battery_soc;
Battery_temp battery_temp;

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/SCL, /* data=*/SDA,
                                         /* reset=*/U8X8_PIN_NONE);

void display_nobt_msg();

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.printf("Starting TinyBMSDisplay\r\n");
  u8g2.begin();

  while (init_bluetooth() < 0) {
    Serial.printf("Bluetooth initialization failure. Retrying after sleep\r\n");
    display_nobt_msg();
    delay(10000);
  }

  init_tinybms();
  Serial.printf("Init OK\r\n");
}

void load_battery_data() {
  if (battery_config.last_success == 0) {
    load_battery_config(&battery_config);
  }
  load_battery_voltage(&battery_config, &battery_voltage);
  load_battery_current(&battery_current);
  load_battery_soc(&battery_soc);
  load_battery_temp(&battery_temp);
}

void display_data() {
  u8g2.firstPage();

  do {
    int y = 0;

    u8g2.setFont(u8g2_font_9x15B_tr);  // font for the title
    u8g2.setCursor(0, y += 15);
    u8g2.printf("SOC:     %hu%%", battery_soc.stateOfCharge);

    u8g2.setFont(u8g2_font_6x13_tr);  // font for the title
    u8g2.setCursor(0, y += 13);
    u8g2.printf("Current: %3.2fA", battery_current.pack_current);

    u8g2.setCursor(0, y += 13);
    u8g2.printf("Voltage: %3.2fV", battery_voltage.pack_voltage.fvoltage);

    u8g2.setFont(u8g2_font_5x7_tr);

    y += 5;
    u8g2.setCursor(0, y += 7);
    u8g2.printf("Cell min/max: %3.2fV %3.2fV",
                battery_voltage.min_cell_voltage / 1000.0,
                battery_voltage.max_cell_voltage / 1000.0);

    u8g2.setCursor(0, y += 7);
    u8g2.printf(
        "Temp sensors: %.0fC %.0fC %.0fC", battery_temp.temp_sensor0 / 10.0,
        battery_temp.temp_sensor1 / 10.0, battery_temp.temp_sensor2 / 10.0);

  } while (u8g2.nextPage());
}

void display_nobt_msg() {
  u8g2.firstPage();

  do {
    u8g2.setFont(u8g2_font_7x14B_tr);
    u8g2.setCursor(0, 30);
    u8g2.printf("No BT Connection");

  } while (u8g2.nextPage());
}

void loop() {
  load_battery_data();

  if (battery_config.cell_count > 0) {
    Serial.printf("SOC: %hu%%\r\n", battery_soc.stateOfCharge);
    Serial.printf("Current: %3.2fA\r\n", battery_current.pack_current);
    Serial.printf("Voltage: %3.2fV\r\n", battery_voltage.pack_voltage.fvoltage);

    Serial.printf("Cell min/max: %3.2fV %3.2fV\r\n",
                  battery_voltage.min_cell_voltage / 1000.0,
                  battery_voltage.max_cell_voltage / 1000.0);

    Serial.printf(
        "Temp sensors: %.0fC %.0fC %.0fC\r\n", battery_temp.temp_sensor0 / 10.0,
        battery_temp.temp_sensor1 / 10.0, battery_temp.temp_sensor2 / 10.0);

    display_data();
  }

  Serial.printf("Uptime: %us\r\n", millis() / 1000);
  Serial.printf("Free memory: %u\r\n\r\n", freeMemory());
  delay(POLL_INTERVAL);
}
