/*
Alumno: Montufar Perez Javier
Institucion: Escuela Superior de Computo (ESCOM)
Materia: Teoria computacional
Grupo: 2CV1
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LONGITUD_MAXIMA_CADENA 100
#define MAXSTACK 10
#define MAXLEN_CADENA_ENTRADA 20

//cola que contiene las instruciones del automata
typedef struct inst{
	char estadoI[3];
	char terminalI[3];
	char topeI[3];
	char estadoF[3];
	char topeF[3];
	struct inst* next;
}inst;

//pila del automata
typedef struct stack{
	int top;
	char entry[MAXSTACK]; //OJO es un arrgelo de caracteres
}stack;

inst* head = NULL;
char alfabetoEntrada[10]="";
char alfabetoPila[10]="";


void analizaAutomata(char* archivo);
void insert(char* eI, char* termI, char* topI, char* eF, char* topF);
inst* buscaInstr(char* eI, char* termI, char* topI, inst* partir);
void printInstr();
int StackEmpty(stack* s);
int StackFull(stack* s);
void Push(char item, stack* s);
char Pop(stack* s);
void CreateStack(stack* s);
char TopeDePila(stack* s);
void PushString(char* item, stack* s);
char* my_strcpy(char* destination, char* source);
void cortaCadena(char* cadena);
int Rama(char* estado, char* cadena, stack s, int* acumulador);
void printEstado(char* estado, char* cadena, stack s);
void toString(char* recibe, const char fuente);
int main(){
	char cadena_e[MAXLEN_CADENA_ENTRADA];
	int opc=1;
	stack pila;
	int acumulador=0;
	system("cls");
	char* archivo="a.txt"; //AQUI SE SELECCIONA EL ARCHIVO QUE SE QUIERE IMPLEMENTAR
	analizaAutomata(archivo);
	printf("\nAlfabeto de entrada: %s", alfabetoEntrada);
	printf("\nAlfabeto de pila: %s", alfabetoPila);
	
	do{
	printf("\n----------------");
	strcpy(cadena_e,"");
	CreateStack(&pila);
	Push('x',&pila);
	printf("\nIngresar una cadena:");
	scanf("%[^\n]", cadena_e);
	fflush(stdin);
	acumulador=0;
	Rama("q0",cadena_e,pila,&acumulador);
	if(acumulador){printf("Cadena aceptada por el automata pila.");}
	else{printf("Cadena no aceptada por el automata pila.");}
	
	//MENU
	printf("\nEscoja una opcion:\n1.Salir\n2.Ingresar otra cadena\nopc:");
	scanf("%d",&opc);
	fflush(stdin);
	}while(opc!=1);
	
}

/*OPERACIONES CON EL ARCHIVO*/
void analizaAutomata(char* archivo){
	FILE * flujo = fopen(archivo, "rb");
	char buffer[LONGITUD_MAXIMA_CADENA]; 
	char* aux;
	char* eI;char* termI;char* topI;char* eF;char* topF;
	char* simpila; char* simentr;
	
	/*LOS ALFABETOS TIENEN SIEMPRE EPSILON INDISTINTAMENTE*/
	strcat(alfabetoPila,"e");
	strcat(alfabetoEntrada,"e");
	
	if(flujo == NULL){perror("Error en la apertura del archivo con la configuracion del automata");}
	else{
		//leemos una linea
		while(fgets(buffer, LONGITUD_MAXIMA_CADENA, flujo)){
			strtok(buffer, "\n");
			printf("%s\n", buffer);
			if(strstr(buffer,"Delta:") != NULL){
				/*Analizamos sintacticamente la linea con una instruccion*/
				char* delimitadores=":,)=('";
				aux=strtok(buffer, delimitadores);
				eI=strtok(NULL,delimitadores);
				termI=strtok(NULL,delimitadores);
				topI=strtok(NULL, delimitadores);
				eF=strtok(NULL, delimitadores);
				topF=strtok(NULL, delimitadores);
				insert(eI, termI, topI, eF, topF);
				/*Ingresamos los simbolos de los alfabetos en las cadenas correspondientes*/
				simentr=termI;
				simpila=topI;
				if(strstr(alfabetoEntrada,simentr) == NULL){strcat(alfabetoEntrada,simentr);}
				if(strstr(alfabetoPila,simpila) == NULL){strcat(alfabetoPila,simpila);}
			}
		}
	}
		
}
/*OPERACIONES CON LA COLA*/
//Inserta l instruccion final de la cola.
void insert(char* eI, char* termI, char* topI, char* eF, char* topF){
	inst* aux=head;
	inst* newinst=(inst*)malloc(sizeof(inst));
	
	if(head==NULL){//Insertar al principio
		head=newinst;
		sprintf(newinst->estadoI,"%s",eI);
		fflush(stdin);
		sprintf(newinst->terminalI,"%s",termI);
		fflush(stdin);
		sprintf(newinst->topeI,"%s",topI);
		fflush(stdin);
		sprintf(newinst->estadoF,"%s",eF);
		fflush(stdin);
		sprintf(newinst->topeF,"%s",topF);
		fflush(stdin);
		newinst->next=NULL;
	}
	else{
		while(aux->next!=NULL){aux=aux->next;}
		aux->next=newinst;
		sprintf(newinst->estadoI,"%s",eI);
		fflush(stdin);
		sprintf(newinst->terminalI,"%s",termI);
		fflush(stdin);
		sprintf(newinst->topeI,"%s",topI);
		fflush(stdin);
		sprintf(newinst->estadoF,"%s",eF);
		fflush(stdin);
		sprintf(newinst->topeF,"%s",topF);
		fflush(stdin);
		newinst->next=NULL;	
	}
}

