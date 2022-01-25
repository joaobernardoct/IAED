///////////////////////////////////////////////////////////////////////////////
//     Projeto 2 IAED                                Joao Bernardo 86443     //
//     Grupo 88                                      Pedro Antunes 86493     //
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Key(A) (A->chave)
#define Key2(C) (C->unidades)
#define eq(A, B) (strcmp(A, B) == 0)
#define less(A, B) (strcmp(A, B) < 0)
#define CHAVE 8

typedef char* Key;

typedef struct produto {
	char chave[CHAVE+1]; 
	int unidades;
}* Produto;

///////////////////////////////////////////////////////////////////////////////
// Utilizacao de arvores binarias para organizar as informacoes dos produtos //
///////////////////////////////////////////////////////////////////////////////

#define Item Produto

typedef struct STnode* link;

struct STnode{
	Item item;
	link l, r;
};

static link head;
static int flag_del;	// toma o valor 1 quando o maximo foi eliminado ou subtraido algo ao seu valor
static int maximo=0;	// variavel que guarda as unidades do maximo
static char max_chave[CHAVE+1]="ffffffff";		// variavel que guarda a chave associada ao maximo


Item newItem(char ch[CHAVE+1], int uni){ 
    Item x = (Item)malloc(sizeof(struct produto)); 
    strcpy(x->chave, ch);
    x->unidades = uni;
    return x; 
} 

void deleteItem(Item a){ 
    free(a); 
} 

void visitItem(Item a) { 
	printf("%s %d\n",a->chave,a->unidades);
}


link NEW(Item item, link l, link r){
	// Cria um novo no na arvore para Item
	link x = (link)malloc(sizeof(struct STnode));
	x -> item = item;
	x -> l = l;
	x -> r = r;
	// Devolve um ponteiro para este no
	return x;
}


void STinit(link*head){
	// Inicia a arvore
	*head = NULL;
}


link insertR(link h, Item item){
	// Insere o Item de forma ordenada na arvore, com base na sua referencia (Key)
	if (h == NULL)
		return NEW(item, NULL, NULL);
	if (less(Key(item), Key(h->item)))
		h->l = insertR(h->l, item);
	else
		h->r = insertR(h->r, item);
	// Devolve a raiz da arvore atualizada
	return h;
}


void STinsert(link*head, Item item){
	*head = insertR(*head, item);
}


Item searchR(link h, Key v){
	// Procura na arvore de raiz 'h' o no com um Item de referencia v, devolve
	// um ponteiro para esse no, caso o Item exista e NULL em caso contrario
	if (h == NULL)
		return NULL;
	if (eq(v, Key(h->item)))
		return h->item;
	if (less(v, Key(h->item)))
		return searchR(h->l, v);
	else
		return searchR(h->r, v);
}


Item STsearch(link head, Key v){
	return searchR(head, v);
}


void atualiza_maximo(char ch[CHAVE+1], int uni){
	if (uni > maximo){		// se uni for maior que o maximo
		maximo = uni;			//maximo passa a ser uni
		strcpy(max_chave,ch);	//e max_chave passa a ser chave
	}
	else if(uni == maximo && less(ch, max_chave)){		// se forem iguais e a chave for lexicograficamente menor que max_chave
		strcpy(max_chave,ch);	// max_chave assume o valor de chave
	}
	else
		return;
}

link max(link h){
	if (h==NULL || h->r==NULL)
		return h;
	else
		return max(h->r);
}


link deleteR(link h, Key k){
	if (h==NULL)
			return h;
	else if (less(k, Key(h->item)))
		h->l=deleteR(h->l,k);
	else if (less(Key(h->item), k))
		h->r=deleteR(h->r,k) ;
	else{
		if (h->l !=NULL && h->r !=NULL){
		link aux = max(h->l);
		{Item x; x=h->item; h->item=aux->item; aux->item=x;}
		h->l= deleteR(h->l, Key(aux->item));
		}
		else {
			link aux = h;
			if (h->l == NULL && h->r == NULL)
				h=NULL;
			else if (h->l==NULL)
				h=h->r;
			else
				h=h->l;
			deleteItem(aux->item);
			free(aux);
		}
	}
	return h;
}

