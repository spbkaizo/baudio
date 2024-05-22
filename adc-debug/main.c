#include "usart.h"
#include "adc.h"
#include <util/delay.h>
#include <stdio.h>

void send_adc_value(uint8_t channel) {
    uint16_t adc_value = read_adc(channel);
    char buffer[30];
    sprintf(buffer, "ADC Value: %u\r\n", adc_value);
    usart_write_string(buffer);
}

int main(void) {
    usart_init();  // Initialize USART
    setup_adc();   // Initialize ADC
    usart_write_string("hai");
    while (1) {
        send_adc_value(ADC_CHANNEL_RIGHT);  // Send ADC reading to USART
        _delay_ms(500);  // Simple delay for readability
    }
}
