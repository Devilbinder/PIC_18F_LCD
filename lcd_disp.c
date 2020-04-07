/*
 * File:   lcd_disp.c
 * Author: Binder
 *
 * Created on 08 March 2020, 8:28 PM
 */


#include "lcd_disp.h"



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

void LCD_clear(void){
    LCD_command(0x01);
}

void LCD_home(void){
  
    LCD_command(0x02);
}


void LCD_entry_mode_set(uint8_t inc_dec,uint8_t shift){
    inc_dec = (inc_dec << 1) & 0x02;
    shift = shift & 0x01;
    LCD_command(0x04 | inc_dec | shift);
}


void LCD_display_controle(  uint8_t display_on_off,
                            uint8_t cursor_on_off,
                            uint8_t cursor_blink){
    
    display_on_off = (display_on_off << 2) & 0x04;
    cursor_on_off = (cursor_on_off << 1) & 0x02;
    cursor_blink = cursor_blink & 0x01;
    LCD_command(0x08 | display_on_off | cursor_on_off | cursor_blink);
}

void LCD_cursor_controle(uint8_t cursor_shift,uint8_t right_left){
    cursor_shift = (cursor_shift << 3) & 0x08;
    cursor_shift = (cursor_shift << 2) & 0x04;
    LCD_command(0x10 | cursor_shift | cursor_shift );
}

void LCD_func_set(uint8_t data_len,uint8_t lines,uint8_t font){
    data_len = (data_len << 4) & 0x10;
    lines = (lines << 3) & 0x08;
    font = (font << 2) & 0x04;
    LCD_command(0x20 | data_len | lines | font);
}

void LCD_cg_ram_addr(uint8_t addr){
    LCD_command(0x40 | addr);
}

uint8_t LCD_busy_flag(void){
    uint8_t addr = 0;
    
    LCD_RS = 0;
    LCD_RW = 1;
    
    LCD_IO_BF = 0xFF;
    
    while(LCD_BF != 0){
        NOP();
    };
    addr = LCD_READ & 0x7F;
    
    LCD_IO_BF = 0x00;
    
    LCD_RS = 0;
    LCD_RW = 0;
    
    return addr;
    
}


void LCD_ram_write(uint8_t data){
    LCD_RS = 1;
    LCD_RW = 0;
    
    LCD_DATA = data;
    
    LCD_clock();
    __delay_ms(10);
    
}


uint8_t LCD_ram_read(void){
    
    uint8_t data = 0;
    LCD_RS = 1;
    LCD_RW = 1;
    
    LCD_IO_BF = 0xFF;
    
    LCD_clock();
    
    data = LCD_READ;
    
    LCD_IO_BF = 0x00;
    
    __delay_ms(10);
    LCD_RS = 0;
    LCD_RW = 0;
    
    return data;  
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
    LCD_func_set(LCD_LENGTH_8BIT,LCD_2_LINE,LCD_FONT_5_8);
    
    //display off
    //cursor off
    //cursor no blink
    LCD_display_controle(LCD_DISPLAY_OFF,LCD_CURSOR_OFF,LCD_CURSOR_BLINK_OFF);
    
    //clear the display and set to home
    LCD_clear();
    
    //increment
    //no shift
    LCD_entry_mode_set(LCD_INCREMENT,LCD_NO_SHIFT);
    
    //display on
    //cursor on
    //cursor blink
    LCD_display_controle(LCD_DISPLAY_ON,LCD_CURSOR_ON,LCD_CURSOR_BLINK_OFF);   
}

static uint8_t pos_track = 0;
static uint8_t line_track = 0;

uint8_t LCD_set_pos(uint8_t x,uint8_t y){
    pos_track = x;
    if(y == 1){     
        x |= 0x40;
        line_track = 1;
    }
    
    LCD_command(0x80 | x);
    return x;
}

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



