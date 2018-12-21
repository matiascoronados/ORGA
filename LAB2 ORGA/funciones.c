#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cabecera.h"
#include "constantes.h"


int numeroCiclo;
int contadorPrograma;
char* nombreRegistros[CANTIDAD_REGISTROS] = {"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7"
                                ,"$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"};
reporte *punteroReporte;


forward* crearForward()
{
    forward *p_forward = malloc(sizeof(forward));
    p_forward->poseeHazzard = 0;
    p_forward->posicion = 0;
    p_forward->numInstruccion = 0;
    p_forward->registroProblema = 0;
    return p_forward;
}


instruccion* crearInstruccion(int tipoInstruccion)
{
    instruccion *p_instruccion;
    if(tipoInstruccion == -1)
    {
        p_instruccion = malloc(sizeof(instruccion));
        p_instruccion->instruccion = NULL;
        p_instruccion->tipo = -1;
        p_instruccion->op = -1;
        p_instruccion->R1 = -1;
        p_instruccion->R2 = -1;
        p_instruccion->R3 = -1;
        p_instruccion->valor_R1 = -1;
        p_instruccion->valor_R2 = -1;
        p_instruccion->valor_R3 = -1;
        p_instruccion->datosForward = NULL;
    }
    else
    {
        p_instruccion = malloc(sizeof(instruccion));
        p_instruccion->instruccion = malloc(sizeof(char)*MAX_STRING);
        p_instruccion->tipo = 0;
        p_instruccion->op = 0;
        p_instruccion->R1 = 0;
        p_instruccion->R2 = 0;
        p_instruccion->R3 = 0;
        p_instruccion->valor_R1 = 0;
        p_instruccion->valor_R2 = 0;
        p_instruccion->valor_R3 = 0;
        p_instruccion->datosForward = crearForward();
    }
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

pipeline* crearPipeline()
{
    pipeline *p_pipe = malloc(sizeof(pipeline));
    p_pipe->ID = NULL;
    p_pipe->IF = NULL;
    p_pipe->EX = NULL;
    p_pipe->MEM = NULL;
    p_pipe->WB = NULL;
    return p_pipe;
}

buffer* crearBufferIntermedios()
{
    buffer *p_buffer = malloc(sizeof(buffer));
    p_buffer->ID_EX = NULL;
    p_buffer->EX_MEM = NULL;
    p_buffer->MEM_WB = NULL;
    return p_buffer;
}


reporte* crearReporteErrores()
{
    reporte *p_reporte = malloc(sizeof(reporte));
    p_reporte->listaError = malloc(sizeof(error*)*MAX_ERRORES);
    for(int i = 0 ; i < MAX_ERRORES ;i++)
    {
        p_reporte->listaError[i] = malloc(sizeof(error));
    }
    p_reporte->indiceError = 0;
    return p_reporte;
}


void agregarError(int tipoError)
{
    int indice = punteroReporte->indiceError;
    error *p_error = punteroReporte->listaError[indice];
    p_error->numeroCiclo = numeroCiclo+2;
    p_error->tipoError = tipoError;
    if(tipoError == 0 || tipoError == 1 || tipoError == 2)
    {
        p_error->numeroInstruccion = contadorPrograma+1;
    }
    else
    {
        p_error->numeroInstruccion = contadorPrograma;
    }
    punteroReporte->indiceError = indice + 1;
}


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
    //printf("Ingrese el nombre del archivo de instrucciones\n");
    //char* nombreArchivo = obtenerNombreArchivo(0);
    instruccionesArchivo* contenido = contenidoArchivo("entradaA.txt");
    //free(nombreArchivo);
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
        if(strncmp(p_listInst->datos[indice],"#",1) == 0 || strncmp(p_listInst->datos[indice],"\n",2) == 0){
            indice--;
        }
        indice++;
    }
    //Se almacena el numero de instrucciones guardadas.
    p_listInst->indice = indice;
    fclose(archivo);
    return p_listInst;
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
    return buscarLabbel(p_listInst,buffer);
}



//OJO: No puede retornar instruccion.
//Necesario reconocer todos estos elementos antes de ingresar al IF real.
//Para analizar busqueda de HAZZARS en las 2 siguientes instrucciones.
//Si existe se agregan NOP y/o aplica FORWARDING.


