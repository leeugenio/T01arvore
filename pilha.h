#ifndef PILHA_H
#define PILHA_H

#include <stdbool.h>

//Declaração do nó da árvore
struct No;

//Armazena os ponteiros para os nós da árvore
typedef struct ElementoP
{
    struct No* noArvore;
    struct ElementoP* proximo;
} ElementoP;

//Estrutura da pilha
typedef struct Pilha
{
    ElementoP* topo;
}Pilha;

//Funções da pilha
void inicializarP(Pilha* p);
bool pilhaVazia(Pilha* p);
void push(Pilha* p, struct No* novoNo);
struct No* pop(Pilha * p);
void excluirPilha(Pilha* p);

#endif