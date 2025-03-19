guedera: Explique o que o projeto deve fazer, a lógica de funcionamento e como cada código .ino funciona!

GitHub Copilot: # Projeto de Comunicação UART entre Arduinos

Este projeto implementa uma comunicação serial UART (Universal Asynchronous Receiver-Transmitter) entre dois Arduinos usando pinos digitais genéricos, ao invés dos pinos TX/RX padrão. Vou explicar como funciona o sistema completo, o protocolo UART e detalhar o funcionamento de cada parte.

## Objetivo do Projeto

O objetivo é criar um sistema de comunicação serial entre dois Arduinos, onde:
- Um Arduino funciona como transmissor, enviando dados
- Outro Arduino funciona como receptor, recebendo e interpretando esses dados
- A comunicação segue o protocolo UART completo com detecção de erros

## Protocolo UART Implementado

O UART é um protocolo de comunicação serial que utiliza:

1. **Start bit**: Sinal LOW que indica o início da transmissão
2. **8 bits de dados**: Os bits de informação transmitidos sequencialmente (LSB primeiro)
3. **Bit de paridade**: Um bit adicional para detecção de erros (neste caso, paridade par)
4. **Stop bit**: Sinal HIGH que indica o fim da transmissão

A taxa de comunicação é configurada para 9600 baud, o que significa 9600 bits por segundo, resultando em aproximadamente 104 microssegundos por bit.

## Funcionamento do Arduino Transmissor (Transmitter.ino)

O transmissor executa as seguintes operações:

1. **Configuração inicial**:
   - Define o pino 7 como TX (saída)
   - Mantém a linha em HIGH quando ociosa (idle)
   - Configura a taxa de transmissão para 9600 baud

2. **Loop principal**:
   - Envia sequencialmente caracteres de 'A' a 'Z'
   - Entre cada transmissão, aguarda 1 segundo
   - Quando chega em 'Z', reinicia com 'A'

3. **Função de transmissão UART (uartTransmit)**:
   - Calcula o bit de paridade para o byte a ser enviado
   - Envia o start bit (LOW)
   - Envia os 8 bits de dados, começando pelo LSB
   - Envia o bit de paridade
   - Envia o stop bit (HIGH)
   - Usa temporizações precisas com `preciseMicrosDelay()` para manter o tempo correto entre bits

## Funcionamento do Arduino Receptor (Receiver.ino)

O receptor executa as seguintes operações:

1. **Configuração inicial**:
   - Define o pino 8 como RX (entrada)
   - Configura para a mesma taxa de recepção (9600 baud)

2. **Loop principal**:
   - Aguarda constantemente por dados recebidos
   - Quando um caractere é recebido, mostra no monitor serial
   - Reporta erros de paridade ou de framing, caso ocorram

3. **Função de recepção UART (uartReceive)**:
   - Espera pelo start bit (transição de HIGH para LOW)
   - Verifica se é realmente um start bit válido
   - Lê os 8 bits de dados no meio de cada período de bit
   - Lê o bit de paridade
   - Lê o stop bit
   - Verifica se há erros (paridade incorreta ou stop bit ausente)
   - Retorna o byte recebido e o status da recepção

## Detecção de Erros

O projeto implementa detecção de erros por:

1. **Paridade par**: O bit de paridade é calculado para que o número total de bits '1' (incluindo o bit de paridade) seja par. Qualquer falha é reportada como "ERRO: Paridade incorreta detectada!"

2. **Verificação de frame**: Confirma se o stop bit é recebido corretamente. Em caso de erro, reporta "ERRO: Stop bit incorreto detectado!"

## Temporização Precisa

Um elemento-chave do projeto é a função `preciseMicrosDelay()` que mantém a sincronização temporal:
- Usa `micros()` para medir o tempo com precisão
- Implementa um loop com instrução `NOP` (No Operation) para evitar otimizações do compilador
- Garante que cada bit tenha exatamente a duração necessária para manter os 9600 baud

Este projeto demonstra os princípios fundamentais da comunicação serial e como a camada física de um sistema de comunicação pode ser implementada usando recursos básicos de hardware digital.