/*Devuelve el pointer a la instruccion que haya coincidencia
  en caso contrario regresamos nulo.*/
/*'partir' indica desde que posicion de la cola se va a buscar
  si se quiere partir de la cabecera se debe invocar con head*/
inst* buscaInstr(char* eI, char* termI, char* topI, inst* partir){
	inst* aux=partir;
	if(aux==NULL){
		return NULL;
		}
	else{
		while(aux!=NULL){
			if((strcmp(aux->estadoI,eI)==0)&&(strcmp(aux->terminalI,termI)==0)&&(strcmp(aux->topeI,topI)==0)){
				return aux;
			}
			aux=aux->next;
		}
		if(aux==NULL){return NULL;} //No hubo coincidencias
	}
}

void printInstr(){
	inst* aux = head;
	printf("\n-----------COLA DE INSTRUCCIONES-------------");
	while(aux!=NULL){
		printf("\n(%s,%s,%s)=(%s,%s)",aux->estadoI,aux->terminalI,aux->topeI,aux->estadoF,aux->topeF);
		aux=aux->next;
	}
}

void printI(inst* aux){
	if(aux==NULL)
		printf("\npuntero nulo.");
	else
		printf("\n(%s,%s,%s)=(%s,%s)",aux->estadoI,aux->terminalI,aux->topeI,aux->estadoF,aux->topeF);
}

/*OPERACIONES CON LA PILA*/

//Dice si esta vacia
int StackEmpty(stack* s){
	return s->top <= 0;
}
//Dice si esta llena
int StackFull(stack* s){
	return MAXSTACK <= s->top;
}
//Op. push
void Push(char item, stack* s){
	if(StackFull(s))
		printf("\nError, la pila esta llena\n");
	else
		s->entry[s->top++]=item; //x++ Postincremento.
}
//Op. pop, regresa por ref.
char Pop(stack* s){
	if(StackEmpty(s))
		return 'e';
	else
		return s->entry[--s->top]; //--x Predecremento
}
//Op. crea pila
void CreateStack(stack* s){
	s->top=0;
}
//Regresa el tope del pila sin sacar ningun item
char TopeDePila(stack* s){
	if(!StackEmpty(s)){
		char x;
		x=Pop(s);
		Push(x,s);
		return x;
	}
}

