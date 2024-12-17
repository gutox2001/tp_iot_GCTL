const int LDR_PIN = A0;  // Pino analógico onde o LDR está conectado

void setup() {
  Serial.begin(9600);  // Inicia a comunicação serial com a taxa de 9600 bps
  while (!Serial);     // Aguarda a inicialização da comunicação serial

  Serial.println("Iniciando medição de luminosidade...");
}

void loop() {
  int ldrValue = analogRead(LDR_PIN);  // Lê o valor do pino analógico onde o LDR está conectado
  
  // Converte o valor para uma escala de luminosidade (0 a 1023)
  float luminosity = map(ldrValue, 0, 1023, 0, 100); // Aqui mapeamos para uma faixa de 0 a 100% de luminosidade

  // Exibe o valor lido no monitor serial
  Serial.print("Luminosidade: ");
  Serial.print(luminosity);
  Serial.println("%");
  
  delay(1000);  // Aguarda 1 segundo antes de realizar nova leitura
}
