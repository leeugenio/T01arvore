#include "arvore.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

//Criar novo no na arvore
No* CriarNo(char* valor){
    //alocar memória para a o no
    No* novoNo = (No*)malloc(sizeof(No));
    //se houver falha na alocação retorna erro
    if(novoNo == NULL){
        printf("Erro ao alocar memória para o nó da árvore!\n");
        exit(1);
    }
    //copia o token de forma segura para o vetor do nó
    strncpy(novoNo->valor, valor, sizeof(novoNo->valor)-1);
    //garante que o ultimo caractere seja o \0
    novoNo->valor[sizeof(novoNo->valor) - 1] = '\0';
    //Coloca valores nulos para os nós filhos
    novoNo->esquerdo = NULL;
    novoNo->direito = NULL;

    return novoNo;
}

//Função para desempilhar operadores e montar as subarvores
void processarOperador(Pilha* pilhaNos, Pilha* pilhaOperadores){
    //desempilha operador
    struct No* operadorNo = pop(pilhaOperadores);
    if(operadorNo == NULL) return;
    //se o operador for unário pega o primeiro operando da pilha
    if(operadorUnario(operadorNo->valor)){
        //operando fica a direita
        operadorNo->direito = pop(pilhaNos);
        operadorNo->esquerdo = NULL;
    }
    //se for binario pega dois operandos, o topo da pilha é o operador da direita e o da esquerda o primeiro
    else{
        operadorNo->direito = pop(pilhaNos);  //segundo operando
        operadorNo->esquerdo = pop(pilhaNos); //primeiro operando
    }
    //coloca o operador na pilha de nós (subarvore montada)
    push(pilhaNos, operadorNo);
}

//função para criar a arvore
No* CriarArvore(char* expressaoInfixa){
    //declaração e inicializacao das pilhas
    Pilha pilhaOperadores;
    Pilha pilhaNos;
    inicializarP(&pilhaOperadores);
    inicializarP(&pilhaNos);

    int i = 0;
    int tam = strlen(expressaoInfixa);
    //percorre a operação
    While(i<tam){
        //se tiver espaço pula para o proximo caractere
        if(isspace(expressaoInfixa[i])){
            i++;
            continue;
        }
        //se o caractere for um número
        if(isdigit(expressaoInfixa[i])){
            char bufferNumero(50);
            int j = 0;
            //le os proximos numeros para ver se tem mais digitos
            while (i<tam && isdigit(expressaoInfixa[i])){
                bufferNumero[j++] = expressaoInfixa[i++];
            }
            bufferNumero[j] = '\0';
            //cria o no folha do operando e joga na pilha de nos
            push(&pilhaNos, CriarNo(bufferNumero));
        }
        
    }
}

//calcula o valor final da expressão
double calcArvore(No* raiz){
    //caso a arvore esteja vazia
    if(raiz == NULL){
        return 0.0;
    }
    //se for um nó folha ele converte a string em double
    if(raiz->esquerdo == NULL && raiz->direito == NULL){
        return atof(raiz->valor);
    }
    //se for nó pai, os filhos são avaliados
    double valorEsq = calcArvore(raiz->esquerdo);
    double valorDir = calcArvore(raiz->direito);
    //resolve a operação
    if(strlen(raiz->valor) == 1){
        char op = raiz->valor[0];
        switch (op){
            case '+': return valorEsq + valorDir;
            case '-': return valorEsq - valorDir;
            case '*': return valorEsq * valorDir;
            case '/': 
                if(valorDir == 0.0){
                    printf("Erro: não é possível dividir por zero\n");
                    return 0.0;
                }
                return valorEsq/valorDir;
            case '^': return pow(valorEsq, valorDir);
            case '~': return -valorDir;
        }
    }
    if(strcmp(raiz->valor, "sqrt") == 0){
        if (valorDir < 0.0){
            printf("Erro: Raiz quadrada de numero negativo!\n");
            return 0.0;
        }
        return sqrt(valorDir);
    }
    return 0.0;
}

//imprime a arvore infixa
void imprimirArvore(No* raiz){
    //se a raiz estiver vazia
    if(raiz == NULL) {
        return;
    }
    //verifia se o elemento é operador, só é operador se tiver pelo menos um filho
    bool ehOperador = (raiz->esquerdo != NULL || raiz->direito != NULL);

    if(ehOperador){
        printf("(");
    }
    //imprimir operação
    imprimirArvore(raiz->esquerdo);
    printf(" %s ", raiz->valor);
    imprimirArvore(raiz->direito);

    if(ehOperador){
        printf(")");
    }
}

//verifica de o token é um operador binário
bool operadorBinario(char* token){
    //verifica apenas se tiver 1 caractere
    if(strlen(token) == 1){
        char c = token[0];
        return(c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
    }
    return false;
}
//verifica se o operador eh unario
bool operadorUnario(char*token){
    //verifica se é raiz quadrada "sqrt"
    if(strcmp(token,"sqrt") == 0){
        return true;
    }
    //verifica se o - é ~ unário
    if (strlen(token) == 1 && token[0] == '~'){
        return true;
    }
    return false;
}
//verifica a prioridade matematica de cada operador
int precedencia(char* token){
    if(strlen(token) == 1){
        char c = token[0];
        if(c == '+' || c == '-') return 1;
        if(c == '*' || c == '/') return 2;
        if(c == '^')             return 3;
        if(c == '~')             return 4;
    }
    if(strcmp(token, "sqrt") == 0){
        return 4;
    }
    return 0;
}