instruccion* conformarInstruccion(instruccionesArchivo *p_listInst, int contadorPrograma)
{
    instruccion* p_inst = crearInstruccion(0);
    strcpy(p_inst->instruccion,p_listInst->datos[contadorPrograma]);
    p_inst->op = reconocerOperacion(p_inst);
    p_inst->tipo = reconocerTipo(p_inst);
    int tipo = p_inst->tipo;
    if(tipo == 0)
    {
        p_inst->R1 = buscarContenidoInstruccion(p_inst,1,0);
        p_inst->R2 = buscarContenidoInstruccion(p_inst,2,0);
        p_inst->R3 = buscarContenidoInstruccion(p_inst,3,0);
    }
    else if(tipo == 1)
    {
        p_inst->R1 = buscarContenidoInstruccion(p_inst,1,0);
        p_inst->R2 = buscarContenidoInstruccion(p_inst,2,0);
        p_inst->R3 = buscarContenidoInstruccion(p_inst,3,1);    
    }
    else if(tipo == 2)
    {
        p_inst->R1 = buscarContenidoInstruccion(p_inst,1,0);
        p_inst->R2 = buscarContenidoInstruccion(p_inst,2,0);
        p_inst->R3 = buscarDireccionMemoriaInstruccion(p_listInst,p_inst,3);
    }
    else if(tipo == 3)
    {
        p_inst->R1 = buscarContenidoInstruccion(p_inst,1,0);
        p_inst->R2 = buscarContenidoInstruccion(p_inst,2,1);
        p_inst->R3 = buscarContenidoInstruccion(p_inst,3,0);
    }
    else if(tipo == 4)
    {
        p_inst->R1 = buscarDireccionMemoriaInstruccion(p_listInst,p_inst,1);
    }
    else if(tipo == 5)
    {
        p_inst->R1 = buscarContenidoInstruccion(p_inst,1,0);
    }
    else if(tipo == 6)
    {
        p_inst->R1 = buscarContenidoInstruccion(p_inst,1,0);
        p_inst->R2 = buscarContenidoInstruccion(p_inst,2,0);
    }
    else
    {
        return NULL;
    }
    return p_inst;
}





void imprimirInstruccion(instruccion *p_inst)
{
    printf("Instruccion = %s\n",p_inst->instruccion);
    printf("Tipo = %d\n",p_inst->tipo);
    printf("OP = %d\n",p_inst->op);
    printf("R1 = %d\n",p_inst->R1);
    printf("R2 = %d\n",p_inst->R2);
    printf("R3 = %d\n\n",p_inst->R3);
}

void iniciarPipeline(instruccionesArchivo *p_listInst,memoria *p_mem, registros *p_reg)
{
    pipeline *p_pipeline = crearPipeline();
    buffer *p_buffer = crearBufferIntermedios();
    contadorPrograma = 0;
    numeroCiclo = 0;
    int saltoLinea;
    int numeroInstrucciones = p_listInst->indice;
    instruccion *NOP = crearInstruccion(-1);
    while(contadorPrograma != numeroInstrucciones)
    {
        instruccion *p_inst;
        p_inst = conformarInstruccion(p_listInst,contadorPrograma);
        if(p_inst != NULL)
        {
            if(existeHazzardDatos(p_inst,p_pipeline) == 2)
            {
                agregarError(2);
                agregarPipeline(p_pipeline,NOP,p_buffer);
                saltoLinea = ejecutarPipeline(p_pipeline,p_buffer,p_mem,p_reg);
                contadorPrograma--;
            }
            else if(existeHazzardControl(p_inst) == 2)
            {
                agregarError(1);
                agregarPipeline(p_pipeline,p_inst,p_buffer);
                saltoLinea = ejecutarPipeline(p_pipeline,p_buffer,p_mem,p_reg);
                agregarPipeline(p_pipeline,NOP,p_buffer);
                saltoLinea = ejecutarPipeline(p_pipeline,p_buffer,p_mem,p_reg);
                agregarPipeline(p_pipeline,NOP,p_buffer);
                saltoLinea = ejecutarPipeline(p_pipeline,p_buffer,p_mem,p_reg);//Obtengo nueva direccion.
                if(saltoLinea != 0)
                {
                    contadorPrograma = saltoLinea;
                }
            }
            else if(existeHazzardControl(p_inst) == 1)
            {
                agregarError(0);
                agregarPipeline(p_pipeline,p_inst,p_buffer);
                saltoLinea = ejecutarPipeline(p_pipeline,p_buffer,p_mem,p_reg);
                agregarPipeline(p_pipeline,NOP,p_buffer);
                saltoLinea = ejecutarPipeline(p_pipeline,p_buffer,p_mem,p_reg);//Obtengo nueva direccion.
                if(saltoLinea != 0)
                {
                    contadorPrograma = saltoLinea;
                }            
            }
            else
            {
                agregarPipeline(p_pipeline,p_inst,p_buffer);
                saltoLinea = ejecutarPipeline(p_pipeline,p_buffer,p_mem,p_reg);
            }
            contadorPrograma++;            
        }
        else
        {
            contadorPrograma++;
        }
    }
    contadorPrograma--;
    agregarPipeline(p_pipeline,NOP,p_buffer);
    saltoLinea = ejecutarPipeline(p_pipeline,p_buffer,p_mem,p_reg);
    agregarPipeline(p_pipeline,NOP,p_buffer);
    saltoLinea = ejecutarPipeline(p_pipeline,p_buffer,p_mem,p_reg);
    agregarPipeline(p_pipeline,NOP,p_buffer);
    saltoLinea = ejecutarPipeline(p_pipeline,p_buffer,p_mem,p_reg);
    agregarPipeline(p_pipeline,NOP,p_buffer);
    saltoLinea = ejecutarPipeline(p_pipeline,p_buffer,p_mem,p_reg);
    agregarPipeline(p_pipeline,NOP,p_buffer);
    saltoLinea = ejecutarPipeline(p_pipeline,p_buffer,p_mem,p_reg);
}