void STdelete(link*head, Key k){
	*head = deleteR(*head, k);
}



int count(link h){
	if (h==NULL)
		return 0;
	else
		return count(h->r)+count(h->l)+1;
}


int STcount(link head){
	return count(head);
}


void sortR(link h, void (*visit)(Item)){
	if (h == NULL)
		return;
	sortR(h->l, visit);
	visit(h->item);
	sortR(h->r, visit);
}


void STsort(link head, void (*visit)(Item)){
	sortR(head, visit);
}


link freeR(link h){
	if (h==NULL)
		return h;
	h->l=freeR(h->l);
	h->r=freeR(h->r);
	return deleteR(h,Key(h->item));
}


void STfree(link*head){
	*head=freeR(*head);
}


void traverse(link h){
	// visita a raiz depois do filho esquerdo e antes do direito
	if (h == NULL)
		return;
	traverse(h->l);
	atualiza_maximo(Key(h->item), Key2(h->item));	// verifica se houve alteracao no maximo
	traverse(h->r);
}


///////////////////////////////////////////////////////////////////////////////
//                                    Comandos                               //
///////////////////////////////////////////////////////////////////////////////


void funcao_a(){
	char ch[CHAVE+1];
	int uni;
	getchar();
	scanf("%s", ch);
	getchar();
	scanf("%d", &uni);
	if (STsearch(head,ch) == NULL){		// se a chave nao existir na arvore, criamos o produto
		if (uni < 0)	// se o valor for negativo, criamos o produto com 0 unidades
			uni = 0;
		atualiza_maximo(ch,uni);
		STinsert(&head, newItem(ch, uni));	
	}
	else if (uni < 0){
		int a = Key2(STsearch(head,ch));
		if ((a + (uni)) <= 0)
			Key2(STsearch(head,ch)) = 0;
		else
			Key2(STsearch(head,ch)) = a + uni;
		if (eq(max_chave, Key(STsearch(head,ch))))
			flag_del = 1;		//se diminuirmos o maximo, colocamos a flag a 1
	}
	
	else{
		int b = Key2(STsearch(head,ch));
		Key2(STsearch(head,ch)) = b + uni;
		atualiza_maximo(ch,b+uni);
	}
}


void funcao_l(){
	if (count(head) == 0)		//verifica se a arvore esta vazia
		return;
	else							//lista todas as chaves e unidades
		STsort(head,visitItem);		//associadas aos produtos existentes
}

void funcao_m(){
	if (count(head) == 0)	//verifica se a arvore esta vazia
		return;
	else if (flag_del == 1){		// caso a chave associada ao maximo tenha sido apagada
		maximo=0;
		strcpy(max_chave,"ffffffff");
		traverse(head);
		printf("%s %d\n",max_chave,maximo);
		flag_del = 0;		// voltamos a colocar a flag a 0
	}
	else
		printf("%s %d\n",max_chave,maximo);
}


void funcao_r(){
	char ch[CHAVE+1];
	getchar();
	scanf("%s", ch);
	if(STsearch(head,ch) == NULL)
		return;
	else {
		if (eq(max_chave,ch))	// caso a chave a apagar seja a do maximo
			flag_del = 1;	// indicamos que o maximo foi apagado
		STdelete(&head,ch);
	}
}

void funcao_x(){
	printf("%d\n",count(head));	// exibe o numero de produtos na arvore
	STfree(&head);
}



///////////////////////////////////////////////////////////////////////////////
//                                      Main                                 //
///////////////////////////////////////////////////////////////////////////////

int main(){
	char op;
	STinit(&head);
	while((op=getchar())!='x'){
		switch(op){
			case 'a':
			funcao_a();
			break;
			case 'l':
			funcao_l();
			break;
			case 'm':
			funcao_m();
			break;
			case 'r':
			funcao_r();
			break;
		}
	}
	funcao_x();
	return 0;
}