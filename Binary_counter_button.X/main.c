/* 
 * File:        Main.c
 * Author:      Florian Grethler & Mike Andreas Baier
 * Hardware:    PIC18F47Q10
 * Controller:  PIC18F47Q10 
 * Compiler:    XC8 (v2.45)
 * MPLAB X:     6.15
 * 
 * Copyright (c) Florian Grethler
 * 
 */

#include "Definitions.h"

typedef enum{
    BT_NOCHANGE,
    BT_SHORT_PRESS,
    BT_LONG_PRESS
} button_t;


static void PORT_Initialize(void)
{
    TRISA = 0;
    LATA = 0;
    
    TRISEbits.TRISE2 = 1;
    ANSELEbits.ANSELE2 = 0;
    WPUEbits.WPUE2 = 1;
}   

static void CLK_Initialize(void)
{
    /* Configure NOSC LFINTOSC; NDIV 1 FOSC = 31kHz */
    OSCCON1bits.NOSC = 5;
    OSCCON1bits.NDIV = 0;
}

static button_t ButtonCheck(void)
{
    button_t result = BT_NOCHANGE;
    uint8_t counter = 0;
    static bool old_button_state = true;
    bool button_state = PORTEbits.RE2;
    if( (button_state == false) && (old_button_state != false) )
    {
        __delay_ms(10);
        while( (PORTEbits.RE2 == false) && \
                (counter < (1500/10)) )
        {
            counter++;
            __delay_ms(10);
        }
        if(counter)
        {
            result = BT_SHORT_PRESS;
            if(counter >= (1500/10))
                result = BT_LONG_PRESS;
        }
    }
    old_button_state = button_state;
    return result;
}

unsigned int binary = 0b00000000;

unsigned int incrementBinary() {
    unsigned int one = 1;
    
    while (binary & one) {
        binary = binary ^ one;
        one = one << 1;
    }
    
    binary = binary ^ one;
    
    return binary;
}

void main(void)
{
    PORT_Initialize();
    CLK_Initialize();
    
    while (1)
    {
        button_t button_status = ButtonCheck();
        if(button_status == BT_SHORT_PRESS)
        {
            incrementBinary(); 
        }
        else if(button_status == BT_LONG_PRESS)
        {
            binary = 0b00000000;
        }
        LATA = (unsigned char)binary;
    }
}