void PushString(char* item, stack* s){
	char caracter;
	int i=strlen(item)-1;
	for(i; i>=0; i--){
		if(item[i]!='e'){Push(item[i], s);} //Metemos a la pila un caracter si es diferente de epsilon
	}
}

/*OPERACIONES CON EL AUTOMATA*/
//Regresa '1' si la cadena enviada es aceptada por el automata '0' en caso contrario.
//OJO acumulador debe enviarse con 0.
//OJO el stack se envia con el 'x' el simbolo de fondo de pila.
//OJO estado se debe enviar "q0"
int Rama(char* estado, char* cadena, stack s, int* acumulador){
	inst* pointer_n=head;
	inst* pointer_e=head;
	char primer_car[MAXLEN_CADENA_ENTRADA];
	char car_top[MAXLEN_CADENA_ENTRADA];
	char estado_actual[MAXLEN_CADENA_ENTRADA];
	toString(primer_car,cadena[0]);
	toString(car_top, TopeDePila(&s));
	strcpy(estado_actual,estado);
	
	
	if((strcmp(cadena,"")==0) && StackEmpty(&s)){
		return 1;
	}
	else{
		pointer_n = buscaInstr(estado_actual,primer_car,car_top,pointer_n);
		pointer_e = buscaInstr(estado_actual,"e",car_top,pointer_e);
		while((pointer_n!=NULL || pointer_e!= NULL)){
			printEstado(estado, cadena, s);
			if(pointer_n!=NULL){
				/*Modificamos los valores que estamos monitoreando*/
				memset(estado_actual, '\0', sizeof(estado_actual));
				strncpy(estado_actual,pointer_n->estadoF,1);
				cortaCadena(cadena);
				Pop(&s);
				PushString(pointer_n->topeF, &s);
				/*Recursividad con las otras ramas*/
				*acumulador += Rama(estado_actual,cadena,s, acumulador);
			}
			if(pointer_e!=NULL){
				memset(estado_actual, '\0', sizeof(estado_actual));
				strncpy(estado_actual,pointer_e->estadoF,1);
				Pop(&s);
				PushString(pointer_e->topeF, &s);
				*acumulador += Rama(estado_actual,cadena,s, acumulador);
			}
			toString(primer_car,cadena[0]);
			toString(car_top, TopeDePila(&s));
			pointer_e->next;
			pointer_n->next;
			pointer_n = buscaInstr(estado_actual,primer_car,car_top,pointer_n);
			pointer_e = buscaInstr(estado_actual,"e",car_top,pointer_e);
		}
		return 0;
	}	
}


void printEstado(char* estado, char* cadena, stack s){
	char car_top[MAXLEN_CADENA_ENTRADA];
	char aux[MAXLEN_CADENA_ENTRADA]="";
	while(!StackEmpty(&s)){
		toString(car_top,Pop(&s));
		strcat(aux, car_top);
	}
	
	printf("(%s,%s,%s)\n",estado, cadena, aux);
}

char* my_strcpy(char* destination, char* source){
    char *start = destination;
	char aux=*source;
    while(aux != '\0')
    {
		//printf("\nLLEGO 4, aux:%c, *destination:%c", aux, *destination);
        *destination = aux;//DESBORDAMIENTO
        destination++;
        source++;
		aux=*source;
    }
    *destination = '\0'; // add '\0' at the end
    return start;
}

void cortaCadena(char* cadena){
	char aux[MAXLEN_CADENA_ENTRADA]="";
	int s=strlen(cadena);
	int i=0;int j=0;
	for(i=1; i<s;i++){
		aux[j]=cadena[i];
		j++;
		aux[j]='\0';
	}
	my_strcpy(cadena,aux);
}

void toString(char* recibe, const char fuente){
	char aux[2];
	aux[0]=fuente;
	aux[1]='\0';
	strcpy(recibe,aux);
}
