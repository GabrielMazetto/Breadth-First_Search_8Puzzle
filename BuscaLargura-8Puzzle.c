#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

//ASSINATURAS DAS FUNÇÕES
void IniciaAleatoria(int matriz[3][3]);
//Imprime a variavel estado, que é uma matriz
void printEstado(int estado[3][3]);
//Copia o conteudo da matriz estado para a matriz copiaEstado
void deepcopy(int estado[3][3], int copiaEstado[3][3]);
//Copia a matriz m1 e faz uma alteracao simples (duplica os valores) alterando a variavel novaMatriz
int altera(int m1[3][3], int novaMatriz[3][3], int dir);

//Verifica se a posição que será movido a peça está dentro dos limites do tabuleiro
int isValidPosition(int finali, int finalj);

//Verifica se chegou no estado meta
int Avaliacao(int estado[3][3]);

//Estrutura do nó
struct no
{
    int estado[3][3];
    struct no *prox;
    struct no *pai;
};
typedef struct no No;

//Estrutura da fila
typedef struct fila
{
    No *inicio;
    No *fim;
} Fila;

//Cria a fila responsável por armazenar os próximos nós a serem visitados
Fila *criarFila();

//Insere um dado nó no final da fila
void InserirFila(Fila *fila, int estado[3][3], No *paiNo);

//Remove o nó do inicio da fila e o retorna
No* RemoverFila(Fila *fila);

//Estrutura da pilha
typedef struct pilha
{
    No *topo;
} Pilha;

//Cria a pilha responsável por armazenar o caminho até a solução
Pilha *criarPilha();

//Insere um dado nó no topo da pilha
void InserirPilha(Pilha *pilha, No *novoNo);

//Remove o nó do topo da pilha e o imprime
void RemoverImprimirPilha(Pilha *pilha);


//Adiciona os filhos do nó atual na fila de nós a serem visitados
void getCurrentOptions(Fila *fila);


//Executa a busca em largura e imprime o caminho para a solução
void BuscaEmLargura(Fila *fila);


//Encontra a posição do zero
void findZeroPosition(int estado[3][3], int *posZeroI, int *posZeroJ);



//MAIN
int main()
{
    Fila *fila = criarFila();

    //int estado[3][3] = {{4,1,3},{7,2,5},{8,0,6}};
    int estado[3][3] = {{0,1,3},{7,4,6},{5,8,2}};
    
    printf("Estado inicial:\n");
    printEstado(estado);

    //Insere o primeiro estado na fila
    InserirFila(fila, estado, NULL);
    getCurrentOptions(fila);

    //Executa a busca em largura
    BuscaEmLargura(fila);


    return 0;
}


//CORPO DAS FUNCOES



//Função para preencher a matriz aleatoriamente
void IniciaAleatoria(int matriz[3][3])
{
    int numerosDisponiveis[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    //Embaralhar os números disponíveis
    srand(time(NULL));
    for (int i = 8; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = numerosDisponiveis[i];
        numerosDisponiveis[i] = numerosDisponiveis[j];
        numerosDisponiveis[j] = temp;
    }

    //Preencher a matriz com os números embaralhados
    int contador = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            matriz[i][j] = numerosDisponiveis[contador];
            contador++;
        }
    }
}





//Imprime a variavel estado, que é uma matriz
void printEstado(int estado[3][3])
{
    for(int i=0; i<3; i++)
    {
        printf("\n");
        for(int j=0; j<3; j++)
        {
            printf("  %d",estado[i][j]);
        }
    }
    printf("\n");
    return;
}


//Copia o conteudo da matriz estado para a matriz copiaEstado
void deepcopy(int estado[3][3], int copiaEstado[3][3])
{
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            copiaEstado[i][j] = estado[i][j];
        }
    }
    return;
}

//Encontra a posição do zero
void findZeroPosition(int estado[3][3], int *posZeroI, int *posZeroJ)
{
    for (int i=0; i<3; i++)
    {
        for (int j=0; j<3; j++)
        {
            if(estado[i][j] == 0)
            {
                *posZeroI = i;
                *posZeroJ = j;
                break;
            }
        }
    }
}

