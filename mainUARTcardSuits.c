//===========================================================

// Test program UART0 TX 7/N/1 @ 9600 bit/s

// This program prints card suits in the center of the pendulum using two interrupts, GPIO interrupt and UART interrupt
// by presseing assigned key for each figure on the kyeboard 
// d : clover; a: diamond; s: Heart; w: spade

//==========================================================

#include "inc/tm4c1294ncpdt.h"
#include "int_handler.h"
#include <stdint.h>
#include <stdio.h>
#define BUFFERLENGTH 1

char buffer[BUFFERLENGTH]; // Rx data buffer
int edgeInterrupt = 0;  // global variable to know when pendulum has reached edge


//Display clover
void displayInPendulumD(void) {

    int i;
    for(i= 0; i<59 ;i++ ){
        TIMER0_CTL_R |= 0x0001;
        while((TIMER0_RIS_R & 0x01)==0);
        TIMER0_ICR_R |= 0x01;
        GPIO_PORTM_DATA_R = 0x00;
        if(i==27)
            GPIO_PORTM_DATA_R = 0x38;
        else if(i==28)
            GPIO_PORTM_DATA_R = 0x38;
        else if(i==29)
            GPIO_PORTM_DATA_R = 0xD2;
        else if(i==30)
            GPIO_PORTM_DATA_R = 0xFE;
        else if(i==31)
            GPIO_PORTM_DATA_R = 0xD2;
        else if(i==32)
            GPIO_PORTM_DATA_R = 0x38;
        else (i==33)
            GPIO_PORTM_DATA_R = 0x38;
    }
}



//Display diamond
void displayInPendulumA(void) {//TODO: change the if statements to else if
    int i;
    for(i= 0; i<59 ;i++ ){
        TIMER0_CTL_R |= 0x0001;
        while((TIMER0_RIS_R & 0x01)==0);
        TIMER0_ICR_R |= 0x01;
        GPIO_PORTM_DATA_R = 0x00;
        if(i==27)
            GPIO_PORTM_DATA_R = 0x10;
        if(i==28)
            GPIO_PORTM_DATA_R = 0x28;
        if(i==29)
            GPIO_PORTM_DATA_R = 0x44;
        if(i==30)
            GPIO_PORTM_DATA_R = 0x82;
        if(i==31)
            GPIO_PORTM_DATA_R = 0x44;
        if(i==32)
            GPIO_PORTM_DATA_R = 0x28;
        if(i==33)
            GPIO_PORTM_DATA_R = 0x10;
    }
}


 //Display spade
 void displayInPendulumW(void) {//TODO: change the if statements to else if 
     int i;
     for(i= 0; i<59 ;i++ ){
         TIMER0_CTL_R |= 0x0001;
         while((TIMER0_RIS_R & 0x01)==0);
         TIMER0_ICR_R |= 0x01;
         GPIO_PORTM_DATA_R = 0x00;
         if(i==27)
             GPIO_PORTM_DATA_R = 0x18;
         if(i==28)
             GPIO_PORTM_DATA_R = 0x28;
         if(i==29)
             GPIO_PORTM_DATA_R = 0x52;
         if(i==30)
             GPIO_PORTM_DATA_R = 0x9E;
         if(i==31)
             GPIO_PORTM_DATA_R = 0x52;
         if(i==32)
             GPIO_PORTM_DATA_R = 0x28;
         if(i==33)
             GPIO_PORTM_DATA_R = 0x18;
     }
}

 
//Display big heart
void displayInPendulumS(void) {//TODO: change the if statements to else if 
    int i;
    for(i= 0; i<59 ;i++ ){
        TIMER0_CTL_R |= 0x0001;
        while((TIMER0_RIS_R & 0x01)==0);
        TIMER0_ICR_R |= 0x01;
        GPIO_PORTM_DATA_R = 0x00;
        if(i==27)
            GPIO_PORTM_DATA_R = 0x70;
        if(i==28)
            GPIO_PORTM_DATA_R = 0x88;
        if(i==29)
            GPIO_PORTM_DATA_R = 0x44;
        if(i==30)
            GPIO_PORTM_DATA_R = 0x22;
        if(i==31)
            GPIO_PORTM_DATA_R = 0x44;
        if(i==32)
            GPIO_PORTM_DATA_R = 0x88;
        if(i==33)
            GPIO_PORTM_DATA_R = 0x70;
    }
}


//GPIO handler

void ISR_PortLHandler(void)// IRQ number 53, vector number 69
{
    GPIO_PORTL_ICR_R |= 0x0001;  // clear interrupt
    TIMER0_CTL_R &= ~0x0001;  // disable timer
    TIMER0_CTL_R |= 0x0001; // enable Timer 0A
    // turn on flag
    edgeInterrupt = 1;
}



//GPIO pin configuration

