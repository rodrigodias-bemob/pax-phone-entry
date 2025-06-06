/**
 * Aplicativo de Recarga para Terminal POS PAX D230
 * SDK Prolin 3.5
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xui.h"
#include "posutils.h" // Incluímos o novo arquivo de cabeçalho

// Definições de tamanho da tela
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

// Estados da aplicação - Ordem ajustada para o novo fluxo
typedef enum {
    STATE_PHONE_INPUT,       // 1. Tela de Inserção de Telefone  
    STATE_CONFIRMATION,      // 2. Tela de Confirmação dos Dados
    STATE_PORTFOLIO,         // 3. Tela de Portfólio de Valores
    STATE_PROCESSING,        // 4. Tela de Processando Recarga
    STATE_SUCCESS,           // Tela de sucesso
    STATE_ERROR,             // Tela de erro
    STATE_AUTH               // Tela de autenticação (mantida por compatibilidade, mas não usada no novo fluxo)
} AppState;

// Valores de recarga disponíveis
const char* rechargeValues[] = {
    "R$15,00",
    "R$20,00",
    "R$30,00",
    "R$50,00",
    "R$100,00"
};

// Número de valores de recarga disponíveis
#define NUM_VALUES 5

// Variáveis globais
AppState currentState = STATE_PHONE_INPUT; // Começamos com a entrada do telefone agora
char phoneNumber[15] = "";
char pinCode[7] = "";
int selectedValueIndex = -1;
char carrierName[10] = "";

// Protótipos de funções
void DisplayPhoneInputScreen(void);     // 1. Tela de Inserção de Telefone
void DisplayConfirmationScreen(void);   // 2. Tela de Confirmação dos Dados
void DisplayPortfolioScreen(void);      // 3. Tela de Portfólio de Valores
void DisplayProcessingScreen(void);     // 4. Tela de Processando Recarga
void DisplaySuccessScreen(void);        // Tela de sucesso
void DisplayErrorScreen(const char* errorMsg); // Tela de erro
void DisplayAuthScreen(void);           // Tela de autenticação (mantida por compatibilidade)
void DetectCarrier(const char* phoneNum);
void ApplyPhoneMask(char* dest, const char* src);
int ValidatePin(const char* pin);

/**
 * Função principal
 */
int main(void) {
    // Inicialização do terminal
    SystemInit();
    
    // Inicializar interface gráfica
    XuiOpen();
    
    while (1) {
        // Máquina de estados com nova ordem
        switch (currentState) {
            case STATE_PHONE_INPUT:       // 1. Tela de Inserção de Telefone
                DisplayPhoneInputScreen();
                break;
                
            case STATE_CONFIRMATION:      // 2. Tela de Confirmação dos Dados
                DisplayConfirmationScreen();
                break;
                
            case STATE_PORTFOLIO:         // 3. Tela de Portfólio de Valores
                DisplayPortfolioScreen();
                break;
                
            case STATE_PROCESSING:        // 4. Tela de Processando Recarga
                DisplayProcessingScreen();
                break;
                
            case STATE_SUCCESS:
                DisplaySuccessScreen();
                break;
                
            case STATE_AUTH:              // Mantida por compatibilidade
                DisplayAuthScreen();
                break;
                
            case STATE_ERROR:
                DisplayErrorScreen("Ocorreu um erro");
                break;
                
            default:
                currentState = STATE_PHONE_INPUT;
                break;
        }
    }
    
    // Nunca deve chegar aqui
    XuiClose();
    return 0;
}

/**
 * Exibe a tela de entrada do número de telefone (Tela 1)
 */
