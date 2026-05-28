#include "arvore.h"
#include "pilha.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>

// Criar novo no na arvore
No* CriarNo(char* valor) {
    // Alocar memória para o no
    No* novoNo = (No*)malloc(sizeof(No));
    // Se houver falha na alocação retorna erro
    if (novoNo == NULL) {
        printf("Erro ao alocar memória para o nó da árvore!\n");
        exit(1);
    }
    // Copia o token de forma segura para o vetor do nó
    strncpy(novoNo->valor, valor, sizeof(novoNo->valor) - 1);
    // Garante que o ultimo caractere seja o \0
    novoNo->valor[sizeof(novoNo->valor) - 1] = '\0';
    // Coloca valores nulos para os nós filhos
    novoNo->esquerdo = NULL;
    novoNo->direito = NULL;

    return novoNo;
}

// Função para desempilhar operadores e montar as subarvores
void processarOperador(Pilha* pilhaNos, Pilha* pilhaOperadores) {
    // Desempilha operador
    struct No* operadorNo = pop(pilhaOperadores);
    if (operadorNo == NULL) return;
    
    // Se o operador for unário pega o primeiro operando da pilha
    if (operadorUnario(operadorNo->valor)) {
        // Operando fica a direita
        operadorNo->direito = pop(pilhaNos);
        operadorNo->esquerdo = NULL;
    }
    // Se for binario pega dois operandos, o topo da pilha é o operador da direita e o da esquerda o primeiro
    else {
        operadorNo->direito = pop(pilhaNos);  // segundo operando
        operadorNo->esquerdo = pop(pilhaNos); // primeiro operando
    }
    // Coloca o operador na pilha de nós (subarvore montada)
    push(pilhaNos, operadorNo);
}

// Função para criar a arvore
No* CriarArvore(char* expressaoInfixa) {
    // Declaração e inicializacao das pilhas
    Pilha pilhaOperadores;
    Pilha pilhaNos;
    inicializarP(&pilhaOperadores);
    inicializarP(&pilhaNos);
    int i = 0;
    int tam = strlen(expressaoInfixa);
    
    // Percorre a operação
    while (i < tam) {
        // Se tiver espaço pula para o proximo caractere
        if (isspace(expressaoInfixa[i])) {
            i++;
            continue;
        }
        // Se o caractere for um número
        if (isdigit(expressaoInfixa[i])) {
            char bufferNumero[16]; // Alinhado com o tamanho máximo de valor em No (16 bytes)
            int j = 0;
            // Le os proximos numeros para ver se tem mais digitos
            while (i < tam && (isdigit(expressaoInfixa[i]) || expressaoInfixa[i] == '.')) {
                if (j < 15) {
                    bufferNumero[j++] = expressaoInfixa[i];
                }
                i++;
            }
            bufferNumero[j] = '\0';
            // Cria o no folha do operando e joga na pilha de nos
            push(&pilhaNos, CriarNo(bufferNumero));
        }
        // Se o caractere for um parênteses de abertura
        else if (expressaoInfixa[i] == '(') { 
            char tokenPar[2] = { '(', '\0' };
            push(&pilhaOperadores, CriarNo(tokenPar));
            i++;
        }
        // Se for um parenteses de fechamento
        else if (expressaoInfixa[i] == ')') {
            // Processa tudo até encontrar '('
            while (!pilhaVazia(&pilhaOperadores) && strcmp(pilhaOperadores.topo->noArvore->valor, "(") != 0) {
                processarOperador(&pilhaNos, &pilhaOperadores);
            }
            // Remove o '(' da pilha de operadores e libera a memoria
            if (!pilhaVazia(&pilhaOperadores)) {
                struct No* abrePar = pop(&pilhaOperadores);
                free(abrePar);
            }
            // Se houver uma função antes do parenteses, processa ela
            if (!pilhaVazia(&pilhaOperadores) &&
                operadorUnario(pilhaOperadores.topo->noArvore->valor)) {

                processarOperador(&pilhaNos, &pilhaOperadores);
            }
            i++;
        }
        // Se for uma função ou operador
        else {
            char tokenOp[10];
            // Verifica se a string é sqrt
            if (strncmp(&expressaoInfixa[i], "sqrt", 4) == 0) {
                strcpy(tokenOp, "sqrt");
                i += 4;
            }
            // Se for caractere unico
            else if (expressaoInfixa[i] == '-') {
                // Verificar se o - é unário
                int j = i - 1;
                while (j >= 0 && isspace(expressaoInfixa[j])) j--; // Ignora espaços
                // Será unario se for o primeiro caractere ou vier depois de outro operador
                if (i == 0 || expressaoInfixa[j] == '(' || expressaoInfixa[j] == '+' || expressaoInfixa[j] == '-' ||
                    expressaoInfixa[j] == '*' || expressaoInfixa[j] == '/' || expressaoInfixa[j] == '$') {
                    strcpy(tokenOp, "~");
                } else {
                    strcpy(tokenOp, "-");
                }
                i++;
            }
            // Se for qualquer outro operador
            else {
                tokenOp[0] = expressaoInfixa[i];
                tokenOp[1] = '\0';
                i++;
            }
            // Enquanto o operador do topo da pilha tiver precedencia maior ou igual ao operador atual processamos o operador anterior primeiro
            while (!pilhaVazia(&pilhaOperadores) && 
                    strcmp(pilhaOperadores.topo->noArvore->valor, "(") != 0 && 
                    strcmp(pilhaOperadores.topo->noArvore->valor, "sqrt") != 0 &&
                    precedencia(pilhaOperadores.topo->noArvore->valor) >= precedencia(tokenOp)) {
                processarOperador(&pilhaNos, &pilhaOperadores);
            }
            // Coloca o operador atual na pilha
            push(&pilhaOperadores, CriarNo(tokenOp));
        }
    }
    // Processa qualquer operador que sobre
    while (!pilhaVazia(&pilhaOperadores)) {
        processarOperador(&pilhaNos, &pilhaOperadores);
    }
    // O no que sobrar no topo é a raiz da arvore
    struct No* raizCalculada = pop(&pilhaNos);
    // Limpeza das pilhas de controle
    excluirPilha(&pilhaOperadores);
    excluirPilha(&pilhaNos);
    return raizCalculada;
}

