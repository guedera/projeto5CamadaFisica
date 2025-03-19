const int TX_PIN = 7; // Pino digital para transmissão
const int BAUDRATE = 9600;
const int BIT_DELAY = 1000000 / BAUDRATE; // Tempo entre bits em microssegundos

void setup() {
    pinMode(TX_PIN, OUTPUT);
    digitalWrite(TX_PIN, HIGH); // Linha ociosa em HIGH
}

void loop() {
    char data = 'A'; // Caractere a ser transmitido
    sendUART(data);
    delay(1000); // Espera para repetir a transmissão
}

void sendUART(char data) {
    byte parity = calcParity(data); // Calcula bit de paridade
    
    // Enviar bit de start (LOW)
    digitalWrite(TX_PIN, LOW);
    delayMicroseconds(BIT_DELAY);
    
    // Enviar 8 bits de dados
    for (int i = 0; i < 8; i++) {
        digitalWrite(TX_PIN, (data >> i) & 1);
        delayMicroseconds(BIT_DELAY);
    }
    
    // Enviar bit de paridade
    digitalWrite(TX_PIN, parity);
    delayMicroseconds(BIT_DELAY);
    
    // Enviar bit de stop (HIGH)
    digitalWrite(TX_PIN, HIGH);
    delayMicroseconds(BIT_DELAY);
}

byte calcParity(byte data) {
    byte parity = 0;
    for (int i = 0; i < 8; i++) {
        parity ^= (data >> i) & 1; // XOR de todos os bits
    }
    return parity; // Retorna paridade ímpar (1 se número de 1s for ímpar)
}