int ejecutarPipeline(pipeline *p_pipeline, buffer *p_buffer,memoria *p_mem,registros *p_reg)
{
    /*
    printf("---------------------------------------------------\nCICLO = %d\n",numeroCiclo);
    if(p_pipeline->IF != NULL)
    {
        printf("Entro a IF: %s\n",p_pipeline->IF->instruccion);
        printf("V_R1 = %d\n",p_pipeline->IF->valor_R1);
        printf("R1 = %d\n",p_pipeline->IF->R1);
        printf("V_R2 = %d\n",p_pipeline->IF->valor_R2);
        printf("R2 = %d\n",p_pipeline->IF->R2);
        printf("V_R3 = %d\n",p_pipeline->IF->R3);
        printf("R3 = %d\n\n",p_pipeline->IF->R3);
    }
    if(p_pipeline->ID != NULL)
    {
        printf("Entro a ID: %s\n",p_pipeline->ID->instruccion);
        printf("V_R1 = %d\n",p_pipeline->ID->valor_R1);
        printf("V_R2 = %d\n",p_pipeline->ID->valor_R2);
        printf("V_R3 = %d\n\n",p_pipeline->ID->valor_R3);
    }
    if(p_buffer->ID_EX != NULL)
    {
        printf("Entro a ID_EX: %s\n",p_buffer->ID_EX->instruccion);
    }
    if(p_pipeline->EX != NULL)
    {
        printf("Entro a EX: %s\n",p_pipeline->EX->instruccion);
        printf("V_R1 = %d\n",p_pipeline->EX->valor_R1);
        printf("V_R2 = %d\n",p_pipeline->EX->valor_R2);
        printf("V_R3 = %d\n\n",p_pipeline->EX->valor_R3);
    }
    if(p_buffer->EX_MEM != NULL)
    {
        printf("Entro a EX_MEM: %s\n",p_buffer->EX_MEM->instruccion);
    }    
    if(p_pipeline->MEM != NULL)
    {
        printf("Entro a MEM: %s\n",p_pipeline->MEM->instruccion);
        printf("V_R1 = %d\n",p_pipeline->MEM->valor_R1);
        printf("V_R2 = %d\n",p_pipeline->MEM->valor_R2);
        printf("V_R3 = %d\n\n",p_pipeline->MEM->valor_R3);        
    }        
    if(p_buffer->MEM_WB != NULL)
    {
        printf("Entro a MEM_WB: %s\n",p_buffer->MEM_WB->instruccion);
    }        
    if(p_pipeline->WB != NULL)
    {
        printf("Entro a WB: %s\n",p_pipeline->WB->instruccion);
        printf("V_R1 = %d\n",p_pipeline->WB->valor_R1);
        printf("V_R2 = %d\n",p_pipeline->WB->valor_R2);
        printf("V_R3 = %d\n\n",p_pipeline->WB->valor_R3);        
    }      
    printf("---------------------------------------------------\n");
    */
    int saltoLineaID, saltoLineaEX;
    numeroCiclo++;

    writeBack(p_pipeline->WB,p_reg);

    memoryAccess(p_pipeline->MEM,p_mem);
    p_buffer->MEM_WB = p_pipeline->MEM;

    saltoLineaEX = execute(p_pipeline->EX); //Obtengo direccion si es BEQ
    p_buffer->EX_MEM = p_pipeline->EX;

    saltoLineaID = instructionDetection(p_pipeline->ID,p_reg,p_buffer);//Obtengo direccion si es 
    p_buffer->ID_EX = p_pipeline->ID;

    instructionFetch(p_pipeline->IF);

    if(saltoLineaID != 0)
    {
        return saltoLineaID;
    }
    else if(saltoLineaEX != 0)
    {
        return saltoLineaEX;
    }
    else
    {
        return 0;
    }
}


