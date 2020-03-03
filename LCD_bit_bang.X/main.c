#include <xc.h>
#include <pic18f4520.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "conbits.h"
#include "uart_layer.h"

#define LCD_DATA    LATB
#define LCD_RS      LATDbits.LD5
#define LCD_RW      LATDbits.LD6
#define LCD_EN      LATDbits.LD7

const uint8_t program_start[18]="\r\nProgram start\n\r";
uint8_t print_buffer[33] = {0}; // buffer to print stuff to serial

volatile uint8_t uart_char = 0;
volatile bool uart_rcv_data = false;

void LCD_clock(void){
    LCD_EN = 1;
    __delay_ms(1);
    LCD_EN = 0;
    __delay_ms(1);
}

void LCD_command(uint8_t command){
    LCD_RS = 0;
    LCD_RW = 0;
    
    LCD_DATA = command;
    LCD_clock();
    __delay_ms(10);  
}

void LCD_init_8bits(void){
    LCD_RS = 0;
    LCD_RW = 0;
    
    __delay_ms(50);
    LCD_command(0x30);
    LCD_command(0x30);
    LCD_command(0x30);
    
    //set function
    //8bit mode
    //2 line display
    //font to 5x8
    LCD_command(0x3C);
    
    //display off
    //cursor off
    //cursor no blink
    LCD_command(0x08);
    
    //clear the display and set to home
    LCD_command(0x01);
    
    //increment
    //no shift
    LCD_command(0x06);
    
    //display on
    //cursor on
    //cursor blink
    LCD_command(0x0F);   
}

uint8_t LCD_set_pos(uint8_t x,uint8_t y){
    
    if(y == 1){     
        x += 0x40;
    }
    
    LCD_command(0x80 | x);
    return x;
}

static uint8_t pos_track = 0;
static uint8_t line_track = 0;

void LCD_print(uint8_t x){
    
    if(pos_track > 15){
        if(line_track > 0){
            LCD_set_pos(0,0);
            line_track = 0;     
        }else{
            line_track++;
            LCD_set_pos(0,1);
        }
        
        pos_track = 0;
    }
    
    LCD_RS = 1;
    LCD_RW = 0;
    
    LCD_DATA = x;
    
    LCD_clock();
    
    pos_track++;
    
}

void LCD_print_string(const char *str){
    uint16_t i = 0;
    
    do{
        LCD_print(str[i]);
        i++;
        
    }while(str[i] != '\0');   
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
    
    __delay_ms(2000);
    uart_send_string((uint8_t *)program_start); // everything works in setup
    
    LCD_print_string("this is a string");
    

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
