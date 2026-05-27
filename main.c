#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"
#include "pilha.h"

#define MAX_EXPRESSOES 100

int main() {
    // Vetores para guardar os dados e exibir tudo no final
    char expressoesSalvas[MAX_EXPRESSOES][500];
    struct No* raizesSalvas[MAX_EXPRESSOES];
    double resultadosSalvos[MAX_EXPRESSOES];
    int totalExp = 0;

    char entrada[500];

    printf("=========================================\n");
    printf("               CALCULADORA\n");
    printf("=========================================\n\n");

    // 1. Loop de leitura das expressoes
    while (totalExp < MAX_EXPRESSOES) {
        printf("Digite a expressao aritmetica (ou 'sair' para ver o resultado):\n> ");
        
        // Le a linha digitada pelo usuario
        fgets(entrada, sizeof(entrada), stdin);
        
        // Remove o '\n' do final que o fgets coloca
        entrada[strcspn(entrada, "\n")] = 0;

        // Criterio de parada
        if (strcmp(entrada, "sair") == 0) {
            break;
        }

        // Ignora entradas vazias
        if (strlen(entrada) == 0) {
            continue;
        }

        // Salva a expressao original digitada
        strcpy(expressoesSalvas[totalExp], entrada);

        // Gera a arvore para esta expressao e salva a raiz
        raizesSalvas[totalExp] = CriarArvore(entrada);

        // Calcula o resultado e salva
        resultadosSalvos[totalExp] = calcArvore(raizesSalvas[totalExp]);

        totalExp++;
    }

    // Presentacao dos resultados no final 
    printf("\n=========================================\n");
    printf("         RESULTADO DAS EXPRESSOES         \n");
    printf("=========================================\n");

    for (int i = 0; i < totalExp; i++) {
        printf("\nExpressao %d de entrada: %s\n", i + 1, expressoesSalvas[i]);
        
        printf("Arvore Gerada (Infixa): ");
        imprimirArvore(raizesSalvas[i]);
        printf("\n");

        printf("Resultado: %.2f\n", resultadosSalvos[i]);
        printf("-----------------------------------------\n");
    }

    // Limpeza da estrutura das árvores
    for (int i = 0; i < totalExp; i++) {
        liberarArvore(raizesSalvas[i]); 
    }

    printf("\nPrograma encerrado com sucesso!\n");
    return 0;
}