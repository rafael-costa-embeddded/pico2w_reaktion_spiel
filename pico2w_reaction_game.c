#include <stdio.h>
#include "pico/stdlib.h"

#define BUTTON_PIN 15
#define LED_PIN 16

void flash_led(int times) {
    for (int i = 0; i < times; i++) {
        gpio_put(LED_PIN, 1);
        sleep_ms(150);
        gpio_put(LED_PIN, 0);
        sleep_ms(150);
    }
}

bool button_pressed(void) {
    return !gpio_get(BUTTON_PIN); //pressed = 0, not pressed = 1
}

int main()
{
    stdio_init_all();

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_put(LED_PIN, 0); // Ensure LED is off at start

    sleep_ms(2000); // wait 2 seconds before starting

    printf("Spiel angefangen, auf LED warten und Knopf drücken.\n");

    while (true) {

        gpio_put(LED_PIN, 0);
        
        uint32_t random_delay = 2000 + (time_us_32() % 3000); //random delay

        sleep_ms(random_delay);

        gpio_put(LED_PIN, 1);

        absolute_time_t start_time = get_absolute_time();

        while (!button_pressed()) {
            sleep_ms(1); // wait for button press
        }

        absolute_time_t end_time = get_absolute_time();

        int64_t reaction_time_us = absolute_time_diff_us(start_time, end_time);
        int64_t reaction_time_ms = reaction_time_us / 1000;

        gpio_put(LED_PIN, 0);

        printf("Reaktionszeit: %lld ms\n", reaction_time_ms);

        if (reaction_time_ms < 250) {
            printf("schnell");
        }
        else if (reaction_time_ms < 400) {
            printf("mittel");
        }
        else {
            printf("langsam");
        }
    }
}
