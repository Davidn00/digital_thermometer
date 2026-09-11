#ifndef CONFIG_H
#define CONFIG_H

#define _XTAL_FREQ 20000000UL

/*
 * ==========================================================
 * XC8 interrupt compatibility with VS Code IntelliSense
 * ==========================================================
 *
 * XC8 provides the __interrupt() function attribute.
 *
 * IntelliSense does not understand this XC8 extension,
 * therefore the attribute is hidden from IntelliSense only.
 * ==========================================================
 */

#ifdef __INTELLISENSE__
    #define XC8_INTERRUPT
#else
    #define XC8_INTERRUPT __interrupt()
#endif

/* ==========================================================
 * ADC0801
 * ========================================================== */

#define ADC0801_FULL_SCALE_MV    5000UL


/* ==========================================================
 * Temperature
 * ========================================================== */

#define LM35_MV_PER_C            10UL

/* 128 samples */
#define FILTER_SAMPLES           128U

/* One ADC sample every 10 ms */
#define TEMPERATURE_SAMPLE_MS    10UL


/* ==========================================================
 * Battery
 *
 * Recommended divider:
 *
 * Battery ---- 30k ----+---- 10k ---- GND
 *                      |
 *                     AN0
 *
 * Ratio = (30k + 10k) / 10k = 4
 * ========================================================== */

#define BATTERY_DIVIDER_NUM      4UL
#define BATTERY_DIVIDER_DEN      1UL

#define BATTERY_MIN_MV           10500UL
#define BATTERY_MAX_MV           12600UL


/* ==========================================================
 * Alarm thresholds
 * Temperature expressed in tenths of °C
 * ========================================================== */

#define ALARM_BLUE_ON_X10        350U
#define ALARM_BLUE_OFF_X10       355U

#define ALARM_RED_ON_X10         380U
#define ALARM_RED_OFF_X10        375U


/* ==========================================================
 * I2C addresses
 * 7-bit addresses
 * ========================================================== */

#define PCF8574_ADDRESS          0x20U
#define EEPROM_24LC256_ADDRESS   0x50U
#define MCP4725_ADDRESS          0x60U


/* ==========================================================
 * History
 * ========================================================== */

#define HISTORY_EEPROM_BASE      0x0010U
#define HISTORY_MAX_RECORDS      4094U

#define HISTORY_MAGIC            0xA5U
#define HISTORY_VERSION          0x01U


/* ==========================================================
 * DAC
 * ========================================================== */

#define DAC_LEVEL_COUNT          5U


/* ==========================================================
 * UI
 * ========================================================== */

#define LCD_UPDATE_MS            250UL
#define BATTERY_UPDATE_MS        500UL
#define HISTORY_LOG_MS           60000UL
#define BUTTON_UPDATE_MS         10UL

#endif