void instructionFetch(instruccion *p_inst)
{
    if(p_inst != NULL)
    {
        if(p_inst->op != -1)
        {
            p_inst->op = reconocerOperacion(p_inst);
            p_inst->tipo = reconocerTipo(p_inst);
        }        
    }

}

//Asignio valores.

int instructionDetection(instruccion *p_instruc, registros *p_reg, buffer *p_buffer)
{
    if(p_instruc != NULL)
    {
        if(p_instruc->op != -1)
        {
            int tipo = p_instruc->tipo;
            int saltoLinea = 0;
            forward *datosForward = p_instruc->datosForward;
            if(tipo == 0){
                //Operaciones: ADD, SUB, MUL
                p_instruc->valor_R2 = p_reg->datos[p_instruc->R2];
                p_instruc->valor_R3 = p_reg->datos[p_instruc->R3];
                if(datosForward->poseeHazzard != 0)
                {
                    modificarDatos(p_instruc,p_buffer);
                }
            }
            else if(tipo == 1)
            {
                //Operaciones: ADDI, SUBI, ADDIU
                p_instruc->valor_R2 = p_reg->datos[p_instruc->R2];
                p_instruc->valor_R3 = p_instruc->R3;
                if(datosForward->poseeHazzard != 0)
                {
                    modificarDatos(p_instruc,p_buffer);
                }
            }
            else if(tipo == 2)
            {
                p_instruc->valor_R1 = p_reg->datos[p_instruc->R1];
                p_instruc->valor_R2 = p_reg->datos[p_instruc->R2];
                if(datosForward->poseeHazzard != 0)
                {
                    modificarDatos(p_instruc,p_buffer);
                }
            }
            else if(tipo == 3)
            {
                if(p_instruc->op == 11)
                {
                    //Operacion: SW
                    p_instruc->valor_R1 = p_reg->datos[p_instruc->R1];
                    p_instruc->valor_R2 = p_instruc->R2;
                    if(datosForward->poseeHazzard != 0)
                    {
                        modificarDatos(p_instruc,p_buffer);
                    }

                }
                else
                {
                    //Operacion: LW
                    p_instruc->valor_R2 = p_instruc->R2;
                    if(datosForward->poseeHazzard != 0)
                    {
                        modificarDatos(p_instruc,p_buffer);
                    }
                }
            }
            else if(tipo == 4)
            {
                saltoLinea = p_instruc->R1;
            }
            else //if(tipo == 5) tipo JR
            {
                p_instruc->valor_R1 = p_reg->datos[p_instruc->R1];
                if(datosForward->poseeHazzard != 0)
                {
                    modificarDatos(p_instruc,p_buffer);
                }                 
            }
            return saltoLinea;        
        }
        else
        {
            return 0;
        }        
    }
    else
    {
        return 0;
    }

}


