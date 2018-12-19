
#include "estructuras.h"

#ifndef CABECERA_H
#define CABECERA_H
//Creacion de estructuras
instruccion* crearInstruccion();
memoria* crearMemoriaDatos();
registros* crearRegistros();
instruccionesArchivo* crearArregloInstrucciones();
gato* crearGato();
//Lectura de archivo
void guardarInstruccion(instruccionesArchivo *p_listInst,char* instruccion);
char* solicitarNombreArchivo(int tipoArchivo);
instruccionesArchivo* leerArchivoEntrada();
instruccionesArchivo* contenidoArchivo(char* nombreArchivo);
//MIPS
instruccion* instructionFetch(instruccionesArchivo *p_listInst);
int instructionDetection(instruccion *p_instruc, registros *p_reg);
int execute(instruccion *p_instruc);
int memoryAccess(instruccion *p_instruc,memoria *p_mem);
int writeBack(instruccion *p_instruc,registros *p_reg);
//Operaciones para reconocer MIPS
int calcularDireccion(int indice);
int reconocerTipo(instruccion *p_inst);
int reconocerOperacion(instruccion *p_inst);
int reconocerRegistro (char *registro);
int buscarContenidoInstruccion(instruccion *p_inst,int indiceContenido);
//Operaciones para reconocer GATO
void analizarInstruccionGato(gato *p_gato,instruccion *p_inst);
void examinarJugada(gato *p_gato,instruccion *p_inst);
void elegirAccionJugada(gato *p_gato,instruccion *p_inst, int jugador,int jugada);
void guardarJugada(gato *p_gato, int jugador,int jugada);
void eliminarJugada(gato *p_gato,int jugador,int jugada);
void eliminarElementoLista(int *lista,int largoLista,int elemento);
int jugadaExiste_Total(gato *p_gato, int jugada);
int jugadaExiste_Jugador(int *listaJugadas,int largoLista,int jugada);
int estadoPartidaGato(gato *p_gato);
int buscarCombinacionGanadora(int *listaJugadas,int largoLista);
int poseeCombinacionGanadora(int *listaJugadas,int largoLista,int M1,int M2,int M3);
//Funciones para archivos salida
void conformarArchivoSalida_Gato(gato *p_gato);
void agregarFigura(FILE *archivo, char* figura, int indice);
void conformarArchivoSalida_Etapas(int contIF,int contID,int contEX,int contMEM,int contWB);
//Funciones para liberar memoria
void liberarMemoria_instruccion(instruccion *p_inst);
void liberarMemoria_memoria(memoria *p_mem);
void liberarMemoria_registros(registros *p_reg);
void liberarMemoria_instruccionesArchivo(instruccionesArchivo *p_listInst);
void liberarMemoria_gato(gato *p_gato);

#endif /* CABECERA_H */