void DisplayPhoneInputScreen(void) {
    int key;
    int cursorPos = 0;
    char displayPhone[15] = "";
    char numericPhone[12] = "";
    
    // Resetar o número de telefone
    memset(phoneNumber, 0, sizeof(phoneNumber));
    
    while (1) {
        // Limpar a tela
        XuiClearArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        
        // Exibir título
        XuiSetForeColor(XUI_COLOR_BLUE);
        XuiSetBgColor(XUI_COLOR_WHITE);
        XuiSetLineWidth(2);
        XuiDrawText(SCREEN_WIDTH/2, 20, "INFORME O NÚMERO", XUI_TEXT_CENTER);
        XuiSetLineWidth(1);
        
        // Aplicar máscara ao número de telefone
        ApplyPhoneMask(displayPhone, numericPhone);
        
        // Detectar operadora
        DetectCarrier(numericPhone);
        
        // Exibir campo de entrada
        XuiDrawRect(20, 70, SCREEN_WIDTH - 40, 40);
        XuiDrawText(SCREEN_WIDTH/2, 90, displayPhone, XUI_TEXT_CENTER);
        
        // Exibir operadora detectada
        if (strlen(carrierName) > 0) {
            XuiSetForeColor(XUI_COLOR_BLUE);
            XuiDrawText(SCREEN_WIDTH/2, 130, carrierName, XUI_TEXT_CENTER);
        }
        
        // Exibir botão verde com texto "Continuar"
        XuiSetForeColor(XUI_COLOR_WHITE);
        XuiSetBgColor(XUI_COLOR_GREEN);
        XuiDrawRect(70, 180, 100, 40);
        XuiDrawText(120, 200, "Continuar", XUI_TEXT_CENTER);
        
        // Restaurar cores
        XuiSetForeColor(XUI_COLOR_BLACK);
        XuiSetBgColor(XUI_COLOR_WHITE);
        
        // Exibir instruções
        XuiDrawText(10, SCREEN_HEIGHT - 30, "[0-9]: Digitar", XUI_TEXT_LEFT);
        XuiDrawText(SCREEN_WIDTH - 10, SCREEN_HEIGHT - 30, "[VERDE]: Continuar", XUI_TEXT_RIGHT);
        
        // Capturar tecla
        key = XuiGetKey();
        
        if (key >= '0' && key <= '9') {
            // Permitir apenas 11 dígitos (DDD + número)
            if (cursorPos < 11) {
                numericPhone[cursorPos++] = (char)key;
                numericPhone[cursorPos] = '\0';
            }
        } else if (key == XUI_KEY_CLEAR) {
            // Apagar último dígito
            if (cursorPos > 0) {
                numericPhone[--cursorPos] = '\0';
            }
        } else if (key == XUI_KEY_CANCEL) {
            // Sair do aplicativo ao pressionar CANCELAR na tela inicial
            XuiClose();
            SystemExit();
            return;
        } else if (key == XUI_KEY_GREEN || key == XUI_KEY_ENTER) {
            // Avançar para a tela de confirmação se o número tiver pelo menos 10 dígitos
            if (cursorPos >= 10) {
                // Guardar o número completo
                strcpy(phoneNumber, numericPhone);
                currentState = STATE_CONFIRMATION;
                return;
            } else {
                // Mostrar erro se o número não estiver completo
                XuiSetForeColor(XUI_COLOR_RED);
                XuiDrawText(SCREEN_WIDTH/2, 160, "Número incompleto!", XUI_TEXT_CENTER);
                XuiDrawText(SCREEN_WIDTH/2, 180, "Digite DDD + número", XUI_TEXT_CENTER);
                XuiRefresh(); // Atualizar a tela para mostrar o erro
                DelayMs(2000); // Mostrar mensagem por 2 segundos
            }
        }
    }
}

/**
 * Exibe a tela de confirmação do número e operadora (Tela 2)
 */