int execute(instruccion *p_instruc)
{
    if(p_instruc != NULL)
    {
        if(p_instruc->tipo != -1)
        {
            int saltoLinea = 0;
            if(p_instruc->op == 0 || p_instruc->op == 4)
            {
                p_instruc->valor_R1 = p_instruc->valor_R2 + p_instruc->valor_R3;
            }
            else if(p_instruc->op == 1 || p_instruc->op == 5)
            {
                p_instruc->valor_R1 = p_instruc->valor_R2 - p_instruc->valor_R3;
            }
            else if(p_instruc->op == 2)
            {
                p_instruc->valor_R1 = p_instruc->valor_R2 * p_instruc->valor_R3;
            }
            else if(p_instruc->op == 3)
            {
                p_instruc->valor_R1 = p_instruc->valor_R1 + p_instruc->valor_R1 * 0;
            }
            else if(p_instruc->op == 6)
            {
                p_instruc->valor_R1 = p_instruc->valor_R2 + fabs(p_instruc->valor_R3);
            }
            else if(p_instruc->op == 7)
            {
                if(p_instruc->valor_R1 == p_instruc->valor_R2)
                {
                    saltoLinea = p_instruc->R3;
                }
            }
            else if(p_instruc->op == 8)
            {
                if(p_instruc->valor_R1 < p_instruc->valor_R2)
                {
                    saltoLinea = p_instruc->R3;
                }
            }
            else if(p_instruc->op == 9)
            {
                if(p_instruc->valor_R1 == p_instruc->valor_R2)
                {
                    saltoLinea = p_instruc->R3;
                }
            }
            else if(p_instruc->op == 10)
            {
                if(p_instruc->valor_R1 > p_instruc->valor_R2)
                {
                    saltoLinea = p_instruc->R3;
                }
            }
            else if(p_instruc->op == 11 || p_instruc->op == 12)
            {
                p_instruc->valor_R2 = calcularDireccion(p_instruc->R2);
            }
            else if(p_instruc->op == 15)
            {
                saltoLinea = p_instruc->R1;
            }
            return saltoLinea;
        }
        else
        {
            return 0;
        }        
    }
    else
    {
        return 0;
    }

}

void memoryAccess(instruccion *p_instruc,memoria *p_mem)
{
    if(p_instruc != NULL)
    {
        if(p_instruc->tipo != -1)
        {
            if(p_instruc->op == 11){
                //Operacion LW
                int indiceRegistro = p_instruc->R3;
                int indiceElemento = p_instruc->valor_R2;
                p_instruc->valor_R1 = p_mem->datos[indiceRegistro][indiceElemento];
            }
            else if(p_instruc->op == 12){
                //Operacion SW
                int indiceRegistro = p_instruc->R3;
                int indiceElemento = p_instruc->valor_R2;
                p_mem->datos[indiceRegistro][indiceElemento] = p_instruc->valor_R1;
            }        
        }        
    }
}


void writeBack(instruccion *p_instruc,registros *p_reg)
{
    if(p_instruc != NULL)
    {
        int tipo = p_instruc->tipo;
        if(tipo != -1)
        {
            if(tipo != 3 && tipo < 7 )
            {
                p_reg->datos[p_instruc->R1] = p_instruc->valor_R1;
            }
            else if(tipo == 12)
            {
                p_reg->datos[p_instruc->R1] = p_instruc->valor_R1;
            }
        }        
    }
}









void modificarDatos(instruccion *p_inst,buffer *p_buffer)
{
    forward *datosForward = p_inst->datosForward;
    if(datosForward->posicion == 0)
    {                                                                                                                                                                                                                   
        if(p_inst->tipo == 2)
        {
            if(datosForward->registroProblema == 1)
            {
                p_inst->valor_R1 = p_buffer->EX_MEM->valor_R1;
                agregarError(4);

                if(datosForward->multipleForward == 1)
                {
                    p_inst->valor_R2 = p_buffer->MEM_WB->valor_R1;
                    agregarError(5);
                }
            }
            else
            {
                p_inst->valor_R2 = p_buffer->EX_MEM->valor_R1;
                agregarError(4);
                if(datosForward->multipleForward == 1)
                {
                    p_inst->valor_R1 = p_buffer->MEM_WB->valor_R1;
                    agregarError(5);
                }
            }
        }
        else if(p_inst->tipo == 5)
        {
            if(datosForward->registroProblema == 1)
            {
                p_inst->valor_R1 = p_buffer->EX_MEM->valor_R1;
                agregarError(4);
            }
        }
        else
        {
            if(datosForward->registroProblema == 2)
            {
                p_inst->valor_R2 = p_buffer->EX_MEM->valor_R1;
                agregarError(4);

                if(datosForward->multipleForward == 1)
                {
                    p_inst->valor_R3 = p_buffer->MEM_WB->valor_R1;
                    agregarError(5);
                }
            }
            else
            {
                p_inst->valor_R3 = p_buffer->EX_MEM->valor_R1;
                agregarError(4);
                if(datosForward->multipleForward == 1)
                {
                    p_inst->valor_R2 = p_buffer->MEM_WB->valor_R1;
                    agregarError(5);
                }
            }
        }
    }
    else
    {

        if(p_inst->tipo == 2)
        {
            if(datosForward->registroProblema == 1)
            {
                p_inst->valor_R1 = p_buffer->MEM_WB->valor_R1;
                agregarError(5);
            }
            else
            {
                p_inst->valor_R2 = p_buffer->MEM_WB->valor_R1;
                agregarError(5);

            }
        }
        else if(p_inst->tipo == 5)
        {
            if(datosForward->registroProblema == 1)
            {
                p_inst->valor_R1 = p_buffer->MEM_WB->valor_R1;
                agregarError(5);
            }
        }
        else
        {
            if(datosForward->registroProblema == 2)
            {
                p_inst->valor_R2 = p_buffer->MEM_WB->valor_R1;
                agregarError(5);
            }
            else
            {
                p_inst->valor_R3 = p_buffer->EX_MEM->valor_R1;
                agregarError(4);
            }
        }
    }
}


