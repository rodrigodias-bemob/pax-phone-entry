
/**
 * Header para API XUI do SDK Prolin
 * Este arquivo define as funções e constantes para trabalhar com a interface gráfica do terminal
 */

#ifndef XUI_H
#define XUI_H

#ifdef __cplusplus
extern "C" {
#endif

// Cores padrão
#define XUI_COLOR_BLACK     0x000000
#define XUI_COLOR_WHITE     0xFFFFFF
#define XUI_COLOR_RED       0xFF0000
#define XUI_COLOR_GREEN     0x00FF00
#define XUI_COLOR_BLUE      0x0000FF
#define XUI_COLOR_YELLOW    0xFFFF00
#define XUI_COLOR_CYAN      0x00FFFF
#define XUI_COLOR_MAGENTA   0xFF00FF
#define XUI_COLOR_GRAY      0x808080
#define XUI_COLOR_LIGHTGRAY 0xC0C0C0
#define XUI_COLOR_DARKGRAY  0x404040

// Teclas especiais
#define XUI_KEY_ENTER       0x000D
#define XUI_KEY_CANCEL      0x001B
#define XUI_KEY_CLEAR       0x0008
#define XUI_KEY_UP          0x0101
#define XUI_KEY_DOWN        0x0102
#define XUI_KEY_LEFT        0x0103
#define XUI_KEY_RIGHT       0x0104
#define XUI_KEY_RED         0x0201
#define XUI_KEY_GREEN       0x0202
#define XUI_KEY_YELLOW      0x0203
#define XUI_KEY_BLUE        0x0204
#define XUI_KEY_F1          0x0301
#define XUI_KEY_F2          0x0302
#define XUI_KEY_F3          0x0303
#define XUI_KEY_F4          0x0304

// Alinhamento de texto
#define XUI_TEXT_LEFT       0
#define XUI_TEXT_CENTER     1
#define XUI_TEXT_RIGHT      2

/**
 * Inicializa a interface XUI
 * 
 * @return 0 se sucesso, valor negativo em caso de erro
 */
int XuiOpen(void);

/**
 * Finaliza a interface XUI
 */
void XuiClose(void);

/**
 * Limpa uma área específica da tela
 * 
 * @param x Posição X inicial
 * @param y Posição Y inicial
 * @param width Largura da área a ser limpa
 * @param height Altura da área a ser limpa
 */
void XuiClearArea(int x, int y, int width, int height);

/**
 * Define a cor de primeiro plano para desenho
 * 
 * @param color Código RGB da cor
 */
void XuiSetForeColor(int color);

/**
 * Define a cor de fundo para desenho
 * 
 * @param color Código RGB da cor
 */
void XuiSetBgColor(int color);

/**
 * Define a espessura das linhas para desenho
 * 
 * @param width Espessura da linha em pixels
 */
void XuiSetLineWidth(int width);

/**
 * Desenha texto na tela
 * 
 * @param x Posição X do texto
 * @param y Posição Y do texto
 * @param text Texto a ser exibido
 * @param alignment Alinhamento do texto (XUI_TEXT_LEFT, XUI_TEXT_CENTER, XUI_TEXT_RIGHT)
 */
void XuiDrawText(int x, int y, const char* text, int alignment);

/**
 * Desenha um retângulo na tela
 * 
 * @param x Posição X inicial
 * @param y Posição Y inicial
 * @param width Largura do retângulo
 * @param height Altura do retângulo
 */
void XuiDrawRect(int x, int y, int width, int height);

/**
 * Desenha um círculo na tela
 * 
 * @param x Posição X do centro
 * @param y Posição Y do centro
 * @param radius Raio do círculo em pixels
 * @param color Cor do círculo
 */
void XuiDrawCircle(int x, int y, int radius, int color);

/**
 * Desenha uma linha na tela
 * 
 * @param x1 Posição X inicial
 * @param y1 Posição Y inicial
 * @param x2 Posição X final
 * @param y2 Posição Y final
 */
void XuiDrawLine(int x1, int y1, int x2, int y2);

/**
 * Espera por um pressionamento de tecla e retorna o código da tecla
 * 
 * @return Código da tecla pressionada
 */
int XuiGetKey(void);

/**
 * Atualiza a tela com as mudanças realizadas
 */
void XuiRefresh(void);

#ifdef __cplusplus
}
#endif

#endif /* XUI_H */