guedera: Me de essa descrição que você acaba de fornecer em markdown

GitHub Copilot: # Projeto de Comunicação UART entre Arduinos

Este projeto implementa uma comunicação serial UART (Universal Asynchronous Receiver-Transmitter) entre dois Arduinos usando pinos digitais genéricos, ao invés dos pinos TX/RX padrão. Vou explicar como funciona o sistema completo, o protocolo UART e detalhar o funcionamento de cada parte.

## Objetivo do Projeto

O objetivo é criar um sistema de comunicação serial entre dois Arduinos, onde:
- Um Arduino funciona como transmissor, enviando dados
- Outro Arduino funciona como receptor, recebendo e interpretando esses dados
- A comunicação segue o protocolo UART completo com detecção de erros

## Protocolo UART Implementado

O UART é um protocolo de comunicação serial que utiliza:

1. **Start bit**: Sinal LOW que indica o início da transmissão
2. **8 bits de dados**: Os bits de informação transmitidos sequencialmente (LSB primeiro)
3. **Bit de paridade**: Um bit adicional para detecção de erros (neste caso, paridade par)
4. **Stop bit**: Sinal HIGH que indica o fim da transmissão

A taxa de comunicação é configurada para 9600 baud, o que significa 9600 bits por segundo, resultando em aproximadamente 104 microssegundos por bit.

## Funcionamento do Arduino Transmissor (Transmitter.ino)

O transmissor executa as seguintes operações:

1. **Configuração inicial**:
   - Define o pino 7 como TX (saída)
   - Mantém a linha em HIGH quando ociosa (idle)
   - Configura a taxa de transmissão para 9600 baud

2. **Loop principal**:
   - Envia sequencialmente caracteres de 'A' a 'Z'
   - Entre cada transmissão, aguarda 1 segundo
   - Quando chega em 'Z', reinicia com 'A'

3. **Função de transmissão UART (uartTransmit)**:
   - Calcula o bit de paridade para o byte a ser enviado
   - Envia o start bit (LOW)
   - Envia os 8 bits de dados, começando pelo LSB
   - Envia o bit de paridade
   - Envia o stop bit (HIGH)
   - Usa temporizações precisas com `preciseMicrosDelay()` para manter o tempo correto entre bits

## Funcionamento do Arduino Receptor (Receiver.ino)

O receptor executa as seguintes operações:

1. **Configuração inicial**:
   - Define o pino 8 como RX (entrada)
   - Configura para a mesma taxa de recepção (9600 baud)

2. **Loop principal**:
   - Aguarda constantemente por dados recebidos
   - Quando um caractere é recebido, mostra no monitor serial
   - Reporta erros de paridade ou de framing, caso ocorram

3. **Função de recepção UART (uartReceive)**:
   - Espera pelo start bit (transição de HIGH para LOW)
   - Verifica se é realmente um start bit válido
   - Lê os 8 bits de dados no meio de cada período de bit
   - Lê o bit de paridade
   - Lê o stop bit
   - Verifica se há erros (paridade incorreta ou stop bit ausente)
   - Retorna o byte recebido e o status da recepção

## Detecção de Erros

O projeto implementa detecção de erros por:

1. **Paridade par**: O bit de paridade é calculado para que o número total de bits '1' (incluindo o bit de paridade) seja par. Qualquer falha é reportada como "ERRO: Paridade incorreta detectada!"

2. **Verificação de frame**: Confirma se o stop bit é recebido corretamente. Em caso de erro, reporta "ERRO: Stop bit incorreto detectado!"

## Temporização Precisa

Um elemento-chave do projeto é a função `preciseMicrosDelay()` que mantém a sincronização temporal:
- Usa `micros()` para medir o tempo com precisão
- Implementa um loop com instrução `NOP` (No Operation) para evitar otimizações do compilador
- Garante que cada bit tenha exatamente a duração necessária para manter os 9600 baud

Este projeto demonstra os princípios fundamentais da comunicação serial e como a camada física de um sistema de comunicação pode ser implementada usando recursos básicos de hardware digital.