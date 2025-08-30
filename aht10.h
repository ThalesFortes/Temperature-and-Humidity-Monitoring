#ifndef AHT10_H
#define AHT10_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Endereço padrão do AHT10
#define AHT10_I2C_ADDR 0x38

typedef struct {
    float temp_celsius;
    float umidade_relativa;
} AHT10_t;

bool AHT10_inicializar(i2c_inst_t *i2c);

bool AHT10_obterMedicao(i2c_inst_t *i2c, AHT10_t *medida);

#endif