void GPIO_port_config(void)
{
    //configure port M
    SYSCTL_RCGCGPIO_R |= (1<<11); // =>0000 1000 0000 0000 Enable and provide a clock to GPIO Port M in Run mode
    while(!(SYSCTL_PRGPIO_R & (1<<11)));// Wait until GPIO Port M Peripheral Ready
    GPIO_PORTM_DEN_R = 0xFF; // => Enable GPIO Port M pins 0 to  7
    GPIO_PORTM_DIR_R = 0xFF; // => Set GPIO Port M pins 0 to 7 to Output

    //configure port L
    SYSCTL_RCGCGPIO_R |= (1<<10); // =>0000 0100 0000 0000 Enable and provide a clock to GPIO Port L in Run mode
    while(!(SYSCTL_PRGPIO_R & (1<<10)));// Wait until GPIO Port L Peripheral Ready
    GPIO_PORTL_DEN_R |= 0x01; // enable pin L(0)
    GPIO_PORTL_DIR_R &= ~0x01 ; // PortL(0) as input
}

//GPIO interrupt configuration
void GPIO_portL_interrupt_config(void)
{
    GPIO_PORTL_IS_R &= ~0x01; // sensitive to edge-sensitive
    GPIO_PORTL_IBE_R |= 0x01; // sensitive to both edges
    GPIO_PORTL_ICR_R |= 0x01; // clear interrupt in PORTL(0)
    GPIO_PORTL_IM_R |= 0x01; // GPIO interrupt Mask register-> 1: establish an interrupt for portL

    //enable PORTL interrupt (Int#53/Vec#69) in NVIC
    NVIC_EN1_R |= (1<<21); //53 - 32
}

//Timer configuration
void timer0A_config(void)
{
    SYSCTL_RCGCTIMER_R |= 0x01;
    while(!(SYSCTL_PRTIMER_R & 0x01));
    TIMER0_CTL_R &= ~0x0001; // disable Timer 0A
    TIMER0_CFG_R = 0x4; // 16 bit selected
    TIMER0_TAMR_R = 0x01; //One-mode timer : Capture mode enable
    TIMER0_TAILR_R = 16000-1; //(unsigned long) load value 1ms
}



void configPortA(void)
{
// initialize Port A
    SYSCTL_RCGCGPIO_R |= (1 << 0); // switch on clock for Port A
    while ((SYSCTL_PRGPIO_R & (1 << 0)) == 0)
        ; // wait for stable clock
    GPIO_PORTA_AHB_DEN_R |= 0x01; // digital I/O enable for pin PA(0)
    GPIO_PORTA_AHB_AFSEL_R |= 0x01; // PA(0) set to alternate func
    GPIO_PORTA_AHB_PCTL_R |= 0x00000001; // alternate func is U0Rx
}

void configUart0(void)
{
// initialize UART0
    SYSCTL_RCGCUART_R |= (1 << 0); // switch on clock for UART0
    while ((SYSCTL_PRUART_R & (1 << 0)) == 0)
        ; // wait for stable clock
    UART0_CTL_R &= ~(1 << 0); // disable UART0 for config

    // initialize bitrate of 9600 bit per second
    UART0_IBRD_R = 104; // set DIVINT of BRD floor(16MHz/16/9600Hz)
    UART0_FBRD_R = 11; // set DIVFRAC of BRD
    UART0_LCRH_R = 0x00000040; // serial format 7N1
    UART0_CTL_R |= 0x0201; // re-enable UART0
}

//UART0 interrupt config
void U0Rx_interrupt_config(void)
{
    UART0_ICR_R = 0xE7FF;               //clear all interrupts
    UART0_IM_R = 1<<4;                  //activate uart4 interrupts
    NVIC_EN0_R |= 1<<5;           //enable uart0 irq in nvic(irq 5)
}

//UART handler
void ISR_HandlerPK_UART0(void)
{
    int i = 0;
    while(i < BUFFERLENGTH) { // loop while buffer not full

    while(UART0_FR_R & (1<<4)) // wait till Rx FIFO is not empty any more
    ;
    buffer[i]=UART0_DR_R; // read byte from UART2 data register
    if (buffer[i] == 0x04) {// break loop if "EOT" (End of Transm.) received
        buffer[i] = 0x00; // console input for EOT = Strg+D
    break; // replace last element by \0 to terminate string
    }
    i++; // increment buffer index
    }

    if (i == BUFFERLENGTH){
        i = 0;
    }
}


void main(void)
{
// config Port A
    configPortA();

// config UART0 Rx+Tx
    configUart0();

    GPIO_port_config();
    timer0A_config();
    GPIO_portL_interrupt_config();
    U0Rx_interrupt_config();

    while (1)
    {
        // print card suits
                // diamond
                if (edgeInterrupt == 1){
                    if (buffer[0] == 0x61)
                        displayInPendulumA();
                    edgeInterrupt = 0;
                }

                // clover
                if (edgeInterrupt == 1){
                    if (buffer[0] == 0x64) // if d has been pressed
                        displayInPendulumD();
                    edgeInterrupt = 0;
                }

                // Heart
                if (edgeInterrupt == 1){
                    if (buffer[0] == 0x73)
                         displayInPendulumS();
                    edgeInterrupt = 0;
                }


                // spade
                if (edgeInterrupt == 1){
                    if (buffer[0] == 0x77)
                        displayInPendulumW();
                    edgeInterrupt = 0;
                }

    }

}
