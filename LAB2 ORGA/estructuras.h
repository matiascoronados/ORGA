#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

//CONSIDERACION: Se utilizaron estructuras usadas en el laboratorio anterior.


/*
 * Descripcion: Estructura que almacena los elementos relacionados al forwarding tales como
 * su posicion, numero de instruccion, cual registro esta involucrado, y si posee multiple forward.
 * 
 */

typedef struct forwardingDatos
{
    int poseeHazzard;
    int posicion;
    int numInstruccion; 
    int registroProblema;
    int multipleForward;
}forward;


/*
 * Descripcion: Dentro de esta estructura se almancenaran los elementos relevantes
 * de cada instruccion, tales como:
 * tipo = Que puede ser del grupo ADDI-SUBI(0) o LW-SW(1)
 * op = Que operacion del grupo es.
 * Rn = Indice del registro.
 * V_Rn = Valor temporal del registro.
 */

typedef struct elementosInstruccion
{
    char *instruccion;
    int tipo;
    int op;
    int R1;
    int R2;
    int R3;
    int valor_R1;
    int valor_R2;
    int valor_R3;
    forward *datosForward;
}instruccion;

/*
 * Descripcion: Estructura que almacena una lista de instrucciones tipo string, estas son
 * ordanizadas por el indice de la estructura, el cual se modifica cada vez que se agrega
 * un nuevo elemento.
 */
typedef struct instruccionesArchivoArchivoLectura
{
    char **datos;
    int indice;
}instruccionesArchivo;

/*
 * Descripcion: Dentro de esta estructura se almacenan todos los datos relacionados con
 * arreglos. Tal sea asi, que se conforma un arreglo por cada registro de este.
 */

typedef struct memoriaDatos
{
    int **datos;
}memoria;

/*
 * Descripcion: Estructura que almacena una lista representativa de los datos almacenados
 * en los registros del programa.
 */

typedef struct registrosPrograma
{
    int *datos;
}registros;

/*
 * Descripcion: Estructura que tiene un puntero para cada instruccion que se ejecutara dentro del pipeline
 */

typedef struct elementosPipeline
{
    instruccion *IF;
	instruccion *ID;
	instruccion *EX;
	instruccion *MEM;
	instruccion *WB;
}pipeline;


/*
 * Descripcion: Estructura que almacena un puntero por cada instruccion que paso por los buffer intermedios.
 */
typedef struct bufferIntermedios
{
    instruccion *ID_EX;
    instruccion *EX_MEM;
    instruccion *MEM_WB;
}buffer;


/*
 * Descripcion: Estructura que almacena el contenido de un error tipo hazard.
 */
typedef struct errorHazzard
{
    int numeroInstruccion;
    int numeroCiclo;
    int tipoError;
}error;

/*
 * Descripcion: Estructura que almacena multiples errores.
 */
typedef struct reporteHazzard
{
    error **listaError;
    int indiceError;
}reporte;





#endif /*ESTRUCTURAS_H*/