void DisplayConfirmationScreen(void) {
    int key;
    char maskedPhone[15];
    
    // Aplicar máscara ao número de telefone
    ApplyPhoneMask(maskedPhone, phoneNumber);
    
    // Detectar operadora novamente para garantir
    DetectCarrier(phoneNumber);
    
    while (1) {
        // Limpar a tela
        XuiClearArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        
        // Exibir título
        XuiSetForeColor(XUI_COLOR_BLUE);
        XuiSetBgColor(XUI_COLOR_WHITE);
        XuiSetLineWidth(2);
        XuiDrawText(SCREEN_WIDTH/2, 20, "CONFIRME OS DADOS", XUI_TEXT_CENTER);
        XuiSetLineWidth(1);
        
        // Exibir caixa de confirmação
        XuiSetForeColor(XUI_COLOR_BLACK);
        XuiDrawRect(10, 70, SCREEN_WIDTH - 20, 150);
        
        // Exibir informações para confirmação
        XuiDrawText(30, 90, "Número:", XUI_TEXT_LEFT);
        XuiSetForeColor(XUI_COLOR_BLUE);
        XuiDrawText(SCREEN_WIDTH - 30, 90, maskedPhone, XUI_TEXT_RIGHT);
        
        XuiSetForeColor(XUI_COLOR_BLACK);
        XuiDrawText(30, 120, "Operadora:", XUI_TEXT_LEFT);
        XuiSetForeColor(XUI_COLOR_BLUE);
        XuiDrawText(SCREEN_WIDTH - 30, 120, carrierName, XUI_TEXT_RIGHT);
        
        // Exibir pergunta de confirmação
        XuiSetForeColor(XUI_COLOR_BLACK);
        XuiDrawText(SCREEN_WIDTH/2, 190, "Seus dados estão corretos?", XUI_TEXT_CENTER);
        
        // Exibir botões verde e vermelho com texto
        // Botão verde
        XuiSetForeColor(XUI_COLOR_WHITE);
        XuiSetBgColor(XUI_COLOR_GREEN);
        XuiDrawRect(120, 220, 110, 40);
        XuiDrawText(175, 240, "Sim, continuar", XUI_TEXT_CENTER);
        
        // Botão vermelho
        XuiSetForeColor(XUI_COLOR_WHITE);
        XuiSetBgColor(XUI_COLOR_RED);
        XuiDrawRect(10, 220, 100, 40);
        XuiDrawText(60, 240, "Não, corrigir", XUI_TEXT_CENTER);
        
        // Restaurar cores para as instruções
        XuiSetForeColor(XUI_COLOR_BLACK);
        XuiSetBgColor(XUI_COLOR_WHITE);
        
        // Exibir instruções
        XuiDrawText(10, SCREEN_HEIGHT - 30, "[VERMELHO]: Corrigir", XUI_TEXT_LEFT);
        XuiDrawText(SCREEN_WIDTH - 10, SCREEN_HEIGHT - 30, "[VERDE]: Continuar", XUI_TEXT_RIGHT);
        
        // Capturar tecla
        key = XuiGetKey();
        
        if (key == XUI_KEY_RED || key == XUI_KEY_CANCEL) {
            // Voltar para a tela de entrada de telefone
            currentState = STATE_PHONE_INPUT;
            return;
        } else if (key == XUI_KEY_GREEN || key == XUI_KEY_ENTER) {
            // Avançar para a tela de portfólio de valores
            currentState = STATE_PORTFOLIO;
            return;
        }
    }
}

/**
 * Exibe a tela de portfólio com os valores disponíveis para recarga (Tela 3)
 */
