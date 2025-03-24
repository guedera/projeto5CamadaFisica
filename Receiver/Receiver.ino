const int RX_PIN = 8; // Pino digital para recepção
const int BAUDRATE = 9600;
const int BIT_DELAY = 1000000 / BAUDRATE; // Tempo entre bits em microssegundos

unsigned long lastReceiveTime = 0;
boolean isReceiving = false;
int currentBit = 0;
byte receivedData = 0;
byte parityBit = 0;

void setup() {
    pinMode(RX_PIN, INPUT);
    Serial.begin(9600);
}

void loop() {
    unsigned long currentTime = micros();
    
    if (!isReceiving && digitalRead(RX_PIN) == LOW) {
        isReceiving = true;
        lastReceiveTime = currentTime;
        currentBit = -1;
    }
    
    if (isReceiving && (currentTime - lastReceiveTime >= BIT_DELAY)) {
        lastReceiveTime = currentTime;
        
        if (currentBit == -1) {
            currentBit++;
        } 
        else if (currentBit >= 0 && currentBit < 8) {
            receivedData |= (digitalRead(RX_PIN) << currentBit);
            currentBit++;
        } 
        else if (currentBit == 8) {
            parityBit = digitalRead(RX_PIN);
            currentBit++;
        } 
        else if (currentBit == 9) {
            isReceiving = false;
            if (calcParity(receivedData) != parityBit) {
                Serial.println("Erro de paridade!");
            } else {
                Serial.print("Caractere recebido: ");
                Serial.println((char)receivedData);
            }
            receivedData = 0;
        }
    }
}

byte calcParity(byte data) {
    byte parity = 0;
    for (int i = 0; i < 8; i++) {
        parity ^= (data >> i) & 1;
    }
    return parity;
}
