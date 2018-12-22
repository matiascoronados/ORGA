
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cabecera.h"
#include "estructuras.h"
#include "constantes.h"

extern reporte *punteroReporte;

/*
 * Funcion principal del programa
 * Autor:   Matias Coronado
 * Trabajo: Laboratorio 3
 * Ramo:    Organizacion de computadores.
 */

int main(int argc, char const **argv)
{
	instruccionesArchivo *p_listInst;
    p_listInst = leerArchivoEntrada();
    punteroReporte = crearReporteErrores();
    memoria *p_mem = crearMemoriaDatos();
    registros *p_reg = crearRegistros();
    iniciarPipeline(p_listInst,p_mem,p_reg);
    crearArchivoErrores();
    crearArchivoSoluciones();
    crearArchivoRegistros(p_reg,p_mem);
	return 0;
}