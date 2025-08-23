#include "aht10.h"

// Comandos principais
static const uint8_t AHT10_CMD_INIT[]    = {0xE1, 0x08, 0x00};
static const uint8_t AHT10_CMD_TRIGGER[] = {0xAC, 0x33, 0x00};

bool AHT10_inicializar(i2c_inst_t *i2c) {
    int resultado = i2c_write_blocking(i2c, AHT10_I2C_ADDR, AHT10_CMD_INIT, sizeof(AHT10_CMD_INIT), false);
    if (resultado < 0) return false;

    sleep_ms(25); // tempo de estabilização
    return true;
}

bool AHT10_obterMedicao(i2c_inst_t *i2c, AHT10_t *medida) {
    // Solicita medição
    if (i2c_write_blocking(i2c, AHT10_I2C_ADDR, AHT10_CMD_TRIGGER, sizeof(AHT10_CMD_TRIGGER), false) < 0)
        return false;

    sleep_ms(80); // aguarda conversão

    // Lê resposta
    uint8_t resposta[6];
    if (i2c_read_blocking(i2c, AHT10_I2C_ADDR, resposta, sizeof(resposta), false) < 0)
        return false;

    // Verifica se sensor está pronto e calibrado
    if ((resposta[0] & 0x88) != 0x08)
        return false;

    // Processa umidade
    uint32_t raw_umid = ((uint32_t)resposta[1] << 12) | ((uint32_t)resposta[2] << 4) | (resposta[3] >> 4);
    medida->umidade_relativa = (raw_umid / 1048576.0f) * 100.0f;

    // Processa temperatura
    uint32_t raw_temp = (((uint32_t)resposta[3] & 0x0F) << 16) | ((uint32_t)resposta[4] << 8) | resposta[5];
    medida->temp_celsius = (raw_temp / 1048576.0f) * 200.0f - 50.0f;

    return true;
}
