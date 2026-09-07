    /*
    * Digital Thermometer - Stage 1
    *
    * PIC18F4550 + ADC0801 + LM35
    * Compiler: MPLAB XC8
    *
    * Stage 1 objectives:
    *   1. Acquire the LM35 analog voltage with ADC0801.
    *   2. Detect end-of-conversion through ADC0801 INTR.
    *   3. Read the 8-bit ADC result.
    *   4. Convert the result to millivolts and approximate °C.
    *
    * Hardware mapping:
    *   ADC0801 DB0..DB7 -> PIC RD0..RD7
    *   ADC0801 CS       -> PIC RA3
    *   ADC0801 WR       -> PIC RA4
    *   ADC0801 RD       -> PIC RA5
    *   ADC0801 INTR     -> PIC RB2/INT2
    */

    #ifdef RCONbits
    #warning "RCONbits es un macro conocido por IntelliSense"
    #endif

    #include <xc.h>
    #include <stdint.h>

    #include "adc0801.h"
    #include "lm35.h"
    #include "filter.h"

    #define _XTAL_FREQ 20000000UL


    volatile uint8_t adc_sample = 0;
    volatile uint8_t adc_sample_ready = 0;


    void Interrupt_Init(void)
    {
        INTCON3bits.INT2IF = 0;
        INTCON3bits.INT2IE = 1;

        INTCON2bits.INTEDG2 = 0;

        INTCONbits.GIE = 1;
    }


    void __interrupt() ISR(void)
    {
        if (INTCON3bits.INT2IF)
        {
            INTCON3bits.INT2IF = 0;

            __delay_us(15);

            adc_sample = ADC0801_Read();
            adc_sample_ready = 1;

            ADC0801_StartConversion();
        }
    }


    void main(void)
    {
        uint8_t filtered_adc;
        uint16_t temperature;

        ADC0801_Init();

        Filter_Reset();

        Interrupt_Init();

        __delay_ms(2);

        ADC0801_StartConversion();

        while (1)
        {
            if (adc_sample_ready)
            {
                adc_sample_ready = 0;

                Filter_AddSample(adc_sample);
            }

            if (Filter_IsReady())
            {
                filtered_adc = Filter_GetAverage();

                temperature = LM35_ADC_To_Celsius(filtered_adc);

                /*
                * Aquí tendremos disponible
                * la temperatura filtrada.
                */
            }
        }
    }














// #include <xc.h>
// #include <stdint.h>

// #define _XTAL_FREQ 20000000UL

// #include "adc0801.h"
// #include "lm35.h"
// #include "config.h"


// /* PIC18F4550 - 20 MHz external crystal */
// #pragma config PLLDIV = 5
// #pragma config CPUDIV = OSC1_PLL2
// #pragma config USBDIV = 2

// #pragma config FOSC = HS
// #pragma config FCMEN = OFF
// #pragma config IESO = OFF

// #pragma config PWRT = OFF
// #pragma config BOR = OFF
// #pragma config VREGEN = OFF

// #pragma config WDT = OFF
// #pragma config WDTPS = 32768

// #pragma config MCLRE = ON
// #pragma config LPT1OSC = OFF
// #pragma config PBADEN = OFF
// #pragma config CCP2MX = ON

// #pragma config STVREN = ON
// #pragma config LVP = OFF
// #pragma config ICPRT = OFF
// #pragma config XINST = OFF
// #pragma config DEBUG = OFF


// #define FILTER_SAMPLES 128

// volatile uint16_t sample_sum = 0;
// volatile uint8_t sample_count = 0;

// volatile uint16_t filtered_sum = 0;
// volatile uint8_t filter_ready = 0;


// /* Variable declarations */

// volatile uint8_t adc_value = 0;
// volatile uint8_t adc_data_ready = 0;

// volatile uint8_t adc_sample_ready = 0;
// volatile uint8_t adc_sample = 0;



// static void MCU_Init(void)
// {
//     /*
//      * Configure all analog-capable pins as digital I/O.
//      *
//      * ADCON1 = 0x0F:
//      * AN0..AN12 disabled.
//      */
//     ADCON1 = 0x0F;

//     /*
//      * PORTA:
//      *
//      * RA3 -> ADC0801 CS
//      * RA4 -> ADC0801 WR
//      * RA5 -> ADC0801 RD
//      */
//     TRISAbits.TRISA3 = 0;
//     TRISAbits.TRISA4 = 0;
//     TRISAbits.TRISA5 = 0;

//     /*
//      * PORTB:
//      *
//      * RB2 -> ADC0801 INTR
//      */
//     TRISBbits.TRISB2 = 1;

//     /*
//      * PORTD:
//      *
//      * RD0..RD7 -> ADC0801 DB0..DB7
//      */
//     TRISD = 0xFF;

//     /*
//      * Initial control states.
//      */
//     LATAbits.LATA3 = 0;   // CS active
//     LATAbits.LATA4 = 1;   // WR inactive
//     LATAbits.LATA5 = 1;   // RD inactive

//     /*
//      * Clear PORTD latch.
//      */
//     LATD = 0x00;
// }

// static void Interrupt_Init(void)
// {
//     /*
//      * Use the PIC18F4550 INT2 external interrupt on RB2.
//      * ADC0801 INTR makes a high-to-low transition at the end
//      * of a conversion, so INT2 is configured for falling edge.
//      */
//     RCONbits.IPEN = 0;              // Disable interrupt priorities

//     INTCON3bits.INT2IF = 0;
//     INTCON3bits.INT2IE = 1;
//     INTCON2bits.INTEDG2 = 0;        // Falling edge

//     INTCONbits.GIE = 1;
// }

// void __interrupt() ISR(void)
// {
//     if (INTCON3bits.INT2IF)
//     {
//         INTCON3bits.INT2IF = 0;

//         __delay_us(15);

//         adc_sample = ADC0801_Read();
//         adc_sample_ready = 1;

//         ADC0801_StartConversion();
//     }
// }

// void main(void)
// {
//     uint16_t temperature_c;
//     uint16_t sensor_mv;

//     MCU_Init();
//     ADC0801_Init();
//     Interrupt_Init();

//     /*
//      * Required initial WR pulse after power-up.
//      * Start the first conversion.
//      */
//     __delay_ms(2);
//     ADC0801_StartConversion();

//     while (1)
//     {
//         if (adc_sample_ready)
//         {
//             uint8_t sample;

//             adc_sample_ready = 0;

//             sample = adc_sample;

//             Filter_AddSample(sample);
//         }
//             sensor_mv = LM35_ADC_To_mV(average);
//             temperature_c = LM35_ADC_To_Celsius(average);

//             /*
//              * Stage 1 intentionally has no LCD output yet.
//              * Put a breakpoint here in VS Code / debugger or
//              * inspect filtered_adc, sensor_mv and temperature_c
//              * in the simulator.
//              */
//             (void)sensor_mv;
//             (void)temperature_c;

//             /*
//              * Start the next conversion after the current sample
//              * has been processed.
//              */
//             __delay_ms(100);
//             ADC0801_StartConversion();
//         }
//     }
// }