void DisplayPortfolioScreen(void) {
    int i;
    int key;
    int selectedOption = 0;
    
    while (1) {
        // Limpar a tela
        XuiClearArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        
        // Exibir título
        XuiSetForeColor(XUI_COLOR_BLUE);
        XuiSetBgColor(XUI_COLOR_WHITE);
        XuiSetLineWidth(2);
        XuiDrawText(SCREEN_WIDTH/2, 20, "ESCOLHA O VALOR", XUI_TEXT_CENTER);
        XuiSetLineWidth(1);
        
        // Exibir instruções
        XuiSetForeColor(XUI_COLOR_BLACK);
        XuiDrawText(SCREEN_WIDTH/2, 45, "Selecione um valor:", XUI_TEXT_CENTER);
        
        // Exibir valores disponíveis como cards
        for (i = 0; i < NUM_VALUES; i++) {
            // Destacar a opção selecionada
            if (i == selectedOption) {
                XuiSetForeColor(XUI_COLOR_WHITE);
                XuiSetBgColor(XUI_COLOR_BLUE);
                XuiDrawRect(10, 70 + (i * 40), SCREEN_WIDTH - 20, 35);
            } else {
                XuiSetForeColor(XUI_COLOR_BLACK);
                XuiSetBgColor(XUI_COLOR_WHITE);
                XuiDrawRect(10, 70 + (i * 40), SCREEN_WIDTH - 20, 35);
            }
            
            // Exibir o valor
            XuiDrawText(SCREEN_WIDTH/2, 70 + (i * 40) + 18, rechargeValues[i], XUI_TEXT_CENTER);
        }
        
        // Exibir instruções de navegação
        XuiSetForeColor(XUI_COLOR_BLACK);
        XuiSetBgColor(XUI_COLOR_WHITE);
        XuiDrawText(10, SCREEN_HEIGHT - 30, "[↑][↓]: Navegar", XUI_TEXT_LEFT);
        XuiDrawText(SCREEN_WIDTH - 10, SCREEN_HEIGHT - 30, "[ENTER]: Selecionar", XUI_TEXT_RIGHT);
        
        // Capturar tecla
        key = XuiGetKey();
        
        switch (key) {
            case XUI_KEY_UP:
                if (selectedOption > 0) selectedOption--;
                break;
                
            case XUI_KEY_DOWN:
                if (selectedOption < NUM_VALUES - 1) selectedOption++;
                break;
                
            case XUI_KEY_ENTER:
                // Salvar o valor selecionado e ir para a tela de processamento
                selectedValueIndex = selectedOption;
                currentState = STATE_PROCESSING;
                return;
                
            case XUI_KEY_CANCEL:
                // Voltar para a tela de confirmação
                currentState = STATE_CONFIRMATION;
                return;
                
            default:
                break;
        }
    }
}

/**
 * Aplica a máscara (xx)xxxxx-xxxx ao número de telefone
 */
void ApplyPhoneMask(char* dest, const char* src) {
    int len = strlen(src);
    memset(dest, 0, 15);
    
    if (len == 0) return;
    
    // DDD
    if (len >= 2) {
        dest[0] = '(';
        dest[1] = src[0];
        dest[2] = src[1];
        dest[3] = ')';
    } else if (len == 1) {
        dest[0] = '(';
        dest[1] = src[0];
        return;
    }
    
    // Primeiros 5 dígitos
    for (int i = 2; i < 7 && i < len; i++) {
        dest[i + 2] = src[i];
    }
    
    // Hífen e últimos dígitos
    if (len > 7) {
        dest[9] = '-';
        for (int i = 7; i < len; i++) {
            dest[i + 3] = src[i];
        }
    }
}

/**
 * Detecta a operadora com base no número de telefone
 */
void DetectCarrier(const char* phoneNum) {
    // Precisamos de pelo menos o DDD e o primeiro dígito após o DDD
    if (strlen(phoneNum) < 3) {
        strcpy(carrierName, "");
        return;
    }
    
    // Pegar o primeiro dígito após o DDD
    char firstDigit = phoneNum[2];
    
    // Prefixos de operadoras brasileiras
    switch (firstDigit) {
        case '6':
        case '8':
        case '9':
            strcpy(carrierName, "VIVO");
            break;
        case '7':
            strcpy(carrierName, "OI");
            break;
        case '2':
        case '3':
        case '4':
            strcpy(carrierName, "CLARO");
            break;
        case '5':
            strcpy(carrierName, "TIM");
            break;
        default:
            strcpy(carrierName, "");
            break;
    }
}

/**
 * Exibe a tela de processamento (Tela 4)
 */
