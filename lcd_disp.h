/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef LCD_DISP_H
#define	LCD_DISP_H

#include <xc.h>
#include <stdint.h>
#include "conbits.h"


#define LCD_DATA    LATB
#define LCD_READ    PORTB
#define LCD_IO_BF   TRISB
#define LCD_BF      PORTBbits.RB7
#define LCD_RS      LATDbits.LD5
#define LCD_RW      LATDbits.LD6
#define LCD_EN      LATDbits.LD7


#define LCD_INCREMENT           1
#define LCD_DECREMENT           0
#define LCD_SHIFT               1
#define LCD_NO_SHIFT            0
#define LCD_DISPLAY_ON          1
#define LCD_DISPLAY_OFF         0
#define LCD_CURSOR_ON           1
#define LCD_CURSOR_OFF          0
#define LCD_CURSOR_BLINK_ON     1
#define LCD_CURSOR_BLINK_OFF    0
#define LCD_MOVE_CURSOR         1
#define LCD_MOVE_SHIFT          0
#define LCD_MOVE_RIGHT          1
#define LCD_MOVE_LEFT           0
#define LCD_LENGTH_8BIT         1
#define LCD_LENGTH_4BIT         0
#define LCD_2_LINE              1
#define LCD_1_LINE              0
#define LCD_FONT_5_10           1
#define LCD_FONT_5_8            0


void LCD_clock(void);
void LCD_command(uint8_t command);
void LCD_clear(void);
void LCD_home(void);
void LCD_entry_mode_set(uint8_t inc_dec,uint8_t shift);

void LCD_display_controle(  uint8_t display_on_off,
                            uint8_t cursor_on_off,
                            uint8_t cursor_blink);

void LCD_cursor_controle(uint8_t cursor_shift,uint8_t right_left);

void LCD_func_set(uint8_t data_len,uint8_t lines,uint8_t font);

void LCD_cg_ram_addr(uint8_t addr);

uint8_t LCD_busy_flag(void);

void LCD_ram_write(uint8_t data);

uint8_t LCD_ram_read(void);

void LCD_init_8bits(void);

uint8_t LCD_set_pos(uint8_t x,uint8_t y);

void LCD_print(uint8_t x);

void LCD_print_string(const char *str);

#endif	/* XC_HEADER_TEMPLATE_H */

