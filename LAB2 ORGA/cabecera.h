#include "estructuras.h"

#ifndef CABECERA_H
#define CABECERA_H

forward* crearForward();
instruccion* crearInstruccion(int tipoInstruccion);
instruccionesArchivo* crearInstruccionesArchivo();
registros* crearRegistros();
memoria* crearMemoriaDatos();
char* obtenerNombreArchivo(int tipoArchivo);
instruccionesArchivo* leerArchivoEntrada();
instruccionesArchivo* contenidoArchivo(char* nombreArchivo);
int buscarLabbel(instruccionesArchivo *p_listInst,char *salto);
int buscarDireccionMemoriaInstruccion (instruccionesArchivo *p_listInst, instruccion *p_inst, int indiceContenido);
instruccion* conformarInstruccion(instruccionesArchivo *p_listInst, int contadorPrograma);
pipeline* crearPipeline();
void iniciarPipeline(instruccionesArchivo *p_listInst,memoria *p_mem, registros *p_reg);
int ejecutarPipeline(pipeline *p_pipeline, buffer *p_buffer,memoria *p_mem,registros *p_reg);
void instructionFetch(instruccion *p_inst);
int instructionDetection(instruccion *p_instruc, registros *p_reg, buffer *p_buffer);
int execute(instruccion *p_instruc);
void memoryAccess(instruccion *p_instruc,memoria *p_mem);
void writeBack(instruccion *p_instruc,registros *p_reg);
void modificarDatos(instruccion *p_inst,buffer *p_buffer);
int calcularDireccion(int indice);
void agregarPipeline(pipeline *p_pipeline,instruccion *p_inst,buffer *p_buffer);
int existeHazzardControl(instruccion *p_inst);
int existeHazzardDatos(instruccion *p_inst, pipeline *p_pipe);
void agregarDatosHazzard(instruccion *p_inst,int punto,int posInst,int regProblema);
int reconocerOperacion(instruccion *p_inst);
int reconocerTipo(instruccion *p_inst);
int buscarContenidoInstruccion(instruccion *p_inst,int indiceContenido,int tipoContenido);
int reconocerRegistro (char *registro);
reporte* crearReporteErrores();
void agregarError(int tipoError);

void crearArchivoErrores();
void crearArchivoSoluciones();
void crearArchivoRegistros(registros *p_reg, memoria *p_mem);

#endif /*CABECERA_H*/