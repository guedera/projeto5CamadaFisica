const int TX_PIN = 7;  
const int BAUDRATE = 9600;
const int BIT_DELAY = 1000000 / BAUDRATE;
const unsigned long CHAR_INTERVAL = 1000000;

const char currentChar = 'A';
unsigned long lastTransmitTime = 0; 
boolean isSending = false;       
int currentBit = 0;            
byte dataToSend = 0;           
byte parityBit = 0;               
unsigned long bitStartTime = 0;

void setup() {
    pinMode(TX_PIN, OUTPUT);
    digitalWrite(TX_PIN, HIGH);
    Serial.begin(9600);
    lastTransmitTime = micros();
}

void loop() {
    unsigned long currentTime = micros();
    
    if (!isSending && (currentTime - lastTransmitTime >= CHAR_INTERVAL)) {
        // Inicia a transmissão do caractere 'A'
        dataToSend = currentChar;
        parityBit = calcParity(dataToSend);
        isSending = true;
        currentBit = -1;
        bitStartTime = currentTime;

        Serial.print("Enviando: ");
        Serial.println((char)dataToSend);
    }
    
    if (isSending && (currentTime - bitStartTime >= BIT_DELAY)) {
        bitStartTime = currentTime;
        
        if (currentBit == -1) {
            // Envia start bit (LOW)
            digitalWrite(TX_PIN, LOW);
            currentBit++;
        } 
        else if (currentBit >= 0 && currentBit < 8) {
            // Envia os 8 bits de dados
            digitalWrite(TX_PIN, (dataToSend >> currentBit) & 1);
            currentBit++;
        } 
        else if (currentBit == 8) {
            // Envia bit de paridade
            digitalWrite(TX_PIN, parityBit);
            currentBit++;
        } 
        else if (currentBit == 9) {
            // Envia stop bit (HIGH)
            digitalWrite(TX_PIN, HIGH);
            currentBit++;
        } 
        else {
            // Completa a transmissão
            isSending = false;
            lastTransmitTime = currentTime;
        }
    }
}

byte calcParity(byte data) {
    byte parity = 0;
    for (int i = 0; i < 8; i++) {
        parity ^= (data >> i) & 1; // XOR de todos os bits
    }
    return parity; // Retorna paridade ímpar (1 se número de 1s for ímpar)
}