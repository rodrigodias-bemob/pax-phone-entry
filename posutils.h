
/**
 * Utilitários para terminal POS PAX D230 - SDK Prolin 3.5
 */

#ifndef POSUTILS_H
#define POSUTILS_H

#ifdef __cplusplus
extern "C" {
#endif

// Estrutura XuiWindow (definição simplificada)
typedef struct {
    int handle;
    // Outros campos necessários para janelas XUI
} XuiWindow;

/**
 * Inicializa o sistema POS
 */
int SystemInit(void);

/**
 * Finaliza o aplicativo e retorna ao menu principal
 */
void SystemExit(void);

/**
 * Aguarda o número especificado de milissegundos
 * 
 * @param ms Tempo em milissegundos para aguardar
 */
void DelayMs(int ms);

#ifdef __cplusplus
}
#endif

#endif /* POSUTILS_H */