int calcularDireccion(int indice)
{
    double direccion = indice/4.0;
    direccion = fabs(direccion);
    return (int)direccion;
}



void agregarPipeline(pipeline *p_pipeline,instruccion *p_inst,buffer *p_buffer)
{
    p_pipeline->WB = NULL;
    p_pipeline->WB = p_pipeline->MEM;
    p_pipeline->MEM = p_pipeline->EX;
    p_pipeline->EX = p_pipeline->ID;
    p_pipeline->ID = p_pipeline->IF;
    p_pipeline->IF = p_inst;
}



int existeHazzardControl(instruccion *p_inst)
{
    if(p_inst->tipo == 2 || p_inst->tipo == 5){
        return 2;
    }
    else if(p_inst->tipo == 4)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int existeHazzardDatos(instruccion *p_inst, pipeline *p_pipe)
{
    //Se agregan NOP y agrega la instruccion.
    instruccion *instIF = p_pipe->IF;
    instruccion *instID = p_pipe->ID;
    
    int existeHazzard = 0;
    if(instIF != NULL)
    {
        int tipoIF = instIF->tipo;
        int opIF = instIF->op;
        if(tipoIF != -1)
        {
            if(tipoIF == 0 || tipoIF == 1)
            {   
                if(p_inst->tipo == 2)
                {
                    if(p_inst->R1 == instIF->R1)
                    {
                        agregarDatosHazzard(p_inst,0,1,1);
                        existeHazzard = 1;
                    }
                    if(p_inst->R2 == instIF->R1)
                    {
                        agregarDatosHazzard(p_inst,0,1,2);
                        existeHazzard = 1;
                    }                    
                }

                else if(p_inst->tipo == 5)
                {
                    if(p_inst->R1 == instIF->R1)
                    {
                        agregarDatosHazzard(p_inst,0,1,1);
                        existeHazzard = 1;
                    }
                }

                else
                {
                    if(p_inst->R2 == instIF->R1)
                    {
                        agregarDatosHazzard(p_inst,0,1,2);
                        existeHazzard = 1;
                    }
                    else if(p_inst->R3 == instIF->R1)
                    {
                        agregarDatosHazzard(p_inst,0,1,3);
                        existeHazzard = 1;
                    }                    
                }
            }
            else if(tipoIF == 3)
            {
                if(p_inst->tipo == 2)
                {
                    if(p_inst->R1 == instIF->R1)
                    {
                        existeHazzard = 2;
                    }
                    else if(p_inst->R2 == instIF->R1)
                    {
                        existeHazzard = 2;
                    }                    
                }
                else if(p_inst->tipo == 5)
                {
                    if(p_inst->R1 == instIF->R1)
                    {
                        existeHazzard = 2;
                    }                    
                }
                else
                {
                    if(p_inst->R3 == instIF->R1)
                    {
                        existeHazzard = 2;
                    }
                    else if(p_inst->R2 == instIF->R1)
                    {
                        existeHazzard = 2;
                    }                    
                }

            }
        }
    }
    if(instID != NULL)
    {   
        int tipoID = instID->tipo;
        int opID = instID->op;
        if(tipoID != -1)
        {
            if(tipoID == 0 || tipoID == 1)
            {   
                if(p_inst->tipo == 2)
                {
                    if(p_inst->R1 == instID->R1)
                    {
                        agregarDatosHazzard(p_inst,1,2,1);
                        existeHazzard = 1;
                    }
                    if(p_inst->R2 == instID->R1)
                    {
                        agregarDatosHazzard(p_inst,1,2,2);
                        existeHazzard = 1;
                    }                    
                }

                else if(p_inst->tipo == 5)
                {
                    if(p_inst->R1 == instID->R1)
                    {
                        agregarDatosHazzard(p_inst,1,2,1);
                        existeHazzard = 1;
                    }
                }

                else
                {
                    if(p_inst->R2 == instID->R1)
                    {
                        agregarDatosHazzard(p_inst,1,2,2);
                        existeHazzard = 1;
                    }
                    else if(p_inst->R3 == instID->R1)
                    {
                        agregarDatosHazzard(p_inst,1,2,3);
                        existeHazzard = 1;
                    }                    
                }
            }
            else if(tipoID == 3)
            {
                if(p_inst->tipo == 2)
                {
                    if(p_inst->R1 == instID->R1)
                    {
                        if(existeHazzard == 1)
                        {
                            p_inst->datosForward->multipleForward = 1;
                        }  
                        else
                        {
                            agregarDatosHazzard(p_inst,1,2,1);
                        }                        
                    }
                    else if(p_inst->R2 == instID->R1)
                    {
                        if(existeHazzard == 1)
                        {
                            p_inst->datosForward->multipleForward = 1;
                        }  
                        else
                        {
                            agregarDatosHazzard(p_inst,1,2,2);
                        }    
                    }                    
                }
                else if(p_inst->tipo == 5)
                {
                    if(p_inst->R1 == instID->R1)
                    {
                        agregarDatosHazzard(p_inst,1,2,1);
                        existeHazzard = 1;
                    }                    
                }
                else
                {
                    if(p_inst->R3 == instID->R1)
                    {
                        if(existeHazzard == 1)
                        {
                            p_inst->datosForward->multipleForward = 1;
                        }              
                        else
                        {
                            agregarDatosHazzard(p_inst,1,2,3);
                            existeHazzard = 1;
                        }          

                    }
                    else if(p_inst->R2 == instID->R1)
                    {
                        if(existeHazzard == 1)
                        {
                            p_inst->datosForward->multipleForward = 1;
                        }              
                        else
                        {        
                            agregarDatosHazzard(p_inst,1,2,2);
                            existeHazzard = 1;
                        }
                    }                    
                }
            }
        }        
    }
    return existeHazzard;
}


void agregarDatosHazzard(instruccion *p_inst,int punto,int posInst,int regProblema)
{
    p_inst->datosForward->poseeHazzard = 1;
    p_inst->datosForward->posicion = punto;
    p_inst->datosForward->numInstruccion = posInst;
    p_inst->datosForward->registroProblema = regProblema;
}


//16 LABBEL.
int reconocerOperacion(instruccion *p_inst)
{
    char *operacion = p_inst->instruccion;
    if(strncmp(operacion,"add",3) == 0 && strncmp(operacion,"addi",4) != 0 ){
        return 0;}
    else if(strncmp(operacion,"sub",3) == 0 && strncmp(operacion,"subi",4) != 0){
        return 1;}
    else if(strncmp(operacion,"mul",3) == 0){
        return 2;}    
    else if(strncmp(operacion,"div",3) == 0){
        return 3;}  
    else if(strncmp(operacion,"addi",4) == 0 && strncmp(operacion,"addiu",5) != 0){
        return 4;}  
    else if(strncmp(operacion,"subi",4) == 0){
        return 5;}  
    else if(strncmp(operacion,"addiu",5) == 0){
        return 6;}  
    else if(strncmp(operacion,"beq",3) == 0){
        return 7;}  
    else if(strncmp(operacion,"blt",3) == 0){
        return 8;}  
    else if(strncmp(operacion,"bne",3) == 0){
        return 9;}  
    else if(strncmp(operacion,"bgt",3) == 0){
        return 10;}  
    else if(strncmp(operacion,"lw",2) == 0){
        return 11;}
    else if(strncmp(operacion,"sw",2) == 0){
        return 12;}
    else if(strncmp(operacion,"j",1) == 0 && strncmp(operacion,"jal",3) != 0 && strncmp(operacion,"jr",2) != 0){
        return 13;}
    else if(strncmp(operacion,"jal",3) == 0 && strncmp(operacion,"jr",2) != 0){
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
    else if(p_inst->op <= 6 && p_inst->op != 3){
        return 1;}
    else if(p_inst->op <= 10){
        return 2;}
    else if(p_inst->op <= 12){
        return 3;}
    else if(p_inst->op <= 14){
        return 4;}
    else if(p_inst->op == 3){
        return 6;}
    else if(p_inst->op == 15){
        return 5;}
    else
    {
        return -1;
    }
}

//Tipo contenido se indica si es un registro o immediate; (0) es registro; (1) es immediate.

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
            return atoi(buffer);
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
        if(tipoContenido == 0){
            return reconocerRegistro(buffer);
        }
        //El indice indica que se trata de un registro
        else{
            return atoi(buffer);
        }
    }
}


