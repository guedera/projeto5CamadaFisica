#define TX_PIN 7        // Pino digital usado para transmissão
#define RX_PIN 8        // Pino digital usado para recepção
#define BAUD_RATE 9600  // Taxa de transmissão em bits/s

// Configuração como receptor apenas
#define RECEIVER
// Transmissor desabilitado neste Arduino

// Constantes para tempo (calculadas para 9600 baud)
const unsigned long BIT_DELAY_US = 1000000UL / BAUD_RATE;  // Tempo por bit em microssegundos

// Função que calcula paridade (paridade par)
bool calcParity(byte data) {
  byte parity = data;
  parity ^= parity >> 4;  // xor dos primeiros 4 bits com os últimos 4 bits
  parity ^= parity >> 2;  // xor dos primeiros 2 bits com os últimos 2 bits
  parity ^= parity >> 1;  // xor dos primeiros bit com o último bit
  return parity & 0x01;   // retorna o bit menos significativo (LSB)
}

// Função de espera precisa sem usar delay
void preciseMicrosDelay(unsigned long us) {
  unsigned long start = micros();
  while (micros() - start < us) {
    asm("NOP");  // Operação nula para evitar que o compilador otimize o loop vazio
  }
}

// Função para transmitir um byte usando o protocolo UART
void uartTransmit(byte data) {
  bool parityBit = calcParity(data);  // Calcula bit de paridade (paridade par)
  
  // Start bit (nível lógico baixo)
  digitalWrite(TX_PIN, LOW);
  preciseMicrosDelay(BIT_DELAY_US);
  
  // Bits de dados (LSB primeiro)
  for (int i = 0; i < 8; i++) {
    digitalWrite(TX_PIN, (data >> i) & 0x01);  // Envia cada bit, começando pelo LSB
    preciseMicrosDelay(BIT_DELAY_US);
  }
  
  // Bit de paridade
  digitalWrite(TX_PIN, parityBit);
  preciseMicrosDelay(BIT_DELAY_US);
  
  // Stop bit (nível lógico alto)
  digitalWrite(TX_PIN, HIGH);
  preciseMicrosDelay(BIT_DELAY_US);
  
  // Mantem a linha em nível alto entre transmissões
  digitalWrite(TX_PIN, HIGH);
}

// Função para receber um byte usando o protocolo UART
bool uartReceive(byte &receivedData) {
  // Aguarda o start bit (transição de HIGH para LOW)
  while (digitalRead(RX_PIN) == HIGH) {
    // Espera pelo start bit
  }
  
  // Confirma que é realmente o start bit (evita falsos positivos)
  preciseMicrosDelay(BIT_DELAY_US / 2);  // Amostra no meio do bit
  if (digitalRead(RX_PIN) != LOW) {
    return false;  // Falso start bit
  }
  
  preciseMicrosDelay(BIT_DELAY_US);  // Avança para o primeiro bit de dados
  
  // Lê os 8 bits de dados
  byte data = 0;
  for (int i = 0; i < 8; i++) {
    data |= digitalRead(RX_PIN) << i;  // Lê cada bit e coloca na posição correta
    preciseMicrosDelay(BIT_DELAY_US);
  }
  
  // Lê o bit de paridade
  bool receivedParity = digitalRead(RX_PIN);
  preciseMicrosDelay(BIT_DELAY_US);
  
  // Lê o stop bit
  bool stopBit = digitalRead(RX_PIN);
  
  // Verifica a paridade
  bool calculatedParity = calcParity(data);
  bool parityError = (receivedParity != calculatedParity);
  
  // Verifica o stop bit
  bool frameError = (stopBit != HIGH);
  
  receivedData = data;
  
  // Reporta erros separadamente para diagnóstico mais preciso
  if (parityError) {
    Serial.println("ERRO: Paridade incorreta detectada!");
  }
  
  if (frameError) {
    Serial.println("ERRO: Stop bit incorreto detectado!");
  }
  
  return !parityError && !frameError;
}

void setup() {
  Serial.begin(9600);  // Inicializa comunicação serial com o computador
  
  pinMode(RX_PIN, INPUT);
  Serial.println("Modo: Receptor UART");
  Serial.print("Pino RX: ");
  Serial.println(RX_PIN);
  Serial.print("Baud Rate: ");
  Serial.println(BAUD_RATE);
  
  Serial.println("Aguardando dados...");
  delay(1000);  // Dá tempo para o usuário abrir o monitor serial
}

void loop() {
  byte receivedByte;
  bool success = uartReceive(receivedByte);
  
  if (success) {
    Serial.print("Recebido: ");
    Serial.print((char)receivedByte);
    Serial.print(" (ASCII: ");
    Serial.print(receivedByte, DEC);
    Serial.println(") - OK");
  } else {
    Serial.println("Erro de transmissão detectado!");
  }
}
