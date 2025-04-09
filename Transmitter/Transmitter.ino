const int TX_PIN = 7;  //Pino de transmissão
const int BAUDRATE = 9600;  //Taxa de transmissão
const int BIT_DELAY = 1000000 / BAUDRATE;  //Delay entre bits
const unsigned long CHAR_INTERVAL = 1000000;  //Intervalo entre caracteres

const char currentChar = 'A';  //Caractere atual
unsigned long lastTransmitTime = 0;  //Última vez que transmitiu
boolean isSending = false;  //Se está enviando
int currentBit = 0;  //Bit atual
byte dataToSend = 0;  //Dados para enviar
byte parityBit = 0;  //Bit de paridade
unsigned long bitStartTime = 0;  //Tempo de início do bit

void setup() {
    pinMode(TX_PIN, OUTPUT);  //Configura pino como saída
    digitalWrite(TX_PIN, HIGH);  //Inicializa pino em HIGH
    Serial.begin(9600);  //Inicia comunicação serial
    lastTransmitTime = micros();  //Marca o tempo atual
}

void loop() {
    unsigned long currentTime = micros();  //Tempo atual
    
    if (!isSending && (currentTime - lastTransmitTime >= CHAR_INTERVAL)) {
        dataToSend = currentChar;  //Pega o caractere atual
        parityBit = calcParity(dataToSend);  //Calcula paridade
        isSending = true;  //Começa a enviar
        currentBit = -1;  //Começa com o start bit
        bitStartTime = currentTime;  //Marca o tempo de início
        Serial.receivedData = 0;
        Serial.print("Enviando: ");  //Debug
        Serial.println((char)dataToSend);  //Debug
    }
    
    if (isSending && (currentTime - bitStartTime >= BIT_DELAY)) {
        bitStartTime = currentTime;  //Atualiza tempo de início
        
        if (currentBit == -1) {
            digitalWrite(TX_PIN, LOW);  //Envia start bit
            currentBit++;
        } 
        else if (currentBit >= 0 && currentBit < 8) {
            digitalWrite(TX_PIN, (dataToSend >> currentBit) & 1);  //Envia bits de dados
            currentBit++;
        } 
        else if (currentBit == 8) {
            digitalWrite(TX_PIN, parityBit);  //Envia bit de paridade
            currentBit++;
        } 
        else if (currentBit == 9) {
            digitalWrite(TX_PIN, HIGH);  //Envia stop bit
            currentBit++;
        } 
        else {
            isSending = false;  //Termina envio
            lastTransmitTime = currentTime;  //Atualiza tempo de última transmissão
        }
    }
}

byte calcParity(byte data) {
    byte parity = 0;
    for (int i = 0; i < 8; i++) {
        parity ^= (data >> i) & 1;  //Calc paridade
    }
    return parity;
}