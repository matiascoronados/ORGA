#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cabecera.h"
#include "constantes.h"


int programCounter = 0;
char* nombreRegistros[CANTIDAD_REGISTROS] = {"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7"
                                ,"$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"};



instruccion* crearInstruccion()
{
    instruccion *p_instruccion = malloc(sizeof(instruccion));
    p_instruccion->instruccion = malloc(sizeof(char)*MAX_STRING);
    p_instruccion->tipo = 0;
    p_instruccion->op = 0;
    p_instruccion->R1 = 0;
    p_instruccion->R2 = 0;
    p_instruccion->R3 = 0;
    p_instruccion->Valor_R1 = 0;
    p_instruccion->Valor_R2 = 0;
    p_instruccion->Valor_R3 = 0;
    return p_instruccion;
}

instruccionesArchivo* crearInstruccionesArchivo()
{
    instruccionesArchivo *p_listInst = malloc(sizeof(instruccionesArchivo));
    p_listInst->datos = malloc(sizeof(char*)*MAX_ARREGLO);
    for(int i = 0 ; i < MAX_ARREGLO;i++){
        p_listInst->datos[i] = malloc(sizeof(char)*MAX_STRING);
    }
     p_listInst->indice = 0;
    return p_listInst;
}


registros* crearRegistros()
{
    registros *p_registros = malloc(sizeof(registros));
    p_registros->datos = malloc(sizeof(int)*CANTIDAD_REGISTROS);
    for(int i = 0 ; i < CANTIDAD_REGISTROS; i++)
    {
        p_registros->datos[i] = 0;
    }
    return p_registros;
}

memoria* crearMemoriaDatos()
{
    memoria *p_memoria = malloc(sizeof(memoria));
    p_memoria->datos = malloc(sizeof(int*)*CANTIDAD_REGISTROS);
    for(int i = 0 ; i < CANTIDAD_REGISTROS; i++)
    {
        p_memoria->datos[i] = malloc(sizeof(int)*CAPACIDAD_MEMORIA);
        for(int j = 0 ; j < CAPACIDAD_MEMORIA; j++)
        {
            p_memoria->datos[i][j] = 0;
        }
    }
    return p_memoria;
}





//////////////////////////////////////////////////////////////////////////////////






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

instruccionesArchivo* leerArchivoEntrada()
{
    printf("Ingrese el nombre del archivo de instrucciones\n");
    char* nombreArchivo = solicitarNombreArchivo(0);
    instruccionesArchivo* contenido = contenidoArchivo(nombreArchivo);
    free(nombreArchivo);
    return contenido;
}



instruccionesArchivo* contenidoArchivo(char* nombreArchivo)
{
    FILE *archivo;
    archivo = fopen(nombreArchivo,"r");
    instruccionesArchivo *p_listInst = crearInstruccionesArchivo();
    int indice = p_listInst->indice;
    while(feof(archivo) == 0){
        fgets(p_listInst->datos[indice],MAX_STRING,archivo);
        //Si el elemento es un comentario, no se cosidera.
        if(strncmp(p_listInst->datos[indice],"#",1) == 0 || strncmp(p_listInst->datos[indice],"",1) == 0){
            indice--;
        }
        indice++;
    }
    //Se almacena el numero de instrucciones guardadas.
    p_listInst->indice = indice;
    fclose(archivo);
    return p_listInst;
}




void prediccionHazard()
{

}



int buscarLabbel(instruccionesArchivo *p_listInst,char *salto)
{
    int largoString = sizeof(salto);
    for(int i = 0 ; i < p_listInst->indice;i++){
        if(strncmp(p_listInst->datos[i],salto,largoString-1) == 0){
            return i;
        }
    }
    printf("Se detecto un LABBEL incorrecto\n");
    printf("Cerrando programa...\n");
    exit(EXIT_FAILURE);
    return -1;
}


int buscarDireccionMemoriaInstruccion (instruccionesArchivo *p_listInst, instruccion *p_inst, int indiceContenido)
{
    char instruccionString[100],*buffer;
    int indice = indiceContenido; 
    strcpy(&instruccionString[0],p_inst->instruccion);
    //Separaciones posibles entre los elementos de la instruccion
    char separacionA[] = " ";
    buffer = strtok( &instruccionString[0], separacionA );
    while( buffer != NULL  && indice != 0){
        buffer = strtok( NULL, separacionA);
        indice--;
    }
    return buscarLabbel(p_listInst,p_inst);
}



//OJO: No puede retornar instruccion.
//Necesario reconocer todos estos elementos antes de ingresar al IF real.
//Para analizar busqueda de HAZZARS en las 2 siguientes instrucciones.
//Si existe se agregan NOP y/o aplica FORWARDING.


