//Trabalho 2 - Organizacao e Recuperacao de Arquivos
//Matheus Rezende Milani Videira - RA: 790809
//Matheus da Silva Vieira - RA: 791085

#include <stdio.h>
#include <stdlib.h>

typedef struct Noh{
    int *chaves; //Array com as chaves
    int t; //Grau minimo
    struct Noh **filhos; //Array com ponteiros para os filhos
    int n; //Quantidade de chaves
    int folha; //Indica se o noh eh folha
} Noh;

typedef struct Arvore{
    Noh *raiz; //Ponteiro para a raiz da arvore
    int t; //Grau minimo
} Arvore;

Noh* criaNoh(int t, int folha); //Cria um noh
Noh* insercaoNaoCheia(Noh *noh, int chave, int *indice, int t); //Funcao auxiliar para inserir chave
Noh* insercao(Arvore *arvore, int chave, int *indice); //Insere chave na arvore
void divideNoh(Noh *noh, int i); //Divide um noh cheio
Noh* busca(Noh *noh, int chave, int *indice); //Busca chave na arvore

//Cria um noh, recebe o grau minimo e se eh folha ou nao como parametros
//Retorna o ponteiro par o noh criado
Noh* criaNoh(int t, int folha){
    Noh *noh = (Noh*)malloc(sizeof(Noh));
    noh->chaves = (int*)malloc(sizeof(int)*(2*t-1));
    for(int i = 0; i < 2*t-1; i++){
        noh->chaves[i] = -1;
    }
    noh->filhos = (Noh**)malloc(sizeof(Noh*)*2*t);
    for(int i = 0; i < 2*t; i++){
        noh->filhos[i] = NULL;
    }
    noh->n = 0;
    noh->folha = folha;
    noh->t = t;
}

//Busca chave na arvore, recebe o noh, a chave e o indice como parametros
//Retorna o ponteiro para o noh onde a chave foi encontrada
//indice eh atualizado para o indice da chave no noh
Noh* busca(Noh *noh, int chave, int *indice){
    int i = 0;
    if(noh == NULL){
        return NULL;
    }
    while(i < noh->n && chave > noh->chaves[i]){ //Procura a chave no noh
        i++;
    }
    if(noh->chaves[i] == chave){ //Chave encontrada
        *indice = i;
        return noh;
    }
    
    return busca(noh->filhos[i], chave, indice); //Chave nao encontrada, busca na subarvore
}

//Insere chave na arvore, recebe a arvore, a chave e o indice como parametros
//Retorna o ponteiro para o noh onde a chave foi inserida
//indice eh atualizado para o indice da chave no noh
Noh* insercao(Arvore *arvore, int chave, int *indice){
    Noh *noh = arvore->raiz;
    Noh *retorno;
    retorno = noh;
    if(noh == NULL){ //Se a arvore estiver vazia, cria um noh como raiz
        arvore->raiz = criaNoh(arvore->t, 1);
        arvore->raiz->chaves[0] = chave;
        arvore->raiz->n = 1;
        return arvore->raiz;
    }
    if(noh->n == 2*arvore->t - 1){ //Se o noh estiver cheio, divide ele
        Noh* temp = criaNoh(arvore->t, 0);
        temp->filhos[0] = arvore->raiz;
        divideNoh(noh, 0);
        noh->folha = 0;
        int i = 0;
        if(chave > temp->chaves[0]){
            i++;
        }
        //Insere a chave no noh
        retorno = insercaoNaoCheia(temp->filhos[i], chave, indice, arvore->t);
        temp->filhos[i] = retorno;
        arvore->raiz = temp;

    } else {
        retorno = insercaoNaoCheia(arvore->raiz, chave, indice, arvore->t);
    }
    return retorno;
}

