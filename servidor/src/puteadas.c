#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE 512 /* Definimos el tamaño del Buffer */

char* reemplazarMalaPalabra (char* cadena);
char* buscarPosicionCadena(char* cadena, char* caracteres);

void main(){
	char* cadena;
	
	cadena=(char*)malloc(BUF_SIZE*sizeof(char));
	cadena[0]='\0';
	
	printf("Escribe algo: ");
	gets(cadena);

	printf("Cadena sin malas palabras: %s\n", reemplazarMalaPalabra(cadena));
}

char* reemplazarMalaPalabra (char* cadena){
	FILE *archivo;
	archivo = fopen("diccionario", "r");
	
	char* cadena2;
	char* caracteres;
	
	cadena2=(char*)malloc(BUF_SIZE*sizeof(char));
	caracteres=(char*)malloc(BUF_SIZE*sizeof(char));
	
	strcpy(cadena2, cadena);
	
	while (feof(archivo) == 0)
 		{
 		fgets(caracteres,50,archivo);
 		
 		caracteres[strlen(caracteres)-1]='\0';
  		 
 		if(strstr(cadena2, caracteres)!=NULL){
 			strcpy(cadena2, buscarPosicionCadena(cadena2, caracteres));
			}
			
		caracteres[0]='\n';
 		}
	
	fclose(archivo);
	return cadena2;
}

char* buscarPosicionCadena(char* cadena, char* caracteres){
	int i=0, e=0, coincidencias=0;
	char* cadena2;
	
	cadena2=(char*)malloc(BUF_SIZE*sizeof(char));
	
	strcpy(cadena2, cadena);
	
	for(i=0; i<strlen(cadena2); i++){
		if(cadena[i] == caracteres[0]){
			coincidencias++;
			for(e=1; e<strlen(caracteres); e++){
				if(cadena[i+e] == caracteres[e]){
					coincidencias++;
				}	
			}
		
		if (coincidencias == strlen(caracteres)){
			for(e=1; e<strlen(caracteres); e++){
				cadena2[i+e] = '*';
			}
		}
		coincidencias  = 0;
		
		}
	}
	
			return cadena2;
}