int reconocerRegistro (char *registro)
{    
    int num_registro = -1;
    for(int i = 0 ; i < CANTIDAD_REGISTROS;i++)
    {
        if(i == 0){
            if(strncmp(registro,nombreRegistros[i],5) == 0){
                num_registro = i;
            }
        }
        else{
            if(strncmp(registro,nombreRegistros[i],3) == 0){
                num_registro = i;
            }
        }
    }
    if(num_registro == -1){
        printf("Se detecto un registro incorrecto\n");
        printf("Cerrando programa...\n");
        exit(EXIT_FAILURE);
    }
    else{
        return num_registro;
    }
}

void crearArchivoErrores()
{
    FILE *archivo;
    char *nombreArchivo;
    printf("Ingrese en nombre del archivo Hazzars\n");
    nombreArchivo = obtenerNombreArchivo(1);
    archivo = fopen(nombreArchivo,"w");

    for(int i = 0; i < punteroReporte->indiceError;i++)
    {
        error *p_error;
        p_error = punteroReporte->listaError[i];
        fprintf(archivo,"%d.",i);
        fprintf(archivo,"Hazzard de ");
        if(p_error->tipoError == 0 || p_error->tipoError == 1)
        {
            fprintf(archivo,"control ");
        }
        else
        {
             fprintf(archivo,"datos ");
        }
        fprintf(archivo,"en la instruccion %d, CC %d\r\n",p_error->numeroInstruccion,p_error->numeroCiclo);
    }
    fclose(archivo);
    free(nombreArchivo);
    printf("Se genero el archivo con exito!\n\n");
}