instruccion* instructionFetch(instruccionesArchivo *p_listInst)
{
    instruccion* p_inst = crearInstruccion();
    strcpy(p_inst->instruccion,p_listInst->datos[programCounter]);
    p_inst->op = reconocerOperacion(p_inst);
    p_inst->tipo = reconocerTipo(p_inst);
    int tipo = p_inst->tipo;
    if(tipo == 0 || tipo == 1 || tipo == 3)
    {
        p_inst->R1 = buscarContenidoInstruccion(p_inst,1);
        p_inst->R2 = buscarContenidoInstruccion(p_inst,2);
        p_inst->R3 = buscarContenidoInstruccion(p_inst,3);
    }
    else if(tipo == 2)
    {
        p_inst->R1 = buscarContenidoInstruccion(p_inst,1);
        p_inst->R2 = buscarContenidoInstruccion(p_inst,2);
        p_inst->R3 = buscarDireccionMemoriaInstruccion(p_listInst,p_inst,3);
    }
    else if(tipo == 4)
    {
        p_inst->R1 = buscarDireccionMemoriaInstruccion(p_listInst,p_inst,1);
    }
    else // tipo == 5
    {
        p_inst->R1 = buscarContenidoInstruccion(p_inst,1);
    }
    programCounter++;
    return p_inst;
}


int instructionDetection(instruccion *p_instruc, registros *p_reg)
{
    int tipo = p_instruc->tipo;
    if(tipo == 0){
        //Operaciones: ADD, SUB, MUL
        int indice_R2;
        indice_R2 = p_instruc->R2;
        p_instruc->valor_R2 = p_reg->datos[indice_R2];
        p_instruc->valor_R3 = p_instruc->R3;
        return 1;
    }
    else if(tipo == 1 || tipo == 2)
    {
        //Operaciones: ADDI, SUBI, ADDIU, BEQ, BLT, BNE, BGT.
    }
    else if(tipo == 3)
    {
        if(p_instruc->op == 11)
        {
            //Operacion: SW
            int indice_R1;
            indice_R1 = p_instruc->R1;
            p_instruc->valor_R1 = p_reg->datos[indice_R1];
            p_instruc->valor_R2 = p_instruc->R2;
            return 1;
        }
        else
        {
        //Operacion: LW
            return 1;
        }
    }
    else if(tipo == 4)
    {
        //Operacion: J,JAL
    }
    else //if(tipo == 5) tipo JR
    {
        //Operacion: 
    }
    return 0;
}







//16 LABBEL.
int reconocerOperacion(instruccion *p_inst)
{
    char *operacion = p_inst->instruccion;
    if(strncmp(operacion,"add",3) == 0){
        return 0;}
    else if(strncmp(operacion,"sub",3) == 0){
        return 1;}
    else if(strncmp(operacion,"mul",3) == 0){
        return 2;}    
    else if(strncmp(operacion,"diopopopopv",3) == 0){ ///////////////////////////////////////////
        return 3;}  
    else if(strncmp(operacion,"addi",4) == 0){
        return 4;}  
    else if(strncmp(operacion,"subi",4) == 0){
        return 5;}  
    else if(strncmp(operacion,"addiu",4) == 0){
        return 6;}  
    else if(strncmp(operacion,"beq",4) == 0){
        return 7;}  
    else if(strncmp(operacion,"blt",4) == 0){
        return 8;}  
    else if(strncmp(operacion,"bne",4) == 0){
        return 9;}  
    else if(strncmp(operacion,"bgt",4) == 0){
        return 10;}  
    else if(strncmp(operacion,"lw",2) == 0){
        return 11;}
    else if(strncmp(operacion,"sw",2) == 0){
        return 12;}
    else if(strncmp(operacion,"j",1) == 0){
        return 13;}
    else if(strncmp(operacion,"jal",3) == 0){
        return 14;}
    else if(strncmp(operacion,"jr",2) == 0){
        return 15;}
    else{
        return 16;
    }
}

int reconocerTipo(instruccion *p_inst)
{
    if(p_inst->op <= 2){
        return 0;}
    else if(p_inst->op <= 6 && p:inst->op != 3){
        return 1;}
    else if(p_inst->op <= 10){
        return 2;}
    else if(p_inst->op <= 12){
        return 3;}
    else if(p_inst->op <= 14){
        return 4;}
    else{
        return 5;}//JR.
}

//Tipo contenido se indica si es un registro o immediate; (0) es registro; (1) es immediate; (2) es direccion.

int buscarContenidoInstruccion(instruccion *p_inst,int indiceContenido,int tipoContenido)
{
    char instruccionString[100],*buffer;
    int indice = indiceContenido;
    //Se copia la instruccion a una auxiliar.
    strcpy(&instruccionString[0],p_inst->instruccion);
    //Separaciones posibles entre los elementos de la instruccion
    char separacionA[] = " ";
    char separacionB[] = "(";
    //Se inicia el proceso.
    buffer = strtok( &instruccionString[0], separacionA );
    //Para las instrucciones de la forma OP R1, R2, R3.
    if(p_inst->tipo != 3)
    {
        while( buffer != NULL  && indice != 0){
            buffer = strtok( NULL, separacionA);
            indice--;
        }

        if(tipoContenido == 0){
            return reconocerRegistro(buffer);
        }
        else
        {
            return atoi(buffer)
        }
    }
    //Para las instrucciones de la forma OP R1, R2(R3)
    else{
        buffer = strtok( NULL, separacionA);
        while( buffer != NULL  && indice-1 != 0){
            buffer = strtok( NULL, separacionB);
            indice--;
        }
        //El indice indica que es un IMMEDIATE.
        if(indiceContenido == 2){
            return atoi(buffer);
        }
        //El indice indica que se trata de un registro
        else{
            return reconocerRegistro(buffer);
        }
    }
}