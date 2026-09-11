#include <xc.h>
#include <stdint.h>


#include "config.h"

#include "adc0801.h"
#include "lm35.h"
#include "filter.h"

#include "timer.h"
#include "adc_internal.h"

#include "i2c.h"
#include "pcf8574.h"
#include "lcd.h"

#include "eeprom.h"
#include "history.h"

#include "mcp4725.h"

#include "buttons.h"
#include "alarm.h"

#include "stats.h"

#include "seg7.h"


/* ==========================================================
 * PIC18F4550 Configuration
 * ========================================================== */

#pragma config PLLDIV = 5
#pragma config CPUDIV = OSC1_PLL2
#pragma config USBDIV = 2

#pragma config FOSC = HS
#pragma config FCMEN = OFF
#pragma config IESO = OFF

#pragma config PWRT = OFF
#pragma config BOR = OFF
#pragma config VREGEN = OFF

#pragma config WDT = OFF
#pragma config WDTPS = 32768

#pragma config MCLRE = ON
#pragma config LPT1OSC = OFF
#pragma config PBADEN = OFF
#pragma config CCP2MX = ON

#pragma config STVREN = ON
#pragma config LVP = OFF
#pragma config ICPRT = OFF
#pragma config XINST = OFF
#pragma config DEBUG = OFF


/* ==========================================================
 * ADC0801 shared data
 * ========================================================== */

volatile uint8_t adc_sample = 0;

volatile uint8_t adc_sample_ready = 0;


/* ==========================================================
 * Application state
 * ========================================================== */

typedef enum
{
    STATE_MAIN = 0,
    STATE_STATS,
    STATE_HISTORY,
    STATE_DAC

} AppState;


static AppState app_state = STATE_MAIN;


/* ==========================================================
 * Application data
 * ========================================================== */

static uint8_t temperature_valid = 0;

static uint16_t temperature10 = 0;

static uint8_t battery_percent = 0;

static AlarmState alarm_state = ALARM_NORMAL;

static uint8_t dac_level = 0;

static uint8_t ui_dirty = 1;


/* ==========================================================
 * Interrupt initialization
 * ========================================================== */

static void Interrupt_Init(void)
{
    /*
     * No interrupt priorities.
     */
    RCONbits.IPEN = 0;


    /*
     * ADC0801 INTR -> INT2
     */
    INTCON3bits.INT2IF = 0;

    INTCON3bits.INT2IE = 1;

    INTCON2bits.INTEDG2 = 0;


    /*
     * Timer0
     */
    INTCONbits.TMR0IF = 0;


    /*
     * Global interrupt enable.
     */
    INTCONbits.GIE = 1;
}


/* ==========================================================
 * Interrupt Service Routine
 * ========================================================== */

void __interrupt() ISR(void)
{
    /*
     * Timer0 - 1 ms
     */
    if (INTCONbits.TMR0IF)
    {
        Timer_1ms_ISR();
    }


    /*
     * ADC0801 conversion completed.
     */
    if (INTCON3bits.INT2IF)
    {
        INTCON3bits.INT2IF = 0;


        /*
         * ADC0801 requires a small delay before RD.
         */
        __delay_us(15);


        adc_sample =
            ADC0801_Read();


        adc_sample_ready = 1;
    }
}


/* ==========================================================
 * LCD helpers
 * ========================================================== */

static void LCD_ClearLine(uint8_t row)
{
    uint8_t i;


    LCD_SetCursor(row, 0);


    for (i = 0; i < 20; i++)
    {
        LCD_WriteChar(' ');
    }


    LCD_SetCursor(row, 0);
}


static void LCD_ShowMain(void)
{
    LCD_ClearLine(0);

    LCD_Print("DIGITAL THERMOMETER");


    LCD_ClearLine(1);

    if (temperature_valid)
    {
        LCD_Print("T:");

        LCD_PrintTemp10(temperature10);

        LCD_Print(" BAT:");

        LCD_PrintBattery(battery_percent);
    }
    else
    {
        LCD_Print("Waiting temperature");
    }


    LCD_ClearLine(2);

    if (temperature_valid)
    {
        LCD_Print("ALARM: ");

        switch (alarm_state)
        {
            case ALARM_LOW:
                LCD_Print("LOW");
                break;

            case ALARM_HIGH:
                LCD_Print("HIGH");
                break;

            default:
                LCD_Print("NORMAL");
                break;
        }
    }


    LCD_ClearLine(3);

    LCD_Print("MODE: MAIN");
}


static void LCD_ShowStats(void)
{
    uint16_t average;

    uint16_t maximum;

    uint16_t minimum;


    LCD_ClearLine(0);

    LCD_Print("TEMPERATURE STATS");


    LCD_ClearLine(1);

    if (Stats_GetCount() == 0)
    {
        LCD_Print("No data");
    }
    else
    {
        average = Stats_GetAverage();

        LCD_Print("AVG: ");

        LCD_PrintTemp10(average);
    }


    LCD_ClearLine(2);

    if (Stats_GetCount() != 0)
    {
        maximum = Stats_GetMaximum();

        LCD_Print("MAX: ");

        LCD_PrintTemp10(maximum);
    }


    LCD_ClearLine(3);

    if (Stats_GetCount() != 0)
    {
        minimum = Stats_GetMinimum();

        LCD_Print("MIN: ");

        LCD_PrintTemp10(minimum);
    }
}


