/* 
 * File:        main.c
 * Author:      Florian Grethler & Mike Andreas Baier
 * Hardware:    PIC18F47Q10
 * Controller:  PIC18F47Q10 
 * Frequency:   1 MHz
 * Compiler:    XC8 (v2.45)
 * MPLAB X:     6.15
 * 
 * Copyright (c) Florian Grethler
 * 
 */

#include "Definitions.h"

/* This Code does a binary incrementation each ~1s.
 * This goes from 0b00000000 -> 0b11111111.
 * If the LEDs RB0 -> RB7 are next to each other.
 * Each LED that is ON represents a 1 in the binary number.
 */

unsigned int binary = 0b00000000;

// Function to increment a binary number by 1
unsigned int incrementBinary() {
    unsigned int one = 1; // Binary representation of 1
    
    while (binary & one) {
        binary = binary ^ one; // Flip the rightmost 1 to 0
        one = one << 1; // Shift the 1 to the left
    }
    
    binary = binary ^ one; // Flip the rightmost 0 to 1
    
    return binary;
}

void __interrupt() ISR_Timer1(void) {
    PIR4bits.TMR1IF = 0; // Clear Timer1 interrupt flag
    TMR1 = 0x8000;       // Reinitialize TMR1
    incrementBinary();
    LATA = (unsigned char)binary;
    Nop();
}

void initialize_Ports(void){
    TRISA = 0;
    LATA = 0;
}

void initialize_OSC(void){
    OSCCON1bits.NOSC = 0b110;     // Select FRCPLL as the system clock source
    OSCCON1bits.NDIV = 0b000;     // Divide FRC by 1 for the PLL input
}

void initialize_TMR1(void){
    T1CONbits.CKPS = 0b11;       // No 1:32 ?
    T1CONbits.NOT_SYNC = 1;      // Use internal clock
    TMR1CLKbits.CS = 0b00010;    // start counter
    TMR1 = 0x8000;               // Set TMR1
    PIR4bits.TMR1IF = 0;         // Clear Timer1 interrupt flag
    PIE4bits.TMR1IE = 1;         // Enable Timer1 interrupt
}

void initialize_Interrupts(void){
    INTCONbits.GIE = 1;           // enable 'global interrupt enable'
    INTCONbits.PEIE = 1;          // enable 'peripheral interrupt enable'
}

void TMR1_Start(void){
    T1CONbits.TMR1ON = 1;       // enable Timer1
}

void main (void) {
    initialize_Ports();
    initialize_OSC();
    initialize_TMR1();
    initialize_Interrupts();
    TMR1_Start();
    while(1){
        //
    }
}
