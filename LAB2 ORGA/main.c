
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cabecera.h"
#include "estructuras.h"
#include "constantes.h"

extern reporte *punteroReporte;

int main(int argc, char const **argv)
{
	instruccionesArchivo *p_listInst;
    p_listInst = leerArchivoEntrada();
    punteroReporte = crearReporteErrores();
    memoria *p_mem = crearMemoriaDatos();
    registros *p_reg = crearRegistros();
    iniciarPipeline(p_listInst,p_mem,p_reg);

	for(int i = 0; i < punteroReporte->indiceError;i++)
    {
        error *p_error;
        p_error = punteroReporte->listaError[i];
        printf("numeroInstruccion = %d\n",p_error->numeroInstruccion);
        printf("numeroCiclo = %d\n",p_error->numeroCiclo);
        printf("tipoError = %d\n",p_error->tipoError);
	}
    crearArchivoErrores();
    crearArchivoSoluciones();
    crearArchivoRegistros(p_reg,p_mem);
	return 0;
}