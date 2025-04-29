
/**
 * Implementação de utilitários para terminal POS PAX D230 - SDK Prolin 3.5
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "posutils.h"

/**
 * Inicializa o sistema POS
 */
int SystemInit(void) {
    // Implementação para inicializar o terminal POS
    // Esta é uma implementação simplificada para o SDK Prolin 3.5
    return 0;
}

/**
 * Finaliza o aplicativo e retorna ao menu principal
 */
void SystemExit(void) {
    // Implementação para encerrar o aplicativo
    exit(0);
}

/**
 * Aguarda o número especificado de milissegundos
 */
void DelayMs(int ms) {
    // Implementação usando usleep (microssegundos)
    usleep(ms * 1000);
}
