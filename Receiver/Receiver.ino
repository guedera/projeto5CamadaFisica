const int RX_PIN = 8; // Pino digital para recepção
const int BAUDRATE = 9600;
const int BIT_DELAY = 1000000 / BAUDRATE; // Tempo entre bits em microssegundos

void setup() {
    pinMode(RX_PIN, INPUT);
    Serial.begin(9600);
}

void loop() {
    char receivedChar = receiveUART();
    Serial.print("Caractere recebido: ");
    Serial.println(receivedChar);
    delay(1000); // Apenas para espaçar as leituras no monitor serial
}

char receiveUART() {
    while (digitalRead(RX_PIN) == HIGH); // Espera pelo bit de start
    delayMicroseconds(BIT_DELAY * 1.5); // Aguarda no meio do primeiro bit
    
    byte data = 0;
    for (int i = 0; i < 8; i++) {
        data |= (digitalRead(RX_PIN) << i);
        delayMicroseconds(BIT_DELAY);
    }
    
    byte parity = digitalRead(RX_PIN); // Lê bit de paridade
    delayMicroseconds(BIT_DELAY);
    
    digitalRead(RX_PIN); // Lê bit de stop (ignorado, mas necessário)
    delayMicroseconds(BIT_DELAY);
    
    // Verificação de paridade (opcional, depende do critério de erro permitido)
    if (calcParity(data) != parity) {
        Serial.println("Erro de paridade!");
    }
    
    return data;
}

byte calcParity(byte data) {
    byte parity = 0;
    for (int i = 0; i < 8; i++) {
        parity ^= (data >> i) & 1;
    }
    return parity;
}
