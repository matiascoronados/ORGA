
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cabecera.h"
#include "estructuras.h"
#include "constantes.h"

extern int programCounter;

/*
 * Funcion principal.
 */

int main(int argc, char** argv) {
    listaInstrucciones *p_listInst;
    //Se almacenan todos las instrucciones del archivo de entrada.
    p_listInst = leerArchivoEntrada();
    int max_inst = p_listInst->indice;

    programCounter = 0;
    int contIF = 0;
    int contID = 0;
    int contEX = 0;
    int contMEM = 0;
    int contWB = 0;
    
    gato* p_gato = crearGato();
    registros *p_reg = crearRegistros();
    memoria *p_mem = crearMemoriaDatos();
    instruccion *p_inst;
    //Se procede a leer las instrucciones hasta alcanzar la ultima instruccion.
    while(programCounter != max_inst)
    {
        p_inst = instructionFetch(p_listInst);
        contIF++;
        contID = instructionDetection(p_inst,p_reg) + contID;
        contEX = execute(p_inst) + contEX;
        contMEM = memoryAccess(p_inst,p_mem) + contMEM;
        contWB = writeBack(p_inst,p_reg) + contWB;
        analizarInstruccionGato(p_gato,p_inst);
        liberarMemoria_instruccion(p_inst);
    }
    //Se conforman los archivos de salida.
    conformarArchivoSalida_Gato(p_gato);
    conformarArchivoSalida_Etapas(contIF,contID,contEX,contMEM,contWB);
    //Se libera la memoria de las estructuras utilizadas.
    liberarMemoria_memoria(p_mem);
    liberarMemoria_registros(p_reg);
    liberarMemoria_listaInstrucciones(p_listInst);
    liberarMemoria_gato(p_gato);
    
    return (EXIT_SUCCESS);
}

