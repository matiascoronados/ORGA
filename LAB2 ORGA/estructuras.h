#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H


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

typedef struct buffer
{
	
}




#endif /*ESTRUCTURAS_H*/