#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H


typedef struct forwardingDatos
{
    int poseeHazzard;
    int posicion;
    int numInstruccion; 
    int registroProblema;
    int multipleForward;
}forward;

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


typedef struct instruccionesArchivoArchivoLectura
{
    char **datos;
    int indice;
}instruccionesArchivo;


typedef struct memoriaDatos
{
    int **datos;
}memoria;


typedef struct registrosPrograma
{
    int *datos;
}registros;

typedef struct elementosPipeline
{
    instruccion *IF;
	instruccion *ID;
	instruccion *EX;
	instruccion *MEM;
	instruccion *WB;
}pipeline;

typedef struct bufferIntermedios
{
    instruccion *ID_EX;
    instruccion *EX_MEM;
    instruccion *MEM_WB;
}buffer;

typedef struct errorHazzard
{
    int numeroInstruccion;
    int numeroCiclo;
    int tipoError; // 0 = JUMP; 1 = BEQ; 2 = SW Con NOP; 3 = SW sin NOP; 4 = Forwarding EX/MEM; 5 = Forwarding MEM/WB.
}error;

typedef struct reporteHazzard
{
    error **listaError;
    int indiceError;
}reporte;





#endif /*ESTRUCTURAS_H*/