#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "aht10.h"
#include "inc/ssd1306.h"
#include "inc/ssd1306_fonts.h"

// Configuração do I2C
#define I2C_PORT i2c0
const uint SDA_PIN = 0;
const uint SCL_PIN = 1;

// GPIOs para LEDs e buzzer
#define LED_AZUL     12
#define LED_VERDE    11
#define LED_VERMELHO 13
#define BUZZER       10

// Controle do buzzer
absolute_time_t proxima_vez_buzzer;

static void setup_gpio() {
    gpio_init(LED_AZUL);     gpio_set_dir(LED_AZUL, GPIO_OUT);
    gpio_init(LED_VERDE);    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_init(LED_VERMELHO); gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_init(BUZZER);       gpio_set_dir(BUZZER, GPIO_OUT);
}

int main() {
    stdio_init_all();


    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    setup_gpio();


    if (!AHT10_inicializar(I2C_PORT)) {
        printf("Falha ao inicializar AHT10!\n");
        while (true);
    }


    ssd1306_Init();
    ssd1306_Fill(Black);
    ssd1306_UpdateScreen();

    proxima_vez_buzzer = get_absolute_time();

    while (true) {
        AHT10_t leitura;

        if (AHT10_obterMedicao(I2C_PORT, &leitura)) {
            char buffer[32];

           
            ssd1306_Fill(Black);
            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString("Monitor AHT10", Font_7x10, White);

            snprintf(buffer, sizeof(buffer), "Temp: %.1f C", leitura.temp_celsius);
            ssd1306_SetCursor(0, 12);
            ssd1306_WriteString(buffer, Font_7x10, White);

            snprintf(buffer, sizeof(buffer), "Umid: %.1f %%", leitura.umidade_relativa);
            ssd1306_SetCursor(0, 24);
            ssd1306_WriteString(buffer, Font_7x10, White);

            if (leitura.umidade_relativa > 70.0f || leitura.temp_celsius < 20.0f)
                ssd1306_WriteString("ALERTA: CRITICO!", Font_6x8, White);
            else
                ssd1306_WriteString("Ambiente OK", Font_6x8, White);

            ssd1306_UpdateScreen();

           
            printf("Temp: %.2f C | Umid: %.2f %%\n", leitura.temp_celsius, leitura.umidade_relativa);

            
            gpio_put(LED_AZUL, leitura.temp_celsius < 0.0f);
            gpio_put(LED_VERDE, leitura.temp_celsius >= 20.0f && leitura.temp_celsius <= 30.0f);

            bool umidAlta = leitura.umidade_relativa > 70.0f;
            gpio_put(LED_VERMELHO, (!umidAlta && leitura.temp_celsius >= 35.0f));

          
            if (leitura.temp_celsius >= 40.0f) {
                if (absolute_time_diff_us(proxima_vez_buzzer, get_absolute_time()) <= 0) {
                    gpio_put(BUZZER, 1);
                    sleep_ms(200);
                    gpio_put(BUZZER, 0);
                    proxima_vez_buzzer = make_timeout_time_ms(10000);
                }
            }

            
            if (umidAlta) {
                for (int i = 0; i < 2; i++) {
                    gpio_put(LED_VERMELHO, 1);
                    sleep_ms(400);
                    gpio_put(LED_VERMELHO, 0);
                    sleep_ms(400);
                }
                continue;
            }
        } else {
            printf("Erro na leitura do AHT10\n");
        }

        sleep_ms(2000);
    }

    return 0;
}