void crearArchivoSoluciones()
{
    FILE *archivo;
    char *nombreArchivo;
    printf("Ingrese en nombre del archivo HazzarsSoluciones\n");
    nombreArchivo = obtenerNombreArchivo(1);
    archivo = fopen(nombreArchivo,"w");

    for(int i = 0; i < punteroReporte->indiceError;i++)
    {
        error *p_error;
        p_error = punteroReporte->listaError[i];
        fprintf(archivo,"%d.",i);
        fprintf(archivo,"Solucionable a travez de ");
        if(p_error->tipoError == 0)
        {
            fprintf(archivo,"FLUSH/NOP en IF/ID\r\n");
        }
        else if(p_error->tipoError == 1)
        {
            fprintf(archivo,"FLUSH/NOP en IF/ID, ID/EX y EX/MEM\r\n");
        }
        else if(p_error->tipoError == 2)
        {
            fprintf(archivo,"NOP y Forwarding MEM/WB a ID/EX\r\n");
        }
        else if(p_error->tipoError == 4)
        {
            fprintf(archivo,"Forwarding EX/MEM a ID/EX\r\n");
        }
        else if(p_error->tipoError == 5)
        {
            fprintf(archivo,"Forwarding MEM/WB a ID/EX\r\n");
        }
    }
    fclose(archivo);
    free(nombreArchivo);
    printf("Se genero el archivo con exito!\n\n");
}

void crearArchivoRegistros(registros *p_reg, memoria *p_mem)
{
    FILE *archivo;
    char *nombreArchivo;
    printf("Ingrese en nombre del archivo Registros\n");
    nombreArchivo = obtenerNombreArchivo(1);
    archivo = fopen(nombreArchivo,"w");
    for(int i = 0 ; i < CANTIDAD_REGISTROS;i++)
    {
        if(i != 29)
        {
            fprintf(archivo, "%s %d\r\n",nombreRegistros[i],p_reg->datos[i]);
        }
        else
        {
            fprintf(archivo, "%s 0x123200\r\n",nombreRegistros[i]);
        }
    }
    fprintf(archivo,"Elemementos almacenados en stack pointer\n");
    for(int i = 0 ; i < 10;i++)
    {
        fprintf(archivo, "%d ",p_mem->datos[29][i]);
    }
    fclose(archivo);
    free(nombreArchivo);
    printf("Se genero el archivo con exito!\n\n");
}
