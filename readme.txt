
# Aplicativo de Recarga para Terminal POS PAX D230

## Visão Geral

Este é um aplicativo de recarga de celular desenvolvido para o terminal POS PAX D230 utilizando o SDK Prolin 3.5 em C ANSI. O aplicativo permite ao usuário selecionar um valor de recarga, inserir o número de telefone do cliente, identificar a operadora automaticamente, autenticar-se como vendedor e processar a recarga.

## Características

- Interface gráfica intuitiva para o operador do terminal
- Navegação utilizando as teclas de função do terminal (vermelho para voltar, verde para avançar)
- Seleção de valor de recarga a partir de um portfólio
- Entrada de número de telefone com máscara automática (xx)xxxxx-xxxx
- Detecção automática da operadora com base no número
- Autenticação do vendedor via PIN
- Simulação de processamento de recarga
- Tela de confirmação de sucesso

## Requisitos Técnicos

- Terminal PAX D230 ou compatível
- Sistema operacional Prolin 3.5
- SDK Prolin com suporte à biblioteca XUI

## Compilação

Para compilar o aplicativo, é necessário ter o ambiente de desenvolvimento do SDK Prolin configurado. O comando básico de compilação é:

```
poscc main.c -o recarga
```

## Fluxo do Aplicativo

1. Tela de portfólio: Seleção do valor de recarga
2. Tela de entrada de telefone: Digitação do número com detecção automática da operadora
3. Tela de autenticação: Inserção do PIN do vendedor (PIN de teste: 1234)
4. Tela de processamento: Simulação do processamento da recarga
5. Tela de sucesso: Confirmação visual da recarga realizada

## Notas de Desenvolvimento

- Este aplicativo foi desenvolvido seguindo as práticas recomendadas para o terminal PAX D230
- A interface utiliza a biblioteca XUI para desenho na tela
- O código segue o padrão C ANSI para máxima compatibilidade com o SDK Prolin
- A função de validação de PIN está configurada para aceitar "1234" como senha válida

## Limitações

- Este é um aplicativo de demonstração e não realiza recargas reais
- Em um ambiente de produção, seria necessário integrar com APIs de recargas de operadoras
