#include <xc.h>
#include "uart_layer.h"

void uart_init(uint16_t gen_reg, unsigned sync,unsigned brgh, unsigned brg16){
    
    TRISCbits.RC7=1;
    TRISCbits.RC6=1;
    
    SPBRGH = (gen_reg & 0xFF00) >> 8;
    SPBRG = gen_reg & 0x00FF;
    
    RCSTAbits.CREN = 1;
    RCSTAbits.SPEN = 1;
    BAUDCONbits.BRG16 = brg16;
    
    TXSTAbits.SYNC = sync;
    TXSTAbits.BRGH = brgh;
    TXSTAbits.TXEN = 1;
    
    IPR1bits.RCIP=1;
    PIE1bits.RCIE=1;  
}

void uart_send(uint8_t c){
    while(TXSTAbits.TRMT==0){
        Nop();
    }
    TXREG=c;
}

void uart_receiver(volatile uint8_t *c,volatile bool *rx_flag){
    
    if(RCSTAbits.FERR){
        uint8_t er = RCREG;
    }else if(RCSTAbits.OERR){
        RCSTAbits.CREN=0;
        RCSTAbits.CREN=1;
    }else{
        *c = RCREG;
        *rx_flag = true;
    }
    
}

void uart_send_array(uint8_t *c,uint16_t len){
    for(uint8_t i = 0; i < len;i++){
        uart_send(c[i]);
    }
}
void uart_send_string(uint8_t *c){
    uint8_t i=0;
    do{
        uart_send(c[i]);
        i++;
    }while(c[i] != '\0');
}









































