//Funcao auxiliar para inserir chave, recebe o noh, a chave e o indice como parametros
//Retorna o ponteiro para o noh onde a chave foi inserida
//indice eh atualizado para o indice da chave no noh
Noh* insercaoNaoCheia(Noh *noh, int chave, int *indice, int t){
    if(noh == NULL){ //Se o noh for cria um noh
        noh = criaNoh(t, 1);
        noh->chaves[0] = chave;
        noh->n = 1;
        *indice = 0;
        return noh;
    }
    int i = noh->n - 1;
    Noh* retorno = noh;
    if(noh->folha){ //Se o noh for folha, insere a chave
        //Enquanto a chave for maior que a chave do noh, procura a posicao correta
        while(i >= 0 && chave < noh->chaves[i]){
            noh->chaves[i+1] = noh->chaves[i];
            i--;
        }
        noh->chaves[i+1] = chave;
        noh->n++;
        *indice = i+1;
        return noh;
    } else { //Se o noh nao for folha, procura a subarvore correta
        //Enquanto a chave for maior que a chave do noh, procura a posicao correta
        while(i >= 0 && chave < noh->chaves[i]){
            i--;
        }
        i++;
        if(noh->filhos[i] == NULL){ //Se a subarvore nao existir, cria uma
            noh->filhos[i] = criaNoh(t, 1);
        }
        if(noh->filhos[i]->n == 2*noh->t - 1){ //Se a subarvore estiver cheia, divide ela
            divideNoh(noh->filhos[i], i);
            if(chave > noh->chaves[i]){
                i++;
            }
        }
        retorno = insercaoNaoCheia(noh->filhos[i], chave, indice ,noh->t); //Insere a chave na subarvore
    }
    *indice = i;
    return retorno;
}

//Divide um noh, recebe o noh e o indice como parametros
void divideNoh(Noh *noh, int i){
    Noh* temp = criaNoh(noh->t, noh->folha); //Cria um novo noh
    temp->n = noh->t - 1;
    for(int j = 0; j < temp->n; j++){ //Copia as chaves para o novo noh
        temp->chaves[j] = noh->chaves[j+noh->t];
    }
    if(!noh->folha){ //Se o noh nao for folha, copia os filhos para o novo noh
        for(int j = 0; j < temp->n; j++){
            temp->filhos[j] = noh->filhos[j+noh->t];
        }
    }
    noh->n = noh->t - 1; 
    for(int j = noh->n; j >= i+1; j--){ //Libera espaco para nova chave
        noh->filhos[j+1] = noh->filhos[j];
    }
    noh->filhos[i+1] = temp;

    for(int j = noh->n-1; j >= i; j--){ //Libera espaco para nova chave
        noh->chaves[j+1] = noh->chaves[j];
    }
    noh->chaves[i] = temp->chaves[noh->t-1];
    noh->n++;
}

int main(){
    int chave, opt, indice = 0;
    Arvore arvore;
    Noh *temp;
    opt = 0;
    printf("Digite o grau da arvore: ");
    scanf("%d", &arvore.t);
    while(opt != 3){
        printf("1. Insere\t 2. Procurar\t 3. Sair\n");
        scanf("%d", &opt);
        switch(opt){
            case 1: 
                printf("Insira a chave:\n");
                scanf("%d", &chave);
                temp = insercao(&arvore, chave, &indice);
                if(temp == NULL){
                    printf("Erro: chave duplicada!\n");
                }
                else{
                    printf("Elemento inserido!\n");
                    printf("No: %p Indice: %d\n\n", temp, indice);
                }
                break;
            case 2:
                printf("Entre o elemento para procurar:\n");
                scanf("%d", &chave);
                temp = busca(arvore.raiz, chave, &indice);
                if(temp == NULL){
                    printf("Elemento nao encontrado!\n");
                } else{
                    printf("Elemento encontrado!\n");
                    printf("No: %p Indice: %d\n\n", temp, indice);
                }
                break;
            case 3:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida\n");
                break;
        }
    }
    return 0;
}

