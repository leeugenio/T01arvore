#include "pilha.h"
#include <stdlib.h>
#include <stdio.h>

//função para inicializar a pilha
void inicializarP(Pilha* p){
    p->topo = NULL;
}

//função que verifica se a pilha está vazia
bool pilhaVazia(Pilha* p){
    return p->topo == NULL;
}

//Função empilhar um novo elemento
void push(Pilha* p, struct No* novoNo){
    //Aloca memória para o novo elemento, se não conseguiu alocar a memória retorna erro
    ElementoP* novoElemento = (ElementoP*)malloc(sizeof(ElementoP));
    if(novoElemento == NULL){
        printf("Erro ao alocar memória da pilha!\n");
        exit(1);
    }
    //cria um novo nó
    novoElemento->noArvore = novoNo;
    //torna o antigo nó do topo no próximo nó 
    novoElemento->proximo = p->topo;
    //coloca o novo elemento no topo do nó
    p->topo = novoElemento;
}

//função desempilhar
struct No* pop(Pilha* p){
    //verifica se a pilha está vazia
    if (pilhaVazia(p)) {
        return NULL;
    }
    //guarda o elemento do topo em aux
    ElementoP* aux = p->topo;
    //Torna aux no no da arvore
    struct No* no = aux->noArvore;
    //coloca o próximo nó no topo da pilha
    p->topo = p->topo->proximo;
    //libera a memória de aux
    free(aux);
    return no;    
}

//função liberar 
void excluirPilha(Pilha* p){
    //enquanto a pilha não estiver vazia, ela desempilha 
    while(!pilhaVazia(p)) {
        pop(p);
    }
}