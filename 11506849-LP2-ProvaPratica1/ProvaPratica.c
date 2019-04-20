#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h> 

pthread_barrier_t barr;
long int bytes = 0;
int lock; //__sync_lock_test_and_set(&lock,1)
//int chdir(const char *pathname);
int a, b, c;

void *func(void *p){
	char reqs[100];
	char fileREQ[100];
	char answ[100];
	FILE *arquivoREQ;//ponteiro para o arquivo de requisição(entrada)
	FILE *arquivoFILE;
	FILE *arquivoANSW;//ponteiro para o arquivo de resposta(saida)
	long int tamanho1, tamanho2; //tamanho em bytes do arquivo
	int j = (int)p;//parametro 

	printf("[Thread #%d] - Executando.\n",j);

	//abrindo o req

	//CHDIR = change working directory 
	//chdir("reqs");
	const char *directory1 = "/reqs";//DEPOIS DE MTO TEMPO FUNCIONOU PQ EU NAO TINHA BOTADO 'CONST'
	int ret;
	ret = chdir(directory1);

	//req = fopen("req1.req", "r");//r = leitura
	if ((arquivoREQ = fopen("req1.req", "r")) == NULL){
		printf("ERRO! O arquivo não foi aberto!\n");
		exit(1);
	}
	//printf("[Thread #%d] - Executando.\n",j);

	//lendo o req
	char ch1[20], ch2[20];
	while(fscanf(arquivoREQ, "%s %s", &ch1, &ch2) != EOF);
	//printf("%s %s\n", ch1, ch2);
	
	sprintf(fileREQ, "%s", ch2);
	printf("Nome do arquivo: %s\n", fileREQ);
	//tamanho1 = calcularTamanhoArquivo(arquivoREQ);

	//FILES
	//FAZER MUDANÇA DE DIRETORIO PRO FILES---------------------------------------------------------------------------------------
	const char *directory2 = "../files"; ///Documentos/Faculdade/p7/LP2/11506849-LP2-ProvaPratica1/files 
	//.. = voltar diretorio 
	int l;
	l = chdir(directory2);
	//printf("[Thread #%d] - Executando.\n",j);
	//abrindo o arquivo em /files
   	if((arquivoFILE = fopen(fileREQ, "rb")) == NULL){ 
       		printf("ERRO! O arquivo não foi aberto!\n");
       		exit(1);
    	}
	//printf("[Thread #%d] - Executando.\n",j);	FUNCIONOU 
	int tam = calcularTamanhoArquivo(arquivoFILE);	
	printf("Tamanho do arquivo: %d\n", tam);

	//CRIAR O ANSW
	//MUDAR DIRETORIO PRO ANSW-----------------------------------------------------------------------------------------------------
	const char *directory3 = "../answ";///Documentos/Faculdade/p7/LP2/11506849-LP2-ProvaPratica1/answ
	int o;
	o = chdir(directory3);
	//arquivoANSW = fopen(answ, "a");// a = escrita
	if ((arquivoANSW = fopen("answ", "wb")) == NULL){
		printf("ERRO! O arquivo não foi aberto!\n");
	} 
	//printf("[Thread #%d] - Executando.\n",j);
	//COPIAR ARQUIVOS PRO ANSW
   	for(int i = 0; i < tam; i++){ //copia o arquivo de entrada inteiro para o arquivo de saída
      		fputc(fgetc(arquivoFILE), arquivoANSW);
   	}
	//printf("[Thread #%d] - Executando.\n",j);

	//fechando os arquivos
	fclose(arquivoREQ);
	fclose(arquivoFILE);
	fclose(arquivoANSW);
}

int calcularTamanhoArquivo(FILE *arquivo) {
 
    // guarda o estado ante de chamar a função fseek
    long posicaoAtual = ftell(arquivo);
 
    // guarda tamanho do arquivo
    long int tamanho;
 
    // calcula o tamanho
    fseek(arquivo, 0, SEEK_END);//fseek – movimenta a posição corrente de escrita ou leitura no arquivo para um local específico
    tamanho = ftell(arquivo);//ftell – retorna a posição corrente de leitura ou escrita no arquivo (em bytes)
 
    // recupera o estado antigo do arquivo
    fseek(arquivo, posicaoAtual, SEEK_SET);
 
    return tamanho;
}

int main(){
	char q[256];//quantidade de reqs

	//FILE *dir;
  	//struct dirent *lsdir;

	/*const char *directory1 = "/reqs";//MUDANDO DIRETORIO PRA REQS
	int ret;
	ret = chdir(directory1);*/
	//CALCULAR QUANTIDADE DE REQS PRA SUBSTITUIR NO NUMERO DE THREADS

	//int nthreads = atoi(q);
	int nthreads = 2; //usei para testar o func antes de calcular os reqs 

	printf("Serao criadas %d threads \n", nthreads);

	pthread_t threads[nthreads]; 
	for (int i = 1; i < nthreads; i++){
       		pthread_create(&threads[i], NULL, func, i);
        	pthread_join(threads[i], NULL);
	}
}
