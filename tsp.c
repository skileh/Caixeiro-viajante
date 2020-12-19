#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct no{

	int custo; 		// armazena o custo total de cada nó
	int *sequencia;
	int sizeSeq;
}NO;

//insere os valores na matriz de adjacencia
void insertValues(int **vet, int x, int y, int custo){
	vet[x][y] = custo;
	vet[y][x] = custo;
}


//imprime na tela o grafo das cidades
void printar(int **vet, int size){
	printf("########################################\nMATRIZ : %d x %d \n", size, size);
	for(int i=0 ; i<size; i++){
		for(int j=0 ; j<size; j++){
			printf(" %d", vet[i][j]);
		}
		printf("\n");
	}
	printf("########################################\n");
}


// Busca o menor caminho sempre, a cada busca, verifica se existe um caminho vizinho menor, se houver então segue ele.
// Le o grafo da matriz N x N e iniciando pelo ponto valInicial salvando todos os caminhos possiveis a partir deste valor 
// e percorre o grafo recursivamente para encontrar o menor caminho sempre.
//inicia buscando sempre o menor valor, e caso não encontre o valor final, retorna uma mensagem de erro.
//mais detalhes no relatorio.
NO *menorCaminho(int **vet, NO *grafo,int size, int sizeGraf,int valInicial){
	int aux=9999;
	int i=0;
	int auxIndex=9999;
	int verifica=0;
	//busca na matriz o menor valor da conexão inicial
	for(i=0; i<size;i++){
		if(aux>vet[valInicial][i]){
			if(vet[valInicial][i] != -1){
				for(int j=0; j<sizeGraf;j++){
					if(i == grafo[j].sizeSeq){
						verifica = 1;
					}
				}
				if(verifica == 0){
					aux = vet[valInicial][i];
					auxIndex=i;

				}
			}
		}
		verifica=0;
	}
	// salva o menor valor em um vetor
	sizeGraf++;
	grafo = realloc(grafo,sizeGraf*sizeof(NO));
	if(grafo == NULL){
		printf("ERRO AO REALOCAR GRAFO");
	}
	grafo[sizeGraf-1].sizeSeq = auxIndex;
	grafo[sizeGraf-1].custo = grafo[sizeGraf-2].custo + aux;
	//faz o teste para ver se o menor valor é o valor final
	// caso não seja, então entra recursivamente em busca deste valor
	// e caso o valor final não seja encontrado, informa um erro de busca.
	if(sizeGraf == size){
		
		grafo->sequencia = &sizeGraf;
		return grafo;

	}else if(auxIndex==9999){
		int k =0;
		grafo->sequencia = &k;
		return grafo;
	}else{
		menorCaminho(vet, grafo,size,sizeGraf,auxIndex);
	}
}

// inicia a busca do menor caminho, executa a função acima duas vezes
// uma do nó inicial até o final e outra do nó final ao inicial
// compara o menor valor e retorna ele junto com os nodos percorridos.
// também percorre varias vezes em busca do menor valor para solução ótima.
void menorCaminhoIni(int **vet, int size ,int primeiro){
	
	if(primeiro ==0){
		NO *grafo = (NO *) malloc(sizeof(NO));

		int inicio;
		printf("\nInsira o numero da cidade incial: ");
		scanf("%d", &inicio);
		grafo->custo=0; 
		grafo->sizeSeq=inicio; // valor do primeiro nodo
		grafo=menorCaminho(vet,grafo,size,1,1);
		int sizeGraf = * grafo->sequencia;
		int aux = grafo[sizeGraf-1].custo;
		
		if(aux && sizeGraf!=0){
			int custoTotal = grafo[sizeGraf-1].custo+vet[sizeGraf-1][0];
			printf("\n DISTANCIA TOTAL DO APROXIMATIVO: %d  \nCIDADES: ", custoTotal);
			for(int j=0;j<sizeGraf;j++){
				printf("- %d", grafo[j].sizeSeq);
			}
			printf("- %d", grafo[0].sizeSeq);
			printf("\n");
			free(grafo);
		
		}else if(sizeGraf== 0){
			printf("ERRO, NO FINAL NÃO ENCONTRADO");
		}
	}
	else{
		NO **grafo = malloc (size * sizeof(NO*));
			
		int aux[size], sizeGraf[size];
		for(int i=0;i<size;i++){
			grafo[i] = (NO *) malloc(sizeof(NO));
			grafo[i][0].custo=0; 
			grafo[i][0].sizeSeq=i; // valor do primeiro nodo
			grafo[i] = menorCaminho(vet,grafo[i],size,1,i );
			sizeGraf[i] = * grafo[i][0].sequencia;
			aux[i] = grafo[i][sizeGraf[i]-1].custo;
		}
		int auxTotal =99999;
		int x;
		for(int j=0;j<size;j++){
			if(auxTotal>aux[j]){
				auxTotal=aux[j];
				x=j;
			}
		}
		int somaTotal = grafo[x][sizeGraf[x]-1].custo+vet[sizeGraf[x]-1][0];
		printf("\n DISTANCIA TOTAL DO ÓTIMO: %d  \nCIDADES: ", somaTotal);
		for(int j=0;j<sizeGraf[x];j++){
			printf("- %d", grafo[x][j].sizeSeq);
		}
		printf("- %d", grafo[x][0].sizeSeq);
		printf("\n");
		for(int i=0; i<size; i++){
			free(grafo[i]);
		}
		free(grafo);

	}
}

int main(){
	printf("INFORME A QUANTIDADE DE CIDADES, AS DISTANCIAS ENTRE ELAS SERÃO GERADAS AUTOMATICAMENTE\n");
	int size=500;
	printf("Insira a quantidade de cidades: ");
	scanf("%d",&size);
	int **vet;
	
	vet = (int **)malloc(size * sizeof(int*));
	if(vet == NULL){
		printf("ERROR 1");
	}
	for(int i=0; i<size ; i++){
		vet[i] = (int * )malloc(size * sizeof(int));
		if(vet[i]==NULL){
			printf("ERROR 2");
		}
	}
	int aux, rands;
	for(int i=0;i<size;i++){
		for(int j=i;j<size;j++){
			if(i!=j){
				aux = rand() % 50; // custo de cada aresta // de 0 até 100
				rands = rand() % 75; // 75 % de chance de gerar uma aresta
				if(rands == 1){
					insertValues(vet,i,j,-1);
				}else{
					aux=aux+1;
					insertValues(vet,i,j,aux);
				}
			}
			else{
				insertValues(vet,i,j,-1);
			}
		}
	}
	
	 clock_t Ticks[2];
	
	Ticks[0] = clock();
	menorCaminhoIni(vet,size,0);
	Ticks[1] = clock();
    double Tempo = (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
    printf("Tempo gasto: %g ms.\n", Tempo);

	Ticks[0] = clock();
	menorCaminhoIni(vet,size,1);
	Ticks[1] = clock();
    Tempo = (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
    printf("Tempo gasto: %g ms.", Tempo);
    printf("\n");
	//printar(vet,size);// descomente aqui para ver a matriz na tela, use valores pequenos para poder ver direitinho..

	free(vet);
	return 0;
}

