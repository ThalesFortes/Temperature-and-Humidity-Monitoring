# 🌡️ Monitoramento de Temperatura e Umidade com AHT10 no Raspberry Pi Pico  

Projeto desenvolvido para monitorar **temperatura** e **umidade relativa do ar** utilizando o sensor **AHT10** conectado ao **Raspberry Pi Pico** via protocolo I2C.  
As informações são exibidas em um **display OLED SSD1306** e há sinais visuais (LEDs) e sonoros (buzzer) para indicar condições críticas.  

---

## 📌 Funcionalidades  
- 📊 Leitura de **temperatura (°C)** e **umidade (%)** com o sensor AHT10.  
- 🖥️ Exibição em tempo real no display OLED SSD1306.  
- 🔵 LED Azul: temperatura abaixo de 0 °C.  
- 🟢 LED Verde: ambiente ideal (20 °C a 30 °C).  
- 🔴 LED Vermelho: temperatura ≥ 35 °C (quando a umidade não está alta).  
- 🚨 LED Vermelho piscando: alerta de **umidade alta** (> 70%).  
- 🔔 Buzzer: dispara quando a temperatura atinge **40 °C** ou mais.  
- 🖥️ Saída de debug via **serial USB**.  

---

## 🛠️ Componentes Utilizados  
- 🖥️ **Raspberry Pi Pico**  
- 🌡️ **Sensor AHT10** (I2C)  
- 📺 **Display OLED SSD1306**  
- 🔴🟢🔵 LEDs (Vermelho, Verde e Azul)  
- 🔔 Buzzer passivo  
- 📡 Resistores pull-up (já ativados via software)  

---

## 📂 Estrutura do Projeto  

📦 Projeto_AHT10
┣ 📜 main.c # Código principal
┣ 📜 aht10.h # Header do sensor AHT10
┣ 📜 aht10.c # Implementação do sensor AHT10
┣ 📂 inc/ # Bibliotecas externas (ex: SSD1306)
┃ ┣ 📜 ssd1306.h
┃ ┣ 📜 ssd1306.c
┃ ┣ 📜 ssd1306_fonts.h
┣ 📜 CMakeLists.txt # Configuração do build
┗ 📜 README.md # Documentação do projeto

---

## ⚙️ Como Compilar e Executar  

1. Clone este repositório:  
   ```bash
   git clone https://github.com/seu-usuario/Projeto_AHT10.git
   cd Projeto_AHT10

Configure o ambiente do Pico SDK no seu computador.

Compile o projeto com CMake:

mkdir build
cd build
cmake ..
make

Conecte o Raspberry Pi Pico em modo BOOTSEL e copie o arquivo .uf2 gerado.

