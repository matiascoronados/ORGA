
#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

/*
 * Descripcion: Dentro de esta estructura se almancenaran los elementos relevantes
 * de cada instruccion, tales como:
 * tipo = Que puede ser del grupo ADDI-SUBI(0) o LW-SW(1)
 * op = Que operacion del grupo es.
 * Rn = Indice del registro.
 * V_Rn = Valor temporal del registro.
 */
typedef struct lineaInstruccion
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
 * Descripcion: Estructura que almacena una lista de instrucciones tipo string, estas son
 * ordanizadas por el indice de la estructura, el cual se modifica cada vez que se agrega
 * un nuevo elemento.
 */
typedef struct arregloInstruccion
{
    char **datos;
    int indice;
}instruccionesArchivo;

/*
 * Descripcion: Estructura representativa del juego "gato", en donde se tiene dos enteros
 * que representan los registros relacionados con los jugadores, un entero que indica las
 * dimenciones del tablero, un contador contenido que se encarga de organizar el almacenamiento
 * de los elementos de este, ademas de dos listas que representan las jugadas de los respectivos
 * jugadores.
 */
typedef struct juegoGato
{
    int jugadorA;
    int jugadorB;
    int espacioTablero;
    int contadorContenido;
    int* listaJugadasA;
    int indiceA;
    int* listaJugadasB;
    int indiceB;
}gato;

#endif /* ESTRUCTURAS_H */

