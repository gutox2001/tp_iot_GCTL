/*
  Projeto: Captura de Temperatura, Ruído e Luminosidade
  Placa: Arduino Nano 33 BLE Sense
  Descrição:
    - Captura dados de temperatura e pressão usando o sensor LPS22HB.
    - Captura dados de ruído usando o microfone PDM.
    - Captura dados de luminosidade usando um LDR.
    - Envia os dados para o Monitor Serial.
*/

#include <Arduino_LPS22HB.h>  // Biblioteca do sensor de pressão/temperatura
#include <PDM.h>              // Biblioteca do microfone PDM

// Definindo o pino do LDR (fotoresistor)
const int LDR_PIN = A0;       // Pino analógico onde o LDR está conectado
const int NUM_SAMPLES = 100;  // Número de amostras para média
int ldrValues[NUM_SAMPLES];   // Array para armazenar as amostras
int sampleIndex = 0;

// Variáveis do Microfone
short sampleBuffer[256];
volatile int samplesRead = 0;

// Variáveis de Controle
bool recording = true;  // Ativar gravação automaticamente

void setup() {
    Serial.begin(9600);
    while (!Serial);

    // Inicializa o Sensor de Temperatura e Pressão
    if (!BARO.begin()) {
        Serial.println("Falha ao iniciar o sensor de temperatura e pressão!");
        while (1);
    }

    // Inicializa o Microfone PDM
    PDM.onReceive(onPDMdata);    // Callback para processar os dados do microfone
    if (!PDM.begin(1, 16000)) {  // Mono, 16kHz
        Serial.println("Falha ao iniciar o microfone PDM!");
        while (1);
    }

    Serial.println("Projeto: Captura de Temperatura, Ruído e Luminosidade");
    Serial.println("Iniciando coleta de dados...");
}

void loop() {
    // 1. Captura de Dados do Sensor de Temperatura e Pressão
    float temperatura = BARO.readTemperature();
    float pressao = BARO.readPressure();

    // 2. Captura e Processamento de Dados do Microfone (Ruído)
    if (samplesRead) {
        long soma = 0;
        for (int i = 0; i < samplesRead; i++) {
            soma += abs(sampleBuffer[i]);  // Soma os valores absolutos
        }
        int nivelRuido = soma / samplesRead;  // Calcula a média simples
        samplesRead = 0;

        // 3. Captura de Dados de Luminosidade (LDR)
        ldrValues[sampleIndex] = analogRead(LDR_PIN);
        sampleIndex = (sampleIndex + 1) % NUM_SAMPLES;

        // Calcula a média das últimas amostras do LDR
        long somaLDR = 0;
        for (int i = 0; i < NUM_SAMPLES; i++) {
            somaLDR += ldrValues[i];
        }
        int avgLdrValue = somaLDR / NUM_SAMPLES;

        // Converte o valor de luminosidade para uma escala de 0 a 100%
        float luminosidade = map(avgLdrValue, 0, 1023, 0, 100);

        // 4. Envio dos Dados para o Monitor Serial
        Serial.print("Temperatura: ");
        Serial.print(temperatura);
        Serial.print(" °C\t");

        Serial.print("Pressão: ");
        Serial.print(pressao / 100.0);  // Converte para hPa
        Serial.print(" hPa\t");

        Serial.print("Nível de Ruído: ");
        Serial.print(nivelRuido);
        Serial.print("\t");

        Serial.print("Luminosidade: ");
        Serial.print(luminosidade);
        Serial.println("%");
    }

    delay(1000);  // Aguarda 1 s antes da próxima leitura
}

void onPDMdata() {
    // Callback para o microfone PDM
    int bytesAvailable = PDM.available();
    PDM.read(sampleBuffer, bytesAvailable);
    samplesRead = bytesAvailable / 2;
}