// Calcula o valor final da expressão
double calcArvore(No* raiz) {
    // Caso a arvore esteja vazia
    if (raiz == NULL) {
        return 0.0;
    }
    // Se for um nó folha ele converte a string em double
    if (raiz->esquerdo == NULL && raiz->direito == NULL) {
        return atof(raiz->valor);
    }
    // Se for nó pai, os filhos são avaliados
    double valorEsq = calcArvore(raiz->esquerdo);
    double valorDir = calcArvore(raiz->direito);
    
    // Resolve a operação
    if (strlen(raiz->valor) == 1) {
        char op = raiz->valor[0];
        switch (op) {
            case '+': return valorEsq + valorDir;
            case '-': return valorEsq - valorDir;
            case '*': return valorEsq * valorDir;
            case '/': 
                if (valorDir == 0.0) {
                    printf("Erro: não é possível dividir por zero\n");
                    return 0.0;
                }
                return valorEsq / valorDir;
            case '$': return pow(valorEsq, valorDir);
            case '~': return -valorDir;
        }
    }
    if (strcmp(raiz->valor, "sqrt") == 0) {
        if (valorDir < 0.0) {
            printf("Erro: Raiz quadrada de numero negativo!\n");
            return 0.0;
        }
        return sqrt(valorDir);
    }
    return 0.0;
}

// Imprime a arvore infixa
void imprimirArvore(No* raiz) {
    // Se a raiz estiver vazia
    if (raiz == NULL) {
        return;
    }
    // Parênteses automáticos apenas para operadores binários
    bool ehBinario = (raiz->esquerdo != NULL && raiz->direito != NULL); 
    if (ehBinario) {
        printf("(");
    }
    
    // Tratamento de impressão para unários
    if (operadorUnario(raiz->valor)) {
        if (strcmp(raiz->valor, "~") == 0) {
            printf("-");
        } else {
            printf("%s", raiz->valor);
        }
        imprimirArvore(raiz->direito);
    } else {
        // Imprimir operação binária padrão
        imprimirArvore(raiz->esquerdo);
        printf(" %s ", raiz->valor);
        imprimirArvore(raiz->direito);
    }

    if (ehBinario) {
        printf(")");
    }
}

// Libera recursivamente todos os nós da árvore para evitar Memory Leak
void liberarArvore(No* raiz) {
    if (raiz == NULL) return;
    liberarArvore(raiz->esquerdo);
    liberarArvore(raiz->direito);
    free(raiz);
}

// Verifica de o token é um operador binário
bool operadorBinario(char* token) {
    if (strlen(token) == 1) {
        char c = token[0];
        return (c == '+' || c == '-' || c == '*' || c == '/' || c == '$');
    }
    return false;
}

// Verifica se o operador e unario
bool operadorUnario(char* token) {
    if (strcmp(token, "sqrt") == 0) {
        return true;
    }
    if (strlen(token) == 1 && token[0] == '~') {
        return true;
    }
    return false;
}

// Verifica a prioridade matematica de cada operador
int precedencia(char* token) {
    if (strlen(token) == 1) {
        char c = token[0];
        if (c == '+' || c == '-') return 1;
        if (c == '*' || c == '/') return 2;
        if (c == '$')             return 3;
        if (c == '~')             return 4;
    }
    if (strcmp(token, "sqrt") == 0) {
        return 4;
    }
    return 0;
}