void DisplayProcessingScreen(void) {
    int i;
    char maskedPhone[15];
    
    // Aplicar máscara ao número de telefone
    ApplyPhoneMask(maskedPhone, phoneNumber);
    
    // Limpar a tela
    XuiClearArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // Exibir título
    XuiSetForeColor(XUI_COLOR_BLUE);
    XuiSetBgColor(XUI_COLOR_WHITE);
    XuiSetLineWidth(2);
    XuiDrawText(SCREEN_WIDTH/2, 20, "PROCESSANDO", XUI_TEXT_CENTER);
    XuiSetLineWidth(1);
    
    // Exibir ícone de carregamento (círculo)
    XuiSetForeColor(XUI_COLOR_GREEN);
    XuiDrawCircle(SCREEN_WIDTH/2, 80, 30, XUI_COLOR_GREEN);
    
    // Exibir mensagem de processamento
    XuiSetForeColor(XUI_COLOR_BLACK);
    XuiDrawText(SCREEN_WIDTH/2, 140, "Estamos processando sua recarga,", XUI_TEXT_CENTER);
    XuiDrawText(SCREEN_WIDTH/2, 160, "aguarde...", XUI_TEXT_CENTER);
    
    // Exibir informações da recarga
    XuiDrawText(20, 200, "Operadora:", XUI_TEXT_LEFT);
    XuiSetForeColor(XUI_COLOR_BLUE);
    XuiDrawText(SCREEN_WIDTH - 20, 200, carrierName, XUI_TEXT_RIGHT);
    
    XuiSetForeColor(XUI_COLOR_BLACK);
    XuiDrawText(20, 230, "Número:", XUI_TEXT_LEFT);
    XuiDrawText(SCREEN_WIDTH - 20, 230, maskedPhone, XUI_TEXT_RIGHT);
    
    XuiDrawText(20, 260, "Valor:", XUI_TEXT_LEFT);
    XuiDrawText(SCREEN_WIDTH - 20, 260, rechargeValues[selectedValueIndex], XUI_TEXT_RIGHT);
    
    // Exibir animação de processamento
    for (i = 0; i < 3; i++) {
        XuiSetForeColor(XUI_COLOR_GREEN);
        XuiDrawCircle(SCREEN_WIDTH/2, 80, 30, XUI_COLOR_GREEN);
        XuiRefresh();
        DelayMs(300);
        
        XuiSetForeColor(XUI_COLOR_LIGHTGRAY);
        XuiDrawCircle(SCREEN_WIDTH/2, 80, 30, XUI_COLOR_LIGHTGRAY);
        XuiRefresh();
        DelayMs(300);
    }
    
    // Simular processamento
    DelayMs(1000);
    
    // Ir para a tela de sucesso (em um caso real, verificaria a resposta da transação)
    currentState = STATE_SUCCESS;
}

/**
 * Exibe a tela de sucesso
 */
