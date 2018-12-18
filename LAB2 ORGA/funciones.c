#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cabecera.h"
#include "constantes.h"

//Tipo 0: Archivo existente.
//Tipo 1: Archivo a crear.
char* obtenerNombreArchivo(int tipoArchivo)
{
    FILE *archivo;
    char *nombreArchivo;
    nombreArchivo = malloc(sizeof(char)*MAX_STRING);
    int verificador = 0;
    while(verificador == 0){
        fflush( stdin );
        scanf("%s",nombreArchivo);
        if(tipoArchivo == 0){
            archivo = fopen(nombreArchivo,"r");
            if(archivo == NULL){
                printf("No se encontro el archivo!\nIngrese nuevamente el nombre\n");
            }
            else{
                fclose(archivo);
                verificador = 1;
            }
        }
        else{
            verificador = 1;
        }
    }
    return nombreArchivo;
}