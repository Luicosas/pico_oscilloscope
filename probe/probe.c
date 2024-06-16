#include "stdio.h"
#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/dma.h"

#define MICROSECONDS_PER_SECOND 1000000

int main() {
    stdio_init_all();

    while(true) {
        int64_t bytes_outputted = 0;
        int64_t start_time = time_us_64();
        while(time_us_64() < start_time + MICROSECONDS_PER_SECOND) {
            printf("abcdefghijklmnopqrstuvwxyz");
            bytes_outputted += 26;
        }
        int64_t end_time = time_us_64();
        
        putchar_raw(0);
        printf("Pico: %lld bytes sent in %lld microseconds", bytes_outputted, end_time - start_time);
        putchar_raw('\n'); // to get around the fact that printf converts '\n' to '\r\n'
        putchar_raw(0);
        
        stdio_flush();
    }
}