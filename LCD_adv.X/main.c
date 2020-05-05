#include <xc.h>
#include <pic18f4520.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "conbits.h"
#include "uart_layer.h"
#include "lcd_disp.h"


const char bell[8] = {
  0b00000100,
  0b00001110,
  0b00001110,
  0b00011111,
  0b00011111,
  0b00011111,
  0b00000100,
  0b00000000,   
};


const char window[8] = {
  0b00011111,
  0b00000100,
  0b00011111,
  0b00000100,
  0b00011111,
  0b00000100,
  0b00011111,
  0b00000000,   
};

const uint8_t program_start[18]="\r\nProgram start\n\r";
uint8_t print_buffer[33] = {0}; // buffer to print stuff to serial

volatile uint8_t uart_char = 0;
volatile bool uart_rcv_data = false;


void LCD_customs_char_write(uint8_t pos,char * x){
    pos = (pos & 0x07) << 3;
    
    LCD_cg_ram_addr(pos);
    
    for(uint8_t i = 0; i < 8; i++){
        LCD_ram_write(x[i]);
    }
}

void main(void){

    OSCCONbits.IDLEN = 0;
    OSCCONbits.IRCF = 0x07;
    OSCCONbits.SCS = 0x03;
    while(OSCCONbits.IOFS!=1); // 8Mhz
    
    TRISDbits.RD5 = 0;
    TRISDbits.RD6 = 0;
    TRISDbits.RD7 = 0;
    
    
    TRISB = 0x00;
    
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_EN = 0;
    
    LATB = 0x00;

    uart_init(51,0,1,0);//baud 9600
    
    RCONbits.IPEN = 1;
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;// base interrupt setup
    
    LCD_init_8bits();
    
    LCD_customs_char_write(0,bell);
    LCD_customs_char_write(2,window);
    
    
    LCD_set_pos(0,0);
    
    __delay_ms(2000);
    uart_send_string((uint8_t *)program_start); // everything works in setup
    
    
    LCD_print_string("Subscribe");
    LCD_set_pos(0,1);
    LCD_print_string("Ring the bell(");
    LCD_print(0);
    LCD_print(2);
    
    
    for(;;){ // while(1)
        
        if(uart_rcv_data){
            uart_send(uart_char);
            LCD_print(uart_char);
            uart_rcv_data = false;
        }
        
    } 
}



void __interrupt() high_isr(void){
    INTCONbits.GIEH = 0;


    if(PIR1bits.RCIF){
        uart_receiver(&uart_char,&uart_rcv_data);
        PIR1bits.RCIF=0;
    }

    INTCONbits.GIEH = 1;
}

void __interrupt(low_priority) low_isr(void){
    INTCONbits.GIEH = 0;

    if(0){

    }

    INTCONbits.GIEH = 1;
}