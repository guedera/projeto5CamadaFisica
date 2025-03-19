#define TX_PIN 7        // Pino digital usado para transmissão
#define RX_PIN 8        // Pino digital usado para recepção
#define BAUD_RATE 9600  // Taxa de transmissão em bits/s

// Configuração como transmissor apenas
#define TRANSMITTER
// Receptor desabilitado neste Arduino

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

void setup() {
  Serial.begin(9600);  // Inicializa comunicação serial com o computador
  
  pinMode(TX_PIN, OUTPUT);
  digitalWrite(TX_PIN, HIGH);  // Linha idle em nível alto
  Serial.println("Modo: Transmissor UART");
  Serial.print("Pino TX: ");
  Serial.println(TX_PIN);
  Serial.print("Baud Rate: ");
  Serial.println(BAUD_RATE);
  
  Serial.println("Iniciando transmissão...");
  delay(1000);  // Dá tempo para o usuário abrir o monitor serial
}

// Variável global para armazenar o caractere a ser enviado
char charToSend = 'A';

void loop() {
  Serial.print("Enviando: ");
  Serial.print(charToSend);
  Serial.print(" (ASCII: ");
  Serial.print((byte)charToSend, DEC);
  Serial.println(")");
  
  uartTransmit(charToSend);
  
  // Aguarda um pouco antes de enviar o próximo caractere
  delay(1000);
  
  // Próximo caractere (para demonstrar múltiplos envios)
  if (charToSend == 'Z') {
    charToSend = 'A';
  } else {
    charToSend++;
  }
}