static void LCD_ShowHistory(void)
{
    HistoryRecord record;


    LCD_ClearLine(0);

    LCD_Print("EEPROM HISTORY");


    LCD_ClearLine(1);

    LCD_Print("RECORDS: ");

    LCD_PrintUInt(
        History_GetCount()
    );


    LCD_ClearLine(2);


    if (History_GetLatest(&record))
    {
        LCD_Print("LAST: ");

        LCD_PrintTemp10(
            record.temperature10
        );
    }
    else
    {
        LCD_Print("No records");
    }


    LCD_ClearLine(3);

    if (History_GetLatest(&record))
    {
        LCD_Print("BAT:");

        LCD_PrintBattery(
            record.battery
        );

        LCD_Print(" DAC:");

        LCD_PrintUInt(
            record.dac_level
        );
    }
}


static void LCD_ShowDAC(void)
{
    uint16_t code;


    LCD_ClearLine(0);

    LCD_Print("MCP4725 DAC");


    LCD_ClearLine(1);

    LCD_Print("LEVEL: ");

    LCD_PrintUInt(dac_level + 1);

    LCD_Print("/5");


    LCD_ClearLine(2);

    LCD_Print("VOUT: ");


    code =
        MCP4725_GetLevelCode(
            dac_level
        );


    LCD_PrintDACVoltage(code);


    LCD_ClearLine(3);

    LCD_Print("SELECT = NEXT");
}


/* ==========================================================
 * UI update
 * ========================================================== */

static void UI_Update(void)
{
    switch (app_state)
    {
        case STATE_MAIN:
            LCD_ShowMain();
            break;

        case STATE_STATS:
            LCD_ShowStats();
            break;

        case STATE_HISTORY:
            LCD_ShowHistory();
            break;

        case STATE_DAC:
            LCD_ShowDAC();
            break;

        default:
            app_state = STATE_MAIN;
            break;
    }


    ui_dirty = 0;
}


/* ==========================================================
 * Button processing
 * ========================================================== */

static void ProcessButtons(uint8_t events)
{
    if (events & BUTTON_MODE)
    {
        app_state++;


        if (app_state > STATE_DAC)
        {
            app_state = STATE_MAIN;
        }


        ui_dirty = 1;
    }


    if (events & BUTTON_SELECT)
    {
        if (app_state == STATE_DAC)
        {
            dac_level++;


            if (dac_level >= DAC_LEVEL_COUNT)
            {
                dac_level = 0;
            }


            MCP4725_SetLevel(
                dac_level
            );


            ui_dirty = 1;
        }
    }
}


/* ==========================================================
 * Main
 * ========================================================== */

void main(void)
{
    uint32_t now;

    uint32_t last_sample = 0;

    uint32_t last_battery = 0;

    uint32_t last_button = 0;

    uint32_t last_history = 0;

    uint32_t last_lcd = 0;


    uint8_t filtered_adc;


    /* ------------------------------------------------------
     * Hardware initialization
     * ------------------------------------------------------ */

    ADC0801_Init();

    ADC_Internal_Init();

    Filter_Reset();


    I2C_Init();

    LCD_Init();

    History_Init();

    MCP4725_Init();


    Buttons_Init();

    Alarm_Init();

    Stats_Reset();


    Seg7_Init();


    Timer_Init();


    Interrupt_Init();


    /*
     * Initial DAC level.
     */
    dac_level = 0;

    MCP4725_SetLevel(0);


    /*
     * Allow hardware to stabilize.
     */
    __delay_ms(100);


    /*
     * Start first ADC0801 conversion.
     */
    ADC0801_StartConversion();


    /* ------------------------------------------------------
     * Main loop
     * ------------------------------------------------------ */

    while (1)
    {
        now = Timer_Millis();


        /* ==================================================
         * ADC sample processing
         * ================================================== */

        if (adc_sample_ready)
        {
            filtered_adc =
                adc_sample;


            adc_sample_ready = 0;


            Filter_AddSample(
                filtered_adc
            );
        }


        /* ==================================================
         * Start next ADC0801 conversion
         * every 10 ms
         * ================================================== */

        if (Timer_Elapsed(
                now,
                &last_sample,
                TEMPERATURE_SAMPLE_MS))
        {
            ADC0801_StartConversion();
        }


        /* ==================================================
         * Filter complete
         * ================================================== */

        if (Filter_IsReady())
        {
            filtered_adc =
                Filter_GetAverage();


            temperature10 =
                LM35_ADC_To_Temperature10(
                    filtered_adc
                );


            temperature_valid = 1;


            /*
             * Statistics
             */
            Stats_Add(
                temperature10
            );


            /*
             * Alarm
             */
            alarm_state =
                Alarm_Update(
                    temperature10
                );


            /*
             * Seven segment display
             */
            Seg7_SetTemperature10(
                temperature10
            );


            ui_dirty = 1;
        }


        /* ==================================================
         * Battery
         * ================================================== */

        if (Timer_Elapsed(
                now,
                &last_battery,
                BATTERY_UPDATE_MS))
        {
            battery_percent =
                Battery_ReadPercent();


            ui_dirty = 1;
        }


        /* ==================================================
         * Buttons
         * ================================================== */

        if (Timer_Elapsed(
                now,
                &last_button,
                BUTTON_UPDATE_MS))
        {
            uint8_t events;


            events =
                Buttons_Update();


            if (events)
            {
                ProcessButtons(events);
            }
        }


        /* ==================================================
         * EEPROM historical record
         * ================================================== */

        if (
            temperature_valid &&
            Timer_Elapsed(
                now,
                &last_history,
                HISTORY_LOG_MS))
        {
            History_Log(
                temperature10,
                battery_percent,
                (uint8_t)alarm_state,
                dac_level
            );


            ui_dirty = 1;
        }


        /* ==================================================
         * LCD
         * ================================================== */

        if (
            ui_dirty ||
            Timer_Elapsed(
                now,
                &last_lcd,
                LCD_UPDATE_MS))
        {
            UI_Update();
        }
    }
}