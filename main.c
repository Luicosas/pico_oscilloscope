#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    stdio_init_all();
    int num = 0;
    while (true) {
        printf("Hello, world! %d\n", num++);
        sleep_ms(1000);
    }
}