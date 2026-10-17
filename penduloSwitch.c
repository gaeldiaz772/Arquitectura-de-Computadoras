#define F_CPU 16000000UL
#include <avr/io.h>     // Librería para registros de I/O
#include <avr/pgmspace.h>
#include <util/delay.h> // Librería de AVR para retardos temporales

const uint16_t velocid[8] PROGMEM = {
    20, 100, 300, 600, 900, 1200, 1400, 1600
};

void delay_ms_var(uint16_t ms) {
    while (ms--) {
        _delay_ms(1);
    }
}

uint16_t velocidad(void){
    uint16_t d = ~PIND;
    uint16_t c = ~PINC;

    // Nibble 1: PD7 (bit 0), PD6 (bit 1), PC0 (bit 2), PC1 (bit 3)
    uint8_t num1 = (((d >> PIND7) & 1) << 0)
                 | (((d >> PIND6) & 1) << 1)
                 | (((c >> PINC0) & 1) << 2);
    return pgm_read_word(&velocid[num1]);
}

int main() {
    DDRB |= ((1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3));
    DDRD &= ~((1 << DDD7) | (1 << DDD6));
    DDRC &= ~((1 << DDC0));

    PORTD |= (1 << PORTD7) | (1 << PORTD6);
    PORTC |= (1 << PORTC0);
    while (1) {
        uint16_t val;
        val = velocidad();

        PORTB |= (1 << PORTB0);       // sbi PORTB, 0
        delay_ms_var(val);
        PORTB &= ~(1 << PORTB0);      // cbi PORTB, 0
        delay_ms_var(val);
        val = velocidad();

        PORTB |= (1 << PORTB1);       // sbi PORTB, 1
        delay_ms_var(val);
        PORTB &= ~(1 << PORTB1);      // cbi PORTB, 1
        delay_ms_var(val);
        val = velocidad();

        PORTB |= (1 << PORTB2);       // sbi PORTB, 2
        delay_ms_var(val);
        PORTB &= ~(1 << PORTB2);      // cbi PORTB, 2
        delay_ms_var(val);
        val = velocidad();

        PORTB |= (1 << PORTB3);       // sbi PORTB, 1
        delay_ms_var(val);
        PORTB &= ~(1 << PORTB3);      // cbi PORTB, 1
        delay_ms_var(val);
        val = velocidad();

        PORTB |= (1 << PORTB2);       // sbi PORTB, 1
        delay_ms_var(val);
        PORTB &= ~(1 << PORTB2);      // cbi PORTB, 1
        delay_ms_var(val);
        val = velocidad();

        PORTB |= (1 << PORTB1);       // sbi PORTB, 1
        delay_ms_var(val);
        PORTB &= ~(1 << PORTB1);      // cbi PORTB, 1
        delay_ms_var(val);
        val = velocidad();

    }

    return 0;
}