# Sistema de Monitoramento da Qualidade do Ar com IoT

Este projeto é um Sistema de Monitoramento da Qualidade do Ar baseado em IoT, desenvolvido para ambientes industriais. Utiliza uma combinação de sensores, microcontroladores e comunicação em nuvem para monitorar níveis de poluição em tempo real e fornece um mecanismo de alerta caso a qualidade do ar caia abaixo de níveis seguros. Este README fornece uma visão geral do projeto, incluindo seus componentes, configuração e uso.

## Visão Geral

Este sistema monitora a qualidade do ar em áreas industriais usando um microcontrolador NodeMCU ESP8266 e um sensor de gás MQ-135. Ele envia dados para a plataforma Adafruit IO, onde podem ser visualizados em tempo real. Além disso, um buzzer é usado para alertar pessoas próximas se os níveis de poluentes excederem um limite definido.

## Componentes Utilizados

- **NodeMCU ESP8266**: O microcontrolador principal, responsável por controlar o sensor e enviar dados para a nuvem.
- **Sensor de Gás MQ-135**: Mede concentrações de gases como CO2, amônia e benzeno para determinar a qualidade do ar.
- **Buzzer**: Fornece um alarme sonoro quando os níveis de poluentes excedem um limite definido (50 ppm).
- **Breadboard e Cabos Jumper**: Utilizados para conectar componentes sem a necessidade de solda.

## Funcionalidades

- Monitoramento em tempo real da qualidade do ar usando o sensor MQ-135.
- Conectividade Wi-Fi para enviar dados para a plataforma Adafruit IO utilizando o protocolo MQTT.
- Alerta sonoro usando um buzzer quando os níveis de poluentes estão inseguros.
- Visualização dos dados através dos feeds do Adafruit IO.

## Iniciando

### Configuração do Hardware

1. **Conecte o Sensor MQ-135** ao pino analógico (`A0`) do NodeMCU. Este sensor mede a qualidade do ar detectando níveis de gás e gera um sinal analógico proporcional à concentração detectada.
2. **Conecte o Buzzer** ao pino digital (`D1`) do NodeMCU. O buzzer será utilizado para indicar quando os níveis de poluição ultrapassarem um limite de segurança.
3. **Alimente o Sistema** via USB, que fornece 5V para o NodeMCU e todos os componentes conectados.

### Configuração do Software

1. **Instale a IDE do Arduino**: Baixe e instale a IDE do Arduino a partir de [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software).
2. **Clone o Repositório**:
   ```sh
   git clone https://github.com/link_do_repositorio
   cd air-quality-monitoring-system
   ```
3. **Instale as Dependências** na IDE do Arduino:

   - Abra a IDE do Arduino, vá para **Sketch > Incluir Biblioteca > Gerenciar Bibliotecas...**.
   - Procure e instale as seguintes bibliotecas:
     - **ESP8266WiFi** da ESP8266 Community
     - **PubSubClient** por Nick O'Leary
     - **Adafruit IO Arduino** da Adafruit

4. **Configure as Credenciais de Wi-Fi e MQTT**: Abra o arquivo `src/main.ino` e atualize as seguintes linhas:
   ```cpp
   const char* ssid = "SEU_WIFI_SSID";
   const char* password = "SUA_SENHA_WIFI";
   #define IO_USERNAME  "SEU_USUARIO_ADAFRUIT"
   #define IO_KEY       "SUA_CHAVE_ADAFRUIT"
   ```

### Upload do Código

1. **Conecte** seu NodeMCU ESP8266 ao computador usando um cabo USB.
2. **Selecione a Placa e a Porta** na IDE do Arduino:
   - Vá até **Ferramentas > Placa** e selecione `NodeMCU 1.0 (ESP-12E Module)`.
   - Vá até **Ferramentas > Porta** e selecione a porta à qual o NodeMCU está conectado (por exemplo, `COM6`).
3. **Faça o Upload do Código**:
   - Clique no botão **Upload** na IDE do Arduino para enviar o código para o NodeMCU.

## Como Funciona

- **Conectividade Wi-Fi**: O NodeMCU conecta-se a uma rede Wi-Fi local utilizando as credenciais fornecidas no código.
- **Aquisição de Dados**: O sensor MQ-135 lê continuamente os dados da qualidade do ar, que são processados para calcular a concentração de CO2 em partes por milhão (ppm).
- **Envio de Dados**: A cada 2,5 segundos, o NodeMCU envia as leituras médias de CO2 para o feed do Adafruit IO usando o protocolo MQTT.
- **Mecanismo de Alerta**: Quando os níveis de poluentes excedem 50 ppm, o buzzer é ativado para fornecer um aviso sonoro aos usuários próximos.

## Monitoramento via Adafruit IO

- Os dados são enviados para o Adafruit IO e podem ser visualizados em tempo real através do feed chamado `co2-sensor`. Você pode criar um painel no Adafruit IO para visualizar os dados históricos e monitorar as tendências.

## Resultados e Desempenho

- O sistema detecta com sucesso mudanças na qualidade do ar e envia dados para o Adafruit IO para visualização.

## Demonstração em Vídeo

- Uma demonstração em vídeo do sistema em ação está disponível no YouTube: [Link para a Demonstração em Vídeo](https://www.youtube.com/watch?v=MxF7MBbYLZM). Neste vídeo, você pode ver como o sistema responde a mudanças na qualidade do ar e envia dados para o Adafruit IO.

## Melhorias e Trabalhos Futuros

- **Calibração do Sensor**: A calibração do sensor MQ-135 pode ser melhorada para fornecer medições de ppm mais precisas.
- **Interface do Usuário**: Uma interface web mais amigável poderia ser criada para o monitoramento remoto da qualidade do ar.
- **Sensores Adicionais**: Integrar sensores ambientais adicionais, como temperatura ou umidade, para fornecer mais contexto às medições da qualidade do ar.

## Conteúdo do Repositório

- **src/**: Contém o código principal do projeto (`main.ino`) que controla as leituras dos sensores, a comunicação MQTT e os alertas do buzzer.
- **README.md**: Este arquivo de documentação que fornece todos os detalhes necessários sobre o projeto.

## Licença

Este projeto está licenciado sob a Licença MIT. Sinta-se à vontade para usar, modificar e distribuir conforme necessário.

## Contribuindo

Contribuições são bem-vindas! Por favor, envie um pull request ou abra uma issue para sugestões e melhorias.

## Contato

Para perguntas ou mais informações, entre em contato com [Maeiro]