void DisplaySuccessScreen(void) {
    int key;
    char maskedPhone[15];
    
    // Aplicar máscara ao número de telefone
    ApplyPhoneMask(maskedPhone, phoneNumber);
    
    while (1) {
        // Limpar a tela
        XuiClearArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        
        // Exibir título com cor verde
        XuiSetForeColor(XUI_COLOR_GREEN);
        XuiSetBgColor(XUI_COLOR_WHITE);
        XuiSetLineWidth(2);
        XuiDrawText(SCREEN_WIDTH/2, 20, "RECARGA CONCLUÍDA", XUI_TEXT_CENTER);
        XuiSetLineWidth(1);
        
        // Desenhar círculo verde com símbolo de check
        XuiDrawCircle(SCREEN_WIDTH/2, 80, 30, XUI_COLOR_GREEN);
        
        // Desenhar check dentro do círculo
        XuiSetForeColor(XUI_COLOR_WHITE);
        XuiSetLineWidth(3);
        XuiDrawLine(SCREEN_WIDTH/2 - 15, 80, SCREEN_WIDTH/2 - 5, 90);
        XuiDrawLine(SCREEN_WIDTH/2 - 5, 90, SCREEN_WIDTH/2 + 15, 70);
        XuiSetLineWidth(1);
        
        // Exibir mensagem de sucesso
        XuiSetForeColor(XUI_COLOR_GREEN);
        XuiDrawText(SCREEN_WIDTH/2, 130, "Recarga concluída com sucesso.", XUI_TEXT_CENTER);
        
        // Exibir detalhes da transação
        XuiSetForeColor(XUI_COLOR_BLACK);
        XuiDrawText(20, 170, "Operadora:", XUI_TEXT_LEFT);
        XuiDrawText(SCREEN_WIDTH - 20, 170, carrierName, XUI_TEXT_RIGHT);
        
        XuiDrawText(20, 200, "Número:", XUI_TEXT_LEFT);
        XuiDrawText(SCREEN_WIDTH - 20, 200, maskedPhone, XUI_TEXT_RIGHT);
        
        XuiDrawText(20, 230, "Valor:", XUI_TEXT_LEFT);
        XuiDrawText(SCREEN_WIDTH - 20, 230, rechargeValues[selectedValueIndex], XUI_TEXT_RIGHT);
        
        // Exibir instruções
        XuiDrawText(SCREEN_WIDTH/2, SCREEN_HEIGHT - 30, "[QUALQUER TECLA]: Nova recarga", XUI_TEXT_CENTER);
        
        // Capturar tecla
        key = XuiGetKey();
        
        // Qualquer tecla retorna à tela inicial
        currentState = STATE_PORTFOLIO;
        return;
    }
}

/**
 * Exibe uma tela de erro
 */
void DisplayErrorScreen(const char* errorMsg) {
    int key;
    
    while (1) {
        // Limpar a tela
        XuiClearArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        
        // Exibir título com cor vermelha
        XuiSetForeColor(XUI_COLOR_RED);
        XuiSetBgColor(XUI_COLOR_WHITE);
        XuiSetLineWidth(2);
        XuiDrawText(SCREEN_WIDTH/2, 20, "ERRO", XUI_TEXT_CENTER);
        XuiSetLineWidth(1);
        
        // Desenhar círculo vermelho com X
        XuiDrawCircle(SCREEN_WIDTH/2, 80, 30, XUI_COLOR_RED);
        
        // Desenhar X dentro do círculo
        XuiSetForeColor(XUI_COLOR_WHITE);
        XuiSetLineWidth(3);
        XuiDrawLine(SCREEN_WIDTH/2 - 15, 65, SCREEN_WIDTH/2 + 15, 95);
        XuiDrawLine(SCREEN_WIDTH/2 + 15, 65, SCREEN_WIDTH/2 - 15, 95);
        XuiSetLineWidth(1);
        
        // Exibir mensagem de erro
        XuiSetForeColor(XUI_COLOR_RED);
        XuiDrawText(SCREEN_WIDTH/2, 130, errorMsg, XUI_TEXT_CENTER);
        
        // Exibir instruções
        XuiSetForeColor(XUI_COLOR_BLACK);
        XuiDrawText(SCREEN_WIDTH/2, SCREEN_HEIGHT - 30, "[QUALQUER TECLA]: Continuar", XUI_TEXT_CENTER);
        
        // Capturar tecla
        key = XuiGetKey();
        
        // Retornar à tela inicial
        currentState = STATE_PORTFOLIO;
        return;
    }
}

/**
 * Exibe a tela de autenticação (entrada de PIN)
 * Mantida por compatibilidade, não usada no novo fluxo
 */
