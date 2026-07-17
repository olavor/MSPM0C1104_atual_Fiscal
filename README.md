Este código implementa uma **Máquina de Estados Finita** (FSM) projetada par
a controlar o comportamento de um LED com base no tempo e na interação com um botão físico, 
rodando no microcontrolador MSPM0C1104.

Abaixo está a explicação detalhada de como cada parte do código funciona:

### 1. Definições de Tempo e Estados

* **`DELAY_1MS (32000)`:** O microcontrolador está configurado para rodar a um clock
* de 24 MHz. Isso significa que ele executa 24.000.000 ciclos por segundo. Para criar
* um atraso de exatamente 1 milissegundo (1ms), o código manda o processador gastar 32.000 ciclos.
* **`EstadoSistema`:** Cria uma estrutura (enumeração) que define os dois momentos
(estados) em que o programa pode estar:
1. `ESTADO_CONTAGEM_65S`: O LED está aceso e o sistema aguarda 65 segundos.
2. `ESTADO_APAGADO_130S`: O LED está apagado e o sistema aguarda 130 segundos.



### 2. Inicialização (`main`)

* **`SYSCFG_DL_init()`:** Aplica todas as configurações de hardware definidas
*  no arquivo SysConfig (como habilitar o clock e configurar os pinos como entrada/saída).
* **`DL_GPIO_setPins(...)`:** Garante que o sistema inicie com o `USER_LED_1` ligado.
* As variáveis `estadoAtual` (que inicia no estado de 65s) e `contadorTempoLED1`
* (que guarda o tempo percorrido) são inicializadas.

### 3. Loop Principal (`while(1)`) e Máquina de Estados

Dentro do loop infinito, o código verifica em qual estado está e toma decisões
baseado nisso. Toda vez que o loop termina, ele sofre um atraso de 1 milissegundo 
(`delay_cycles(DELAY_1MS)`). Isso significa que **1 iteração do loop = 1 milissegundo**.

Aqui está o que acontece em cada estado:

**Estado 1: `ESTADO_CONTAGEM_65S**`

* **Leitura do Botão:** O código lê o pino do botão (`USER_INPUT`).
* **Se o botão estiver pressionado:** A variável `contadorTempoLED1` é zerada.
*  Isso significa que, enquanto houver atividade no botão, o tempo de 65
*  segundos nunca será atingido.
* **Se o botão NÃO for pressionado:** A variável `contadorTempoLED1` é
* incrementada em 1 (ou seja, passou 1 milissegundo).


* **Verificação de Tempo:** Se o contador atingir **65.000** (o equivalente
*  a 65 segundos ininterruptos sem ninguém tocar no botão), o sistema:
1. Apaga o LED (`DL_GPIO_clearPins`).
2. Zera o contador para usá-lo na próxima etapa.
3. Muda o `estadoAtual` para o próximo estado (`ESTADO_APAGADO_130S`).



**Estado 2: `ESTADO_APAGADO_130S**`

* **Contagem Livre:** Neste estado, a leitura do botão não importa mais.
* O código simplesmente incrementa o `contadorTempoLED1` a cada milissegundo.
* **Verificação de Tempo:** Se o contador atingir **130.000** (o equivalente a 130 segundos), o sistema:
1. Liga o LED novamente (`DL_GPIO_setPins`).
2. Zera o contador de tempo.
3. Devolve o `estadoAtual` para `ESTADO_CONTAGEM_65S`, reiniciando todo o ciclo.
