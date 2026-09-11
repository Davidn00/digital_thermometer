#include <xc.h>
#include <stdint.h>

#include "config.h"
#include "i2c.h"


/*
 * ==========================================================
 * I2C SOFTWARE - PIC18F4550
 * ==========================================================
 *
 * RC3 -> SCL
 * RC4 -> SDA
 *
 * Debido a que el header del PIC18F4550 utilizado por XC8
 * no expone RC3/RC4 como miembros individuales de LATCbits
 * y TRISCbits, se utilizan los registros completos LATC/TRISC.
 *
 * Las líneas I2C utilizan lógica open-drain simulada:
 *
 * TRIS = 0 -> línea en LOW
 * TRIS = 1 -> línea liberada (HIGH mediante pull-up externo)
 * ==========================================================
 */


#define I2C_SCL_MASK       0x08U   /* RC3 */
#define I2C_SDA_MASK       0x10U   /* RC4 */


/* ==========================================================
 * Líneas I2C
 * ========================================================== */

static void I2C_SCL_Low(void)
{
    LATC &= (uint8_t)~I2C_SCL_MASK;
    TRISC &= (uint8_t)~I2C_SCL_MASK;
}


static void I2C_SCL_High(void)
{
    /*
     * Liberar SCL.
     *
     * El pull-up externo lleva la línea a HIGH.
     */
    TRISC |= I2C_SCL_MASK;
}


static void I2C_SDA_Low(void)
{
    LATC &= (uint8_t)~I2C_SDA_MASK;
    TRISC &= (uint8_t)~I2C_SDA_MASK;
}


static void I2C_SDA_High(void)
{
    /*
     * Liberar SDA.
     */
    TRISC |= I2C_SDA_MASK;
}


static uint8_t I2C_SDA_Read(void)
{
    return (PORTC & I2C_SDA_MASK) != 0U;
}


/* ==========================================================
 * Delay
 * ========================================================== */

static void I2C_Delay(void)
{
    __delay_us(5);
}


/* ==========================================================
 * Inicialización
 * ========================================================== */

void I2C_Init(void)
{
    /*
     * Liberar ambas líneas.
     */
    I2C_SCL_High();
    I2C_SDA_High();
}


/* ==========================================================
 * Escritura de bit
 * ========================================================== */

static void I2C_WriteBit(uint8_t bit)
{
    if (bit)
    {
        I2C_SDA_High();
    }
    else
    {
        I2C_SDA_Low();
    }


    I2C_SCL_High();

    I2C_Delay();

    I2C_SCL_Low();

    I2C_Delay();
}


/* ==========================================================
 * Lectura de bit
 * ========================================================== */

static uint8_t I2C_ReadBit(void)
{
    uint8_t bit;


    /*
     * Liberar SDA para que el esclavo pueda conducirla.
     */
    I2C_SDA_High();


    I2C_SCL_High();

    I2C_Delay();


    bit = I2C_SDA_Read();


    I2C_SCL_Low();

    I2C_Delay();


    return bit;
}


/* ==========================================================
 * START
 * ========================================================== */

void I2C_Start(void)
{
    I2C_SDA_High();

    I2C_SCL_High();

    I2C_Delay();


    I2C_SDA_Low();

    I2C_Delay();


    I2C_SCL_Low();
}


/* ==========================================================
 * STOP
 * ========================================================== */

void I2C_Stop(void)
{
    I2C_SDA_Low();

    I2C_SCL_High();

    I2C_Delay();

    I2C_SDA_High();

    I2C_Delay();
}


/* ==========================================================
 * Escritura de byte
 * ========================================================== */

uint8_t I2C_WriteByte(uint8_t data)
{
    uint8_t i;

    uint8_t ack;


    for (i = 0; i < 8; i++)
    {
        I2C_WriteBit(
            (data & 0x80U) != 0U
        );

        data <<= 1;
    }


    /*
     * ACK activo en LOW.
     */
    ack = (uint8_t)(!I2C_ReadBit());


    return ack;
}


/* ==========================================================
 * Lectura de byte
 * ========================================================== */

uint8_t I2C_ReadByte(uint8_t ack)
{
    uint8_t i;

    uint8_t data = 0;


    for (i = 0; i < 8; i++)
    {
        data <<= 1;

        data |= I2C_ReadBit();
    }


    /*
     * ACK = SDA LOW
     * NACK = SDA HIGH
     */
    I2C_WriteBit(
        ack ? 0U : 1U
    );


    return data;
}