void DisplayAuthScreen(void) {
    int key;
    int cursorPos = 0;
    char displayPin[7] = "";
    
    // Resetar o PIN
    memset(pinCode, 0, sizeof(pinCode));
    
    while (1) {
        // Limpar a tela
        XuiClearArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        
        // Exibir título
        XuiSetForeColor(XUI_COLOR_BLUE);
        XuiSetBgColor(XUI_COLOR_WHITE);
        XuiSetLineWidth(2);
        XuiDrawText(SCREEN_WIDTH/2, 20, "AUTENTICAÇÃO", XUI_TEXT_CENTER);
        XuiSetLineWidth(1);
        
        // Exibir instruções
        XuiSetForeColor(XUI_COLOR_BLACK);
        XuiDrawText(SCREEN_WIDTH/2, 60, "Insira a senha do vendedor:", XUI_TEXT_CENTER);
        
        // Exibir campo de entrada
        XuiDrawRect(70, 90, 100, 40);
        
        // Exibir asteriscos para cada dígito
        memset(displayPin, 0, sizeof(displayPin));
        for (int i = 0; i < cursorPos; i++) {
            displayPin[i] = '*';
        }
        displayPin[cursorPos] = '\0';
        
        XuiDrawText(SCREEN_WIDTH/2, 110, displayPin, XUI_TEXT_CENTER);
        
        // Exibir instruções
        XuiDrawText(10, SCREEN_HEIGHT - 60, "[0-9]: Digitar", XUI_TEXT_LEFT);
        XuiDrawText(10, SCREEN_HEIGHT - 30, "[VERMELHO]: Voltar", XUI_TEXT_LEFT);
        XuiDrawText(SCREEN_WIDTH - 10, SCREEN_HEIGHT - 30, "[VERDE]: Confirmar", XUI_TEXT_RIGHT);
        
        // Capturar tecla
        key = XuiGetKey();
        
        if (key >= '0' && key <= '9') {
            // Permitir apenas 6 dígitos para o PIN
            if (cursorPos < 6) {
                pinCode[cursorPos++] = (char)key;
                pinCode[cursorPos] = '\0';
            }
        } else if (key == XUI_KEY_CLEAR) {
            // Apagar último dígito
            if (cursorPos > 0) {
                pinCode[--cursorPos] = '\0';
            }
        } else if (key == XUI_KEY_RED || key == XUI_KEY_CANCEL) {
            // Voltar para a tela de entrada de telefone
            currentState = STATE_PHONE_INPUT;
            return;
        } else if (key == XUI_KEY_GREEN || key == XUI_KEY_ENTER) {
            // Verificar PIN - pelo menos 4 dígitos
            if (cursorPos >= 4) {
                if (ValidatePin(pinCode)) {
                    currentState = STATE_PROCESSING;
                    return;
                } else {
                    // Mostrar erro de PIN inválido
                    XuiSetForeColor(XUI_COLOR_RED);
                    XuiDrawText(SCREEN_WIDTH/2, 160, "PIN inválido!", XUI_TEXT_CENTER);
                    XuiDrawText(SCREEN_WIDTH/2, 180, "Tente novamente", XUI_TEXT_CENTER);
                    XuiRefresh(); // Atualizar a tela para mostrar o erro
                    DelayMs(2000); // Mostrar mensagem por 2 segundos
                    
                    // Limpar o PIN para nova tentativa
                    cursorPos = 0;
                    memset(pinCode, 0, sizeof(pinCode));
                }
            } else {
                // Mostrar erro se o PIN tiver menos de 4 dígitos
                XuiSetForeColor(XUI_COLOR_RED);
                XuiDrawText(SCREEN_WIDTH/2, 160, "PIN muito curto!", XUI_TEXT_CENTER);
                XuiDrawText(SCREEN_WIDTH/2, 180, "Mínimo 4 dígitos", XUI_TEXT_CENTER);
                XuiRefresh(); // Atualizar a tela para mostrar o erro
                DelayMs(2000); // Mostrar mensagem por 2 segundos
            }
        }
    }
}

/**
 * Valida o PIN inserido
 * Retorna 1 se válido, 0 se inválido
 */
int ValidatePin(const char* pin) {
    // PIN fixo para teste: "1234"
    // Em produção, deveria validar contra um backend
    return strcmp(pin, "1234") == 0;
}
