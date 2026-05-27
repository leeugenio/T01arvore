#ifndef ARVORE_H
#define ARVORE_H

#include <stdbool.h>

typedef struct Pilha Pilha;

//estrutura do nó da arvore
typedef struct No{
    char valor[16];             //guarda o operador ou operando
    struct No* esquerdo;     //ponteiro para o filho esquerdo
    struct No* direito;      //Ponteiro para o filho direito         
} No;

//protótipos das funções
No* CriarNo(char* valor);
void processarOperador(Pilha* pilhaNos, Pilha* pilhaOperadores);
No* CriarArvore(char* expressaoInfixa);
double calcArvore(No* raiz);
void imprimirArvore(No* raiz);
bool operadorBinario(char* token);
bool operadorUnario(char*token);
int precedencia(char* token);
void liberarArvore(No* raiz);

#endif