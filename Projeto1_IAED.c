#include <stdio.h>
#include <string.h>

#define FR_MAX 140
#define USER_MAX 999
#define MSG_MAX 10000
#define EXIT_SUCCESS 0


int num_msgs = 0;			/*Variavel que guarda o nr de mensagens postadas no forum*/
int participacoes[USER_MAX+1]={0};	/*Vetor q guarda o nr de participacoes de cada user. Indice do vetor corresponde ao nr do user*/
int mais_ativo = 0;			/*Variavel que guarda o nr de participacoes do user mais ativo*/
int mais_longa = 0;			/*Variavel que guarda o tamanho da mensagem mais longa do forum*/

typedef struct{
	int user;
	char frase[FR_MAX+1];
}mensagens;

mensagens msgs[MSG_MAX];


/*------------------------Merge Sort-------------------------*/
void merge(mensagens a[], int left, int mid, int right) {
	int c1 = left, c2 = mid+1, cAux = 0;
	mensagens aAux[right-left+1];
	while(c1<=mid && c2<=right){
		if(strcmp(a[c1].frase, a[c2].frase)<0){ 
			aAux[cAux] = a[c1];
			c1++;
		}
		else if(strcmp(a[c1].frase, a[c2].frase)==0 && a[c1].user < a[c2].user){ /*Criterio de desempate menor user id primeiro*/
			aAux[cAux] = a[c1];
			c1++;			
		}
		else{
			aAux[cAux] = a[c2];
			c2++;
		}
		cAux++;
	}
	while(c1<=mid){  
		aAux[cAux] = a[c1];
		cAux++;
		c1++;
	}
	while(c2<=right){
        	aAux[cAux] = a[c2];
        	cAux++;
		c2++;
	}
	for(cAux=left; cAux<=right; cAux++){
		a[cAux] = aAux[cAux-left];
	}
}

void mergesort(mensagens a[], int left, int right){
	if (left < right){
		int mid = (left+right)/2;
		mergesort(a, left, mid);
		mergesort(a, mid+1, right);
		merge(a, left, mid, right);
	}
}
/*-----------------------------------------------------------*/

void function_A(){
	getchar();
	scanf("%d", &msgs[num_msgs].user);
	getchar();
	fgets(msgs[num_msgs].frase,MSG_MAX+1,stdin);
	if (strlen(msgs[num_msgs].frase) > mais_longa)			/*Verifica se ha atualizacao da mais_longa*/
		mais_longa = strlen(msgs[num_msgs].frase);
	participacoes[msgs[num_msgs].user]++;				/*Incrementa o nr de participacoes do user que escreveu a msg*/
	if (participacoes[msgs[num_msgs].user] > mais_ativo)
		mais_ativo = participacoes[msgs[num_msgs].user];	/*Verifica se ha atualizacao do mais_ativo*/

	num_msgs++;							/*Incrementa o num_msgs presentes no forum*/
}


void function_L(){
	int i;	
	printf("*TOTAL MESSAGES:%d\n",num_msgs);
	for (i=0; i<num_msgs; i++)				
		printf("%d:%s",msgs[i].user,msgs[i].frase);
}


void function_U(){
	int i, utilizador;
	getchar();
	scanf("%d", &utilizador);
	printf("*MESSAGES FROM USER:%d\n", utilizador);
	for (i=0; i<num_msgs; i++){
		if ((msgs[i].user) == utilizador)			/*Verifica se a mensagem pertence ao user que queremos*/
			printf("%s",msgs[i].frase);
	}
}


void function_O(){
	int i;
	for (i=0; i<num_msgs; i++){
		if (strlen(msgs[i].frase) == mais_longa)		/*Verifica se a mensagem tem o tamanho da msg mais longa*/
			printf("*LONGEST SENTENCE:%d:%s",msgs[i].user,msgs[i].frase);
	}
}


void function_T(){
	int i;
	for (i=0; i<USER_MAX+1; i++){
		if (participacoes[i] == mais_ativo)
			printf("*MOST ACTIVE USER:%d:%d\n",i,mais_ativo);
	}
}


void function_S(){
	printf("*SORTED MESSAGES:%d\n", num_msgs);
	int i;
	mensagens msgs_aux[MSG_MAX+1];
	for (i=0; i<num_msgs; i++){				/*Constroi um vetor de mensagens auxiliar*/
		msgs_aux[i].user = msgs[i].user;
		strcpy(msgs_aux[i].frase, msgs[i].frase);
	}
	mergesort(msgs_aux, 0, num_msgs-1);			/*Aplicar o mergesort ao vetor de mensagens auxiliar*/
	for (i=0; i<num_msgs; i++)
		printf("%d:%s", msgs_aux[i].user, msgs_aux[i].frase);
}


void function_C(){
	char palavra[FR_MAX+1], frase_aux[FR_MAX+1];
	int i, num_ocorrencias=0;
	char* palavra_aux;
	getchar();
	scanf("%s", palavra);
	for (i=0; i<num_msgs; i++){
		strcpy(frase_aux, msgs[i].frase);		/*Cria um vetor auxiliar que contem a i-gesima mensagem*/
		palavra_aux = strtok(frase_aux, ";, \t\n.");
		while (palavra_aux != NULL){			/*Vamos avaliar palavra a palavra ate chegarmos ao fim da mensagem*/
			if (strcmp(palavra_aux,palavra)==0)	/*Avalia se a palavra obtida pelo strtok e a palavra que queremos*/
				num_ocorrencias++;
			palavra_aux = strtok(NULL, ";, \t\n.");
		}
	}
	printf("*WORD %s:%d\n", palavra, num_ocorrencias);
}


void function_X(){
	printf("%d\n",num_msgs);	
}


int main (){
	char command;
	while (1){
		command = getchar();
		switch (command){
		case 'A':
			function_A();
			break;
		case 'L':
			function_L();
			getchar();
			break;
		case 'U':
			function_U();
			getchar();
			break;
		case 'O':
			function_O();
			getchar();
			break;
		case 'T':
			function_T();
			getchar();
			break;
		case 'S':
			function_S();
			getchar();
			break;
		case 'C':
			function_C();
			getchar();
			break;
		case 'X':
			function_X();
			return EXIT_SUCCESS;
		default:
			printf("ERRO: Command not found\n");
		}
	}
	return 0;
}
