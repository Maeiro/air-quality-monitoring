#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define IO_USERNAME  "Maeiro"
#define IO_KEY       "aio_EBsC916rej5hqTcysMQsKfxJ4NWN"
const char* mqttServer = "io.adafruit.com";
const int mqttPort = 1883;
const char* co2Feed = IO_USERNAME "/feeds/co2-sensor";
const char* ssid = "Kirby";
const char* password = "kirby1401";

// Parâmetros do cálculo do gás
#define RLOAD 10.0
#define RZERO 76.63
#define PARA 116.6020682
#define PARB 2.769034857
#define CORA 0.00035
#define CORB 0.02718
#define CORC 1.39538
#define CORD 0.0018
#define ATMOCO2 397.13

// Configuração dos pinos
int buzzerPin = D1;      // Pino do buzzer
int gasSensorPin = A0;   // Pino analógico do sensor de gás MQ-135
float threshold = 200.0;  // Limite para acionar o buzzer

// Configuração do cliente WiFi e MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Variáveis para a média e tempos de resposta
float ppmSum = 0.0;
int sampleCount = 0;
unsigned long lastSendTime = 0;
unsigned long sensorReadTime, sensorResponseTime, buzzerActivationTime;

// Arrays para armazenar as medições
unsigned long sensorTimes[4];
unsigned long actuatorTimes[4];
int measurementCount = 0; // Contador de medições

// Função para obter a resistência do sensor
float getResistance() {
  int val = analogRead(gasSensorPin);
  if (val == 0) {
    Serial.println("Erro na leitura do sensor: valor inválido.");
    return -1; // Valor indicativo de erro
  }
  return ((1023.0 / (float)val) * 3.3 - 1.0) * RLOAD;
}

// Função para calcular o valor de CO2 em ppm
float getPPM() {
  float resistance = getResistance();
  if (resistance < 0) {
    return 0; // Retorna 0 se houve erro na leitura
  }
  float ppm = PARA * pow((resistance / RZERO), -PARB);
  return ppm;
}

// Função para reconectar ao MQTT caso desconecte
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.println("MQTT desconectado, tentando reconectar...");
    if (client.connect("NodeMCUClient", IO_USERNAME, IO_KEY)) {
      Serial.println("Reconectado ao MQTT");
    } else {
      Serial.print("Falhou, rc=");
      Serial.print(client.state());
      Serial.println(". Tentando novamente em 5 segundos...");
      delay(5000);  // Evitar muitas tentativas consecutivas
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT); // Define o pino do buzzer como saída

  // Conecta-se ao WiFi
  Serial.print("Conectando ao WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado ao WiFi");

  // Configura o servidor MQTT
  client.setServer(mqttServer, mqttPort);
  reconnectMQTT(); // Conectar ao MQTT
}

void loop() {
  // Verifica a conexão ao servidor MQTT e reconecta se necessário
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  // Verifica o estado da conexão WiFi e reconecta se necessário
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi desconectado, tentando reconectar...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nReconectado ao WiFi");
  }

  // Registra o tempo de leitura do sensor
  sensorReadTime = micros();  // Usando micros() para precisão em microssegundos

  // Calcula o valor de CO2 em ppm e acumula
  float ppm = getPPM();
  ppmSum += ppm;
  sampleCount++;

  // Verifica se o nível de CO2 ultrapassa o limite e aciona o buzzer
  if (ppm >= threshold) {
    if (digitalRead(buzzerPin) == LOW) {  // Se o buzzer estava desligado
      digitalWrite(buzzerPin, HIGH);      // Liga o buzzer (nível elevado de CO2)
      buzzerActivationTime = micros() - sensorReadTime;
      actuatorTimes[measurementCount] = buzzerActivationTime;
    }
  } else {
    if (digitalRead(buzzerPin) == HIGH) { // Se o buzzer estava ligado
      digitalWrite(buzzerPin, LOW);       // Desliga o buzzer (nível normal de CO2)
    }
  }

  // Verifica se 2,5 segundos se passaram para enviar a média de CO2
  if (micros() - lastSendTime >= 2500000) {
    float avgPPM = ppmSum / sampleCount; // Calcula a média
    ppmSum = 0.0;                        // Reseta a soma
    sampleCount = 0;                     // Reseta a contagem

    // Calcula o tempo de resposta do sensor e armazena a medição
    sensorResponseTime = micros() - sensorReadTime;
    sensorTimes[measurementCount] = sensorResponseTime;

    // Envia a média de CO2 para o Adafruit IO
    char avgPPMStr[8];
    dtostrf(avgPPM, 1, 2, avgPPMStr);
    client.publish(co2Feed, avgPPMStr);

    // Atualiza o tempo do último envio
    lastSendTime = micros();

    // Incrementa o contador de medições e calcula a média após 4 medições
    measurementCount++;
    if (measurementCount == 4) {
      unsigned long totalSensorTime = 0;
      unsigned long totalActuatorTime = 0;

      for (int i = 0; i < 4; i++) {
        totalSensorTime += sensorTimes[i];
        totalActuatorTime += actuatorTimes[i];
      }

      unsigned long averageSensorTime = totalSensorTime / 4;
      unsigned long averageActuatorTime = totalActuatorTime / 4;

      // Exibe as médias de tempo de resposta do sensor e do atuador
      Serial.println("\n===== Médias de Tempo de Resposta =====");
      Serial.print("Tempo médio de resposta do sensor: ");
      Serial.print(averageSensorTime);
      Serial.println(" µs");

      Serial.print("Tempo médio de resposta do atuador: ");
      Serial.print(averageActuatorTime);
      Serial.println(" µs");
      Serial.println("=======================================\n");

      // Reseta o contador de medições
      measurementCount = 0;
    }
  }

  delay(200);  // Pequeno atraso entre as leituras para melhorar a média
}
