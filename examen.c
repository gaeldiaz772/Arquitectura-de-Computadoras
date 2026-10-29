#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#define N_ESTADOS 7 

const uint16_t modo[4] PROGMEM = {
    1000, 500, 2000, 1000
};

uint16_t velocidad(){
    uint8_t c = ~PINC;

    uint8_t num1 = (((c >> PINC1) & 1) << 0)
                 | (((c >> PINC2) & 1) << 1);
    return pgm_read_word(&modo[num1]);
}

const uint8_t tabla_salidas[6] PROGMEM = {
    ((1 << PORTB0) | (1 << PORTB3)), //S0
    ((1 << PORTB0) | (1 << PORTB3)), //S1
    ((1 << PORTB1) | (1 << PORTB2)), //S2
    ((1 << PORTB1) | (1 << PORTB2)), //S3
    ((1 << PORTB1) | (1 << PORTB3)), //S4
    ((1 << PORTB1) | (1 << PORTB3) | (1 << PORTB4)), //S5
};

const uint8_t tabla_transiciones[N_ESTADOS][3] PROGMEM = {
    { 0, 1, 0 }, 
    { 1, 2, 0 }, 
    { 2, 3, 0 }, //Normal hasta aqui
    { 3, 4, 0 }, 
    { 4, 0, 0 }, //Si es 0 va a el estado 0
    { 4, 5, 1 }, //Si es 1 va a el estado 5
    { 5, 0, 0 } //Termina el ciclo
};

//no existe el puerto 0xFF entonces no prende ninguno
const uint8_t leds_parpadean[6][2] PROGMEM = {
    { 0xFF, 0xFF },        
    { PORTB0, PORTB3 },    
    { 0xFF, 0xFF },        
    { PORTB2, PORTB1 },    
    { 0xFF, 0xFF },     
    { 0xFF, 0xFF }  
};

void delay_ms_variable(uint16_t ms) {
    while (ms--) {
        _delay_ms(1);
    }
}

void parpadeo(uint16_t duracion_total, uint16_t vel_rapida, uint8_t pin_parpadea, uint8_t pin_fijo) {
    uint16_t tiempo_transcurrido = 0;
    uint16_t contador_rapido = 0;
    PORTB |= (1 << pin_parpadea) | (1 << pin_fijo);
    while (tiempo_transcurrido < duracion_total) {
        _delay_ms(10); 
        tiempo_transcurrido += 10;
        contador_rapido += 10;
        if (contador_rapido >= vel_rapida) {
            PORTB ^= (1 << pin_parpadea);
            contador_rapido = 0;       
        }
    }
    PORTB &= ~((1 << pin_parpadea) | (1 << pin_fijo));
}

int main(){
    DDRB |= 0x1F;
    PORTB = 0x00;
    DDRC &= ~((1 << DDC0) | (1 << DDC1) | (1 << DDC2));
    PORTC |= (1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2);
    uint8_t estado_actual = 0;
    while(1){
        uint16_t vel = velocidad();
        
        uint8_t led_parpadeo = pgm_read_byte(&leds_parpadean[estado_actual][0]);
        uint8_t led_fijo = pgm_read_byte(&leds_parpadean[estado_actual][1]);
        if(led_parpadeo != 0xFF){
            parpadeo(vel, 150, led_parpadeo, led_fijo);
        } else {
            PORTB = pgm_read_byte(&tabla_salidas[estado_actual]);
            delay_ms_variable(vel);
        }

        uint8_t c = ~PINC;
        uint8_t p = (c >> PINC0) & 1;
        for(uint8_t i = 0; i < N_ESTADOS; i++){
            uint8_t estado_act = pgm_read_byte(&tabla_transiciones[i][0]);
            uint8_t estado_sig = pgm_read_byte(&tabla_transiciones[i][1]);
            uint8_t boton_peaton = pgm_read_byte(&tabla_transiciones[i][2]);
            if(estado_act == estado_actual){
                if(estado_actual == 4){
                    if(boton_peaton == p){
                        estado_actual = estado_sig;
                        break;
                    }
                } else {
                    estado_actual = estado_sig;
                    break;
                }
            }
        }
    }
    return 0;
}