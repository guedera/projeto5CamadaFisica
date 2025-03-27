# Projeto de Comunicação Física - Transmitter

O Transmitter é responsável por enviar dados de forma serial, bit a bit, utilizando um protocolo simples que inclui bits de início, dados, paridade e parada. Abaixo está uma explicação linha a linha do código do Transmitter e os conceitos envolvidos.

---

## Explicação do Código

### Definições Iniciais
```ino
const int TX_PIN = 7;  
const int BAUDRATE = 9600;
const int BIT_DELAY = 1000000 / BAUDRATE;
const unsigned long CHAR_INTERVAL = 1000000;
```
- `TX_PIN`: Define o pino digital usado para transmissão.
- `BAUDRATE`: Taxa de transmissão em bits por segundo (bps).
- `BIT_DELAY`: Calcula o tempo necessário para transmitir um bit, em microssegundos.
- `CHAR_INTERVAL`: Intervalo entre o envio de caracteres, em microssegundos.

```ino
const char currentChar = 'A';
unsigned long lastTransmitTime = 0; 
boolean isSending = false;       
int currentBit = 0;            
byte dataToSend = 0;           
byte parityBit = 0;               
unsigned long bitStartTime = 0;
```
- `currentChar`: O caractere que será enviado (neste caso, sempre 'A').
- `lastTransmitTime`: Marca o tempo da última transmissão.
- `isSending`: Indica se o Transmitter está enviando dados.
- `currentBit`: Controla qual bit está sendo enviado no momento.
- `dataToSend`: Armazena o dado a ser enviado.
- `parityBit`: Calcula o bit de paridade para verificação de erros.
- `bitStartTime`: Marca o início do envio de cada bit.

---

### Configuração Inicial
```ino
void setup() {
    pinMode(TX_PIN, OUTPUT);
    digitalWrite(TX_PIN, HIGH);
    Serial.begin(9600);
    lastTransmitTime = micros();
}
```
- Configura o pino de transmissão como saída (`OUTPUT`).
- Define o estado inicial do pino como `HIGH` (nível lógico alto).
- Inicializa a comunicação serial para depuração.
- Armazena o tempo inicial para controle do intervalo entre transmissões.

---

### Loop Principal
```ino
void loop() {
    unsigned long currentTime = micros();
```
- Obtém o tempo atual em microssegundos para controlar os intervalos de transmissão.

#### Início da Transmissão
```ino
    if (!isSending && (currentTime - lastTransmitTime >= CHAR_INTERVAL)) {
        dataToSend = currentChar;
        parityBit = calcParity(dataToSend);
        isSending = true;
        currentBit = -1;
        bitStartTime = currentTime;

        Serial.print("Enviando: ");
        Serial.println((char)dataToSend);
    }
```
- Verifica se não está enviando e se o intervalo entre transmissões foi atingido.
- Prepara o dado a ser enviado (`dataToSend`) e calcula o bit de paridade.
- Define que a transmissão está ativa (`isSending = true`) e reinicia o controle de bits.
- Exibe no monitor serial o caractere que será enviado.

#### Envio dos Bits
```ino
    if (isSending && (currentTime - bitStartTime >= BIT_DELAY)) {
        bitStartTime = currentTime;
```
- Verifica se está enviando e se o tempo necessário para enviar o próximo bit foi atingido.

##### Envio do Start Bit
```ino
        if (currentBit == -1) {
            digitalWrite(TX_PIN, LOW);
            currentBit++;
        }
```
- O start bit é sempre `LOW` (nível lógico baixo) e indica o início da transmissão.

##### Envio dos Bits de Dados
```ino
        else if (currentBit >= 0 && currentBit < 8) {
            digitalWrite(TX_PIN, (dataToSend >> currentBit) & 1);
            currentBit++;
        }
```
- Envia os 8 bits do dado, um por vez, deslocando o bit correspondente para a posição menos significativa e aplicando um `AND` lógico.

##### Envio do Bit de Paridade
```ino
        else if (currentBit == 8) {
            digitalWrite(TX_PIN, parityBit);
            currentBit++;
        }
```
- Envia o bit de paridade, que é usado para detectar erros na transmissão.

##### Envio do Stop Bit
```ino
        else if (currentBit == 9) {
            digitalWrite(TX_PIN, HIGH);
            currentBit++;
        }
```
- O stop bit é sempre `HIGH` (nível lógico alto) e indica o fim da transmissão.

##### Finalização da Transmissão
```ino
        else {
            isSending = false;
            lastTransmitTime = currentTime;
        }
    }
}
```
- Finaliza a transmissão e atualiza o tempo da última transmissão.

---

### Função de Cálculo de Paridade
```ino
byte calcParity(byte data) {
    byte parity = 0;
    for (int i = 0; i < 8; i++) {
        parity ^= (data >> i) & 1;
    }
    return parity;
}
```
- Calcula a paridade ímpar do dado, aplicando a operação XOR em todos os bits.
- A paridade ímpar é 1 se o número de bits 1 no dado for ímpar, e 0 caso contrário.

---

## Conceitos Envolvidos

1. **Comunicação Serial**: Envia dados bit a bit em sequência, utilizando um único fio de transmissão.
2. **Start e Stop Bits**: Sinalizam o início e o fim de cada caractere transmitido.
3. **Paridade**: Técnica simples de detecção de erros, verificando se o número de bits 1 é par ou ímpar.
4. **Taxa de Baud (Baudrate)**: Define a velocidade de transmissão em bits por segundo.
5. **Microssegundos**: Unidade de tempo usada para controlar os intervalos de transmissão.

Com isso, o Transmitter envia o caractere 'A' repetidamente, garantindo que os dados sejam transmitidos de forma confiável e verificável.
