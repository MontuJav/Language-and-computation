/*
Alumno: Montufar Perez Javier
Institucion: Escuela Superior de Computo (ESCOM)
Materia: Teoria computacional
Grupo: 2CV1
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ERROR 1
#define BIEN 0
#define MAX_TAMANIO 50 //Maximo tamanio de la cadena

int Imprime_automata(char*); //Imprime todo el automata
int Analiza_automata(char*); //Pide una cadena al usuario y dice si el automata lo acepta
int Posiciones(char, char*); //Determina las posiciones a la derecha a mover en la tabla del automata
int Sintaxis_cadena(char*, char*); //Determina si los simbolos de la cadena de entrada 

int alfabeto=0;
int estados=0;

int main(){
	int SALIDA;
	char* archivo="config1.txt"; //AQUI SE SELECCIONA EL ARCHIVO QUE SE QUIERE IMPLEMENTAR
	
	system("cls");
	SALIDA = Imprime_automata(archivo);
	if(SALIDA!=ERROR)
		SALIDA = Analiza_automata(archivo);
	return SALIDA;
}

/*Imprime el automata y regresa BIEN si puede cargar el archivo, en caso contrario regresa ERROR.
*/
int Imprime_automata(char* archivo){
	FILE * flujo = fopen(archivo, "rb");
	char caracter;

	if(flujo == NULL){
		perror("Error en la apertura del archivo con la configuracion del automata");
		return ERROR;
	}
	while(feof(flujo)==0 && caracter != '	'){
		caracter=fgetc(flujo);
		printf("%c", caracter);
	}
	while(feof(flujo)==0 && caracter != ' '){
		caracter=fgetc(flujo);
		printf("%c", caracter);
		if(caracter!='	' && caracter!=' ')
			alfabeto++;
	}
	printf("\n-------------------------");
	while(feof(flujo)==0){
		caracter=fgetc(flujo);
		printf("%c", caracter);
		if(caracter=='\n')
			estados++;
	}
	
	fclose(flujo);
	return BIEN;
}

/* Pide una cadena al usuario y lo compara con el automata, además realiza las consideraciones necesarias 
   para que el usuario no ingrese simbolos que no se correspondan con los del alfabeto.*/
int Analiza_automata(char* archivo){
	FILE * flujo = fopen(archivo, "rb");
	char caracter;
	char automata[estados][alfabeto+2];
	
	int i=0,j=0;
	char cadena_e[MAX_TAMANIO];
	char estado_actual;
	char simbolos[10];
	int posicion;
	int p;
	
	//En esta seccion se analiza el automata y se organiza en una tabla con un formato especifico.
	while(feof(flujo)==0 && caracter != '	'){
		caracter=fgetc(flujo);
	}

	while(feof(flujo)==0 && caracter != '\n'){
		caracter=fgetc(flujo);
		if(caracter!='	' && caracter!='\r' && caracter!=' ' && caracter!='\n'){
			simbolos[i]=caracter; //LLenamos el arreglo, con todos los simbolos del alfabeto.
			i++;
		}
	}
	simbolos[i]='\0';
	i=0;
	caracter=fgetc(flujo);
	while(feof(flujo)==0){
		if(caracter=='\n'){
			j=0;
			i++;
		}
		else if(caracter=='	' || caracter=='\r' || caracter=='-'){}
		
		else if(caracter=='<'){ //Si se encuentra el caracter '<' se coloca el caracter 'i'.
			automata[i][j]='i';
			j++;
		}
		else if(caracter=='*'){ //Si se encuentra el caracter '*' se coloca el caracter 'f'.
			automata[i][j]='f';
			j++;
		}
		else if(caracter=='+'){ //Si se encuentra el caracter '+' se coloca el caracter 's'.
			automata[i][j]='s';
		}
		else if(caracter==' '){ //Si no se encuentra el caracter, se debió colocar un espacio ' ', e ese caso se coloca el caracter 'n'.
			automata[i][j]='n';
			j++;
		}
		else{
			automata[i][j]=caracter;
			j++;
		}
			
		caracter=fgetc(flujo);
	}
	
	fclose(flujo);
	
	//---------------------------------------------------------------
	//En esta parte pedimos una cadena al usuario y analizamos la cadena.
	int posicion_caracter,i2,j2;
	char opc;
	
	do{
		printf("\n-------------------------");//MENU
		printf("\n*Introducir una cadena de entrada(presione cualquier tecla).");
		printf("\n*Salir(presione 1).");
		printf("\nSeleccione una opcion: ");
		scanf("%c", &opc);
		fflush(stdin);
		strcpy(cadena_e,"");
		posicion_caracter=0;
		
		if(opc!='1'){
			cadena_e[0]='a'; //bandera, si el usuario ingresa una cadena vacia, i.e. un epsilon, esta 'a' servirá para reconocerlo.
			printf("\nIntroduzca una cadena:");
			scanf("%[^\n]", cadena_e);
			fflush(stdin);
			
			if(Sintaxis_cadena(cadena_e, simbolos)){
				for(i2=0;i2<estados;i2++){
					for(j2=0;j2<(alfabeto+2);j2++){
						if(automata[i2][j2]=='i')
							posicion_caracter=i2;
					}
				}
				
				if((cadena_e[0])!='a'){
					p=Posiciones(cadena_e[0], simbolos);
					estado_actual=automata[posicion_caracter][p];
					
					for(j=1; j<strlen(cadena_e); j++){
						for(i2=0;i2<estados;i2++){
							if(automata[i2][0]==estado_actual)
								posicion_caracter=i2;
						}
						p = Posiciones(cadena_e[j], simbolos);
						estado_actual = automata[posicion_caracter][p];
					}
				}
				else{ //Opcion cadena vacia, epsilon
					estado_actual=automata[posicion_caracter][0];
				}
				for(i2=0; i2<estados; i2++){
					if(automata[i2][0]==estado_actual)
						posicion_caracter=i2;
				}
				printf("El estado final es: %c\n", estado_actual);
				if((automata[posicion_caracter][alfabeto+1] == 'f') || (automata[posicion_caracter][alfabeto+1] == 's'))
					printf("Cadena aceptada por el AFD.\n");
				else
					printf("Cadena no aceptada por el AFD.\n");
					
			}
			else
				printf("Error en la cadena introducida hay un simbolo que no pertenece al alfabeto\n");
		}
	}while(opc!='1');
	
	return BIEN;
}

//Enviamos un caracter y regresa las posiciones necesarias a saltar a la derecha en la tabla.
int Posiciones(char caracter, char* simbolos){
	int index=0;
	int posicion=0;
	for(index=0;index<alfabeto;index++){
		if(simbolos[index]==caracter){
			posicion=index;
			index=alfabeto;
		}	
	}
	return posicion+1; //Se suma uno pues el arreglo bidimensional tiene una columna de mas
}

//Analiza si la cadena introducida tiene unicamente simbolos del alfabeto
//Regresa 1 si asi es y 0 en caso contrario
int Sintaxis_cadena(char* cadena, char* simbolos){
	int bandera=0,i,j;
	if(cadena[0]=='a')
		return 1;
	else{
		for(i=0;i<strlen(cadena);i++){
			for(j=0;j<alfabeto;j++){
				if(cadena[i] == simbolos[j]){
					bandera=1;
				}
			}
			if(!bandera)
				break;
			if(i=strlen(cadena))
				break;
			bandera=0;
		}
	}
	return bandera;
}