//Verifica se a peça está se movendo dentro do tabuleiro
int isValidPosition(int finali, int finalj)
{
    if ((finali <= 2 && finali >= 0) && (finalj <= 2 && finalj >= 0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//Copia a matriz m1 e faz uma alteracao simples (duplica os valores) alterando a variavel novaMatriz
int altera(int m1[3][3], int novaMatriz[3][3], int dir)
{
    deepcopy(m1, novaMatriz);
    int posZeroI;
    int posZeroJ;
    findZeroPosition(novaMatriz, &posZeroI, &posZeroJ);

    //0: Esq, 1: Dir, 2: Cima, 3: Baixo
    if (dir == 0 && isValidPosition(posZeroI, posZeroJ - 1))
    {
        novaMatriz[posZeroI][posZeroJ] = novaMatriz[posZeroI][posZeroJ - 1];
        novaMatriz[posZeroI][posZeroJ - 1] = 0;
        return 1;
    }
    else if (dir == 1 && isValidPosition(posZeroI, posZeroJ + 1))
    {
        novaMatriz[posZeroI][posZeroJ] = novaMatriz[posZeroI][posZeroJ + 1];
        novaMatriz[posZeroI][posZeroJ + 1] = 0;
        return 1;
    }
    else if (dir == 2 && isValidPosition(posZeroI - 1, posZeroJ))
    {
        novaMatriz[posZeroI][posZeroJ] = novaMatriz[posZeroI - 1][posZeroJ];
        novaMatriz[posZeroI - 1][posZeroJ] = 0;
        return 1;
    }
    else if (dir == 3 && isValidPosition(posZeroI + 1, posZeroJ))
    {
        novaMatriz[posZeroI][posZeroJ] = novaMatriz[posZeroI + 1][posZeroJ];
        novaMatriz[posZeroI + 1][posZeroJ] = 0;
        return 1;
    }
    
    return 0;
}

//Verifica se chegou no estado meta
int Avaliacao(int estado[3][3])
{
    int estadoMeta[3][3] = {{1,2,3},{4,5,6},{7,8,0}};
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(estadoMeta[i][j] != estado[i][j])
            {
                return 0;
            }
        }
    }
    return 1;
}

//Adiciona os filhos do nó atual na fila de nós a serem visitados
void getCurrentOptions(Fila *fila)
{
    int novoEstado[3][3];
    if (fila->inicio != NULL)
    {
        for (int dir = 0; dir <= 3; dir++)
        {
            int alterou = altera(fila->inicio->estado, novoEstado, dir);

            //Se for possivel fazer o movimento, adiciona na fila
            if (alterou == 1)
            {
                InserirFila(fila, novoEstado, fila->inicio);
            }
        }
    }
}


//Cria a fila responsável por armazenar os próximos nós a serem visitados
Fila *criarFila()
{
    Fila *novaFila = (Fila*)malloc(sizeof(Fila));
    novaFila->inicio = NULL;
    novaFila->fim = NULL;
    return novaFila;
}

//Insere um dado nó no final da fila
void InserirFila(Fila *fila, int estado[3][3], No *paiNo)
{
    No *novoNo = (No*)malloc(sizeof(No));

    deepcopy(estado, novoNo->estado);

    novoNo->prox = NULL;
    novoNo->pai = paiNo;

    if (fila->inicio == NULL)
    {
        fila->inicio = novoNo;
        fila->fim = novoNo;
    }
    else
    {
        fila->fim->prox = novoNo;
        fila->fim = novoNo;
    }
}

//Remove o nó do inicio da fila e o retorna
No* RemoverFila(Fila *fila)
{
    if (fila->inicio == NULL)
    {
        return NULL;
    }

    No *temp = fila->inicio;
    fila->inicio = fila->inicio->prox;

    //Se a fila ficar vazia
    if (fila->inicio == NULL)
    {
        fila->fim = NULL;
    }

    return temp;
}




//Cria a pilha responsável por armazenar o caminho até a solução
Pilha *criarPilha()
{
    Pilha *novaPilha = (Pilha*)malloc(sizeof(Pilha));
    novaPilha->topo = NULL;
    return novaPilha;
}

//Insere um dado nó no topo da pilha
void InserirPilha(Pilha *pilha, No *novoNo)
{
    if (pilha == NULL || novoNo == NULL)
    {
        return;
    }

    novoNo->prox = pilha->topo;
    pilha->topo = novoNo;
}

//Remove o nó do topo da pilha e o imprime
void RemoverImprimirPilha(Pilha *pilha)
{
    if (pilha == NULL || pilha->topo == NULL)
    {
        return; 
    }

    No *removido = pilha->topo;
    pilha->topo = removido->prox;
    removido->prox = NULL;

    printEstado(removido->estado);
    free(removido);
}


//Executa a busca em largura e imprime o caminho para a solução
void BuscaEmLargura(Fila *fila)
{
    while (fila->inicio != NULL)
    {
        No *removido = RemoverFila(fila);
        if (Avaliacao(removido->estado))
        {
            printf("\nSolução encontrada!\n");
            printEstado(removido->estado);

            printf("\nCaminho:\n");
            Pilha *caminho = criarPilha();
            //Adiciona em uma pilha o caminho (pois partimos do ultimo estado e caminhamos até o primeiro, necessitando da pilha para imprimir o inverso)
            while(removido->pai != NULL)
            {
                InserirPilha(caminho, removido);
                removido = removido->pai;
            }
            //Imprime o caminho
            while(caminho->topo != NULL)
            {
                RemoverImprimirPilha(caminho);
            }
            printf("\n8-puzzle resolvido.");
            free(removido);
            return;
        }
        else
        {
            getCurrentOptions(fila);
        }
    }

    printf("Nenhuma solução encontrada.\n");
}