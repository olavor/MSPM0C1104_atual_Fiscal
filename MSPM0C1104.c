#include "ti_msp_dl_config.h"

// Definição de ciclos para atraso de 1ms baseado no clock padrão de 32MHz
// 24000 ciclos @ 24MHz = ~1ms
#define DELAY_1MS (24000)

// Definição dos estados do sistema para o LED 1
typedef enum {
    ESTADO_CONTAGEM_65S,
    ESTADO_APAGADO_130S
} EstadoSistema;

int main(void){
    SYSCFG_DL_init();
    
    // O LED 1 começa SEMPRE aceso
    DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);

    EstadoSistema estadoAtual = ESTADO_CONTAGEM_65S;
    uint32_t contadorTempoLED1 = 0;

    while (1) {
        
        // --- MÁQUINA DE ESTADOS (LED 1 E BOTÃO USER_INPUT) ---
        switch (estadoAtual) {
            
            case ESTADO_CONTAGEM_65S:
                // LÓGICA DO BOTÃO E CONTAGEM DO LED 1
                // Se o botão (USER_INPUT) for pressionado, reinicia o temporizador
                if ((DL_GPIO_readPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_INPUT_PIN) & GPIO_LEDS_USER_INPUT_PIN) != 0) {
                    contadorTempoLED1 = 0; 
                } else {
                    contadorTempoLED1++; // Incrementa se o botão NÃO for pressionado
                }

                // Se atingir 65.000 iterações (65 segundos) sem interrupção do botão
                if (contadorTempoLED1 >= 60000) {
                    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN); // Apaga o LED 1
                    
                    contadorTempoLED1 = 0;             // Reseta o contador principal
                    estadoAtual = ESTADO_APAGADO_130S; // Transiciona o estado
                }
                break;

            case ESTADO_APAGADO_130S:
                contadorTempoLED1++; // Incrementa o tempo que o LED 1 passa apagado

                // Se atingir 130.000 iterações (130 segundos)
                if (contadorTempoLED1 >= 120000) {
                    DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN); // Liga o LED 1 novamente
                    
                    contadorTempoLED1 = 0;              // Reseta o contador
                    estadoAtual = ESTADO_CONTAGEM_65S;  // Retorna para o estado inicial
                }
                break;
        }

        // Base de tempo: espera 1 milissegundo antes de rodar o loop novamente
        delay_cycles(DELAY_1MS);
    }
}