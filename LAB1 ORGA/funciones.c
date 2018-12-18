
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cabecera.h"
#include "constantes.h"

int programCounter = 0;
char* nombreRegistros[CANTIDAD_REGISTROS] = {"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7"
                                ,"$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"};
/*
 * Entradas: N/A
 * Salida: Puntero de una estructura instruccion, con todos sus elementos inicializados,
 *         y con memoria. 
 */
instruccion* crearInstruccion()
{
    instruccion *p_instruccion = malloc(sizeof(instruccion));
    p_instruccion->instruccion = malloc(sizeof(char)*MAX_STRING);
    p_instruccion->tipo = 0;
    p_instruccion->op = 0;
    p_instruccion->R1 = 0;
    p_instruccion->R2 = 0;
    p_instruccion->R3 = 0;
    p_instruccion->V_R1 = 0;
    p_instruccion->V_R2 = 0;
    p_instruccion->V_R3 = 0;
    return p_instruccion;
}

/*
 * Entradas: N/A
 * Salida: Puntero de una estructura memoria, con todos sus elementos inicializados,
 *         y con memoria. 
 */
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

/*
 * Entradas: N/A
 * Salida: Puntero de una estructura registros, con todos sus elementos inicializados,
 *         y con memoria. 
 */
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

/*
 * Entradas: N/A
 * Salida: Puntero de una estructura listaInstrucciones, con todos sus elementos inicializados,
 *         y con memoria. 
 */
listaInstrucciones* crearArregloInstrucciones()
{
    listaInstrucciones *p_listInst = malloc(sizeof(listaInstrucciones));
    p_listInst->indice = 0;
    p_listInst->datos = malloc(sizeof(char*)*MAX_ARREGLO);
    for(int i = 0 ; i < MAX_ARREGLO;i++)
    {
        p_listInst->datos[i] = malloc(sizeof(char)*MAX_STRING);
    }
    return p_listInst;
}

/*
 * Entradas: N/A
 * Salida: Puntero de una estructura gato, con todos sus elementos inicializados,
 *         y con memoria. 
 */
gato* crearGato()
{
    gato *p_gato = malloc(sizeof(gato));
    p_gato->espacioTablero = 0;
    p_gato->jugadorA = 0;
    p_gato->jugadorB = 0;
    p_gato->contadorContenido = 0;
    p_gato->indiceA = 0;
    p_gato->indiceB = 0;
    p_gato->listaJugadasA = malloc(sizeof(int)*MAX_TABLERO);
    p_gato->listaJugadasB = malloc(sizeof(int)*MAX_TABLERO);
    return p_gato;
}

/*
 * Entradas: Puntero a una lista de instrucciones, y una instruccion a almacenar
 * Salida: N/A
 */
void guardarInstruccion(listaInstrucciones *p_listInst,char* instruccion)
{
    int indice = p_listInst->indice;
    strcpy(p_listInst->datos[indice],instruccion);
    p_listInst->indice = indice +1;
}

/*
 * Entradas: Entero que indica cual nombre se le tiene que solicitar al usuario
 * Salida: String representativo del nombre de un archivo.
 */
char* solicitarNombreArchivo(int tipoArchivo)
{
    FILE *archivo;
    char *nombreArchivo;
    nombreArchivo = malloc(sizeof(char)*MAX_STRING);
    if(tipoArchivo == 0){
        printf("Ingrese el nombre del archivo de entrada: Instrucciones\n");
    }
    else if(tipoArchivo == 1){
        printf("Ingrese el nombre para el archivo de salida: Resultados del juego\n");
    }
    else{
        printf("Ingrese el nombre para el archivo de salida: Etapas\n");
    }
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

/*
 * Entradas: N/A
 * Salida: Lista de instrucciones leidas a partir de un archivo de entrada.
 */
listaInstrucciones* leerArchivoEntrada()
{
    char* nombreArchivo = solicitarNombreArchivo(0);
    listaInstrucciones* listaSalida = contenidoArchivo(nombreArchivo);
    free(nombreArchivo);
    return listaSalida;
}

/*
 * Entradas: String que indica el nombre del archivo a abrir
 * Salida: Lista con todas las intrucciones encontradas dentro del archivo.
 */
listaInstrucciones* contenidoArchivo(char* nombreArchivo)
{
    FILE *archivo;
    archivo = fopen(nombreArchivo,"r");
    listaInstrucciones *p_listInst = crearArregloInstrucciones();
    int indice = p_listInst->indice;

    while(feof(archivo) == 0){
        fgets(p_listInst->datos[indice],MAX_STRING,archivo);
        //Si el elemento es un comentario, no se cosidera.
        if(strncmp(p_listInst->datos[indice],"#",1) == 0){
            indice--;
        }
        indice++;
    }
    //Se almacena el numero de instrucciones guardadas.
    p_listInst->indice = indice;
    fclose(archivo);
    return p_listInst;
}

/*
 * Entradas: Puntero a una estructura que almacena una lista de instrucciones
 * Salida: Instruccion con todos sus elementos reconocidos. 
 * Consideracion: La eleccion de cual instruccion tomar esta dado por la variable
 * global programCounter, la cual se modifica tras cada iteracion de la funcion.
 */
instruccion* instructionFetch(listaInstrucciones *p_listInst)
{
    instruccion* p_inst = crearInstruccion();
    strcpy(p_inst->instruccion,p_listInst->datos[programCounter]);
    p_inst->op = reconocerOperacion(p_inst);
    p_inst->tipo = reconocerTipo(p_inst);
    p_inst->R1 = buscarContenidoInstruccion(p_inst,1);
    p_inst->R2 = buscarContenidoInstruccion(p_inst,2);
    p_inst->R3 = buscarContenidoInstruccion(p_inst,3);
    programCounter++;
    return p_inst;
}

/*
 * Entradas: Puntero a una instruccion, y un puntero a los registros actuales.
 * Salida: Entero que indica si se paso o no por el proceso. 1 == Si; 0 == No.
 * Consideracion: En la funcion se le asiganran valores V a cada registro de la
 * instruccion, esto a razon del contenido de la instruccion, y el valor que posean
 * los registros actuales.
 */
int instructionDetection(instruccion *p_instruc, registros *p_reg)
{
    if(p_instruc->tipo == 0){
        //Operaciones: ADDI, SUBI
        int indice_R2;
        indice_R2 = p_instruc->R2;
        p_instruc->V_R2 = p_reg->datos[indice_R2];
        p_instruc->V_R3 = p_instruc->R3;
        return 1;
    }
    else if(p_instruc->tipo == 1){
        if(p_instruc->op == 3){
            //Operacion: SW
            int indice_R1;
            indice_R1 = p_instruc->R1;
            p_instruc->V_R1 = p_reg->datos[indice_R1];
            p_instruc->V_R2 = p_instruc->R2;
            return 1;
        }
        else{
            //Operacion: LW
            return 1;
        }
    }
    return 0;
}

/*
 * Entradas: Puntero a una instruccion
 * Salida: Entero que indica si se paso o no por el proceso. 1 == Si; 0 == No.
 * Consideracion: Se procede a realizar todos los calculos pertinentes a cada operacion
 * para luego almacenarlo en su respectivo registro.
 */
int execute(instruccion *p_instruc)
{
    if(p_instruc->tipo == 0){
        //Operacion ADDI
        if(p_instruc->op == 0){
            p_instruc->V_R1 = p_instruc->V_R2 + p_instruc->V_R3;
            return 1;
        }
        //Operacion SUBI
        else if(p_instruc->op == 1){
            p_instruc->V_R1 = p_instruc->V_R2 - p_instruc->V_R3;
            return 1;
        }
    }
    else{
        //Operacion LW, SW.
        p_instruc->V_R2 = calcularDireccion(p_instruc->R2);
        return 1;
    }
}

/*
 * Entradas: Puntero a una instruccion, y un puntero a la memoria.
 * Salida: Entero que indica si se paso o no por el proceso. 1 == Si; 0 == No.
 * Consideracion: Obtiene los datos almacenados en la memoria, para las instrucciones
 * LW y SW.
 */
int memoryAccess(instruccion *p_instruc,memoria *p_mem)
{
    if(p_instruc->tipo == 1){
        if(p_instruc->op == 2){
            //Operacion LW
            int indiceRegistro = p_instruc->R3;
            int indiceElemento = p_instruc->V_R2;
            p_instruc->V_R1 = p_mem->datos[indiceRegistro][indiceElemento];
            return 1;
        }
        else if(p_instruc->op == 3){
            //Operacion SW
            int indiceRegistro = p_instruc->R3;
            int indiceElemento = p_instruc->V_R2;
            p_mem->datos[indiceRegistro][indiceElemento] = p_instruc->V_R1;
            return 1;
        }
    }
    else{
        //Operacion ADDI, SUBI
        return 0;
    }
}

/*
 * Entradas: Puntero a una instruccion,y un puntero a los registros actuales.
 * Salida: Entero que indica si se paso o no por el proceso. 1 == Si; 0 == No.
 * Consideracion: Se procede a escribir en los registro actuales, a razon de los
 * elemementos obtenidos en el desarrollo de la instruccion de entrada.
 */
int writeBack(instruccion *p_instruc,registros *p_reg)
{
    if(p_instruc->tipo == 0){
        //Operacion ADDI,SUBI
        int indiceRegistro = p_instruc->R1;
        p_reg->datos[indiceRegistro] = p_instruc->V_R1;
        return 1;
    }
    else if(p_instruc->tipo == 1){
        if(p_instruc->op == 2){
            //Operacion LW
            int indiceRegistro = p_instruc->R1;
            p_reg->datos[indiceRegistro] = p_instruc->V_R1;
            return 1;
        }
        else{
            //Operacion SW
            return 0;
        }
    }
    else{
        return 0;
    }
}

/*
 * Entradas: Entero indice, el cual se transforma para que se norme de tal forma
 * que apunte a la dirrecion de los registros del programa.
 * Salida: Entero indice modificado.
 */
int calcularDireccion(int indice)
{
    double direccion = indice/4.0;
    direccion = fabs(direccion);
    return (int)direccion;
}

/*
 * Entradas: Puntero a una instruccion.
 * Salida: Entero que indica cual es el tipo de la instruccion, 0 para ADDI,SUBI
 * y 1 para LW,SW.
 */
int reconocerTipo(instruccion *p_inst)
{
    if(p_inst->op <= 1){
        return 0;
    }
    else{
        return 1;
    }
}

/*
 * Entradas: Puntero a una instruccion.
 * Salida: Entero que indica cual operacion corresponde
 * Consideraion: A razon del string leido en el archivo, se procede a analizar
 * que tipo de operacion corresponde, representando a esta con un entero.
 */
int reconocerOperacion(instruccion *p_inst)
{
    char *operacion = p_inst->instruccion;
    if(strncmp(operacion,"addi",4) == 0){
        return 0;
    }
    else if(strncmp(operacion,"subi",4) == 0){
        return 1;
    }
    else if(strncmp(operacion,"lw",2) == 0){
        return 2;
    }
    else if(strncmp(operacion,"sw",2) == 0){
        return 3;
    }
    else{
        printf("Se detecto una instruccion incorrecta\n");
        printf("Cerrando programa...\n");
        exit(EXIT_FAILURE);
    }
}

/*
 * Entradas: Puntero a una instruccion.
 * Salida: Entero que indica cual registro corresponde.
 * Consideracion: A razon de un string registro, se procede a analizar a cuales de esto
 * corresponde dentro de los almacenados en el programa.
 */
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

/*
 * Entradas: Puntero a una instruccion, entero que indica la posicion del elemento a leer.
 * Salida: Entero representativo del elemento encontrado.
 * Consideracion: La funcion se encarga de subdividir un string leido del archivo,
 * con el fin de obtener uno de sus elementos. La obtencion del elemento estara normada
 * por el indice de entrada.
 */
int buscarContenidoInstruccion(instruccion *p_inst,int indiceContenido)
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
    if(p_inst->tipo == 0)
    {
        while( buffer != NULL  && indice != 0){
            buffer = strtok( NULL, separacionA);
            indice--;
        }
        //El indice indica que es un IMMEDIATE.
        if(indiceContenido == 3){
            return atoi(buffer);
        }
        //El indice indica que se trata de un registro
        else{
            return reconocerRegistro(buffer);
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
        if(indiceContenido == 2){
            return atoi(buffer);
        }
        //El indice indica que se trata de un registro
        else{
            return reconocerRegistro(buffer);
        }
    }
}

/*
 * Entradas: Puntero a un gato, y un puntero a una instruccion.
 * Salida: N/A
 * Consideracion: La funcion se encarga de analizar la instruccion con el fin de
 * almacenar, en el puntero gato, el espacio del tablero, los registros
 * de los jugadores,y las jugadas de estos. Estos procesos estaran guiados por un
 * contador de contenido, el cual se modificara tras cada proceso.
 */
void analizarInstruccionGato(gato *p_gato,instruccion *p_inst)
{
    if(p_gato->contadorContenido == 0){
        //Almacenar registro de Jugador 1 X
        p_gato->jugadorA = p_inst->R1;
        p_gato->contadorContenido = p_gato->contadorContenido +1;
    }
    else if(p_gato->contadorContenido == 1){
        //Almacenar registro de Jugador 2 O
        if(p_inst->R3 == 2)
        {
            p_gato->jugadorB = p_inst->R1;
        }
        else
        {
            p_gato->jugadorB = p_gato->jugadorA;
            p_gato->jugadorA = p_inst->R1;
        }
        
        p_gato->contadorContenido = p_gato->contadorContenido +1;
    }
    else if(p_gato->contadorContenido == 2){
        //Si se trata del registro $sp
        //Almacenar espacio del tablero.
        if(p_inst->R1 == 29){
            p_gato->espacioTablero = abs(p_inst->V_R3);
        }
        else{
            //Guardar jugada de usuario
            examinarJugada(p_gato,p_inst);
        }
        p_gato->contadorContenido = p_gato->contadorContenido +1;
    }
    else{
        //Guardar jugada de usuario
        examinarJugada(p_gato,p_inst);
    }
}

/*
 * Entradas: Puntero a un gato, y un puntero a una instruccion.
 * Salida: N/A
 * Consideracion: Dependiendo de la operacion, y el tipo de la instruccion, se
 * procede a analizar que jugada realizo el usuario.
 */
void examinarJugada(gato *p_gato,instruccion *p_inst)
{
    //La instruccion indica que se trata del jugador A
    if(p_inst->R1 == p_gato->jugadorA){
        //Jugada de la forma OP R1, R2, R3
        if(p_inst->tipo == 0){
            elegirAccionJugada(p_gato,p_inst,p_gato->jugadorA,p_inst->V_R3);
        }
        //Jugada de la forma OP R1, R2(R3)
        else if(p_inst->tipo == 1){
            elegirAccionJugada(p_gato,p_inst,p_gato->jugadorA,p_inst->V_R2);
        }
    }
    //La instruccion indica que se trata del jugador B
    else if(p_inst->R1 == p_gato->jugadorB){
        //Jugada de la forma OP R1, R2, R3
        if(p_inst->tipo == 0){
            elegirAccionJugada(p_gato, p_inst,p_gato->jugadorB,p_inst->V_R3);
        }
        //Jugada de la forma OP R1, R2(R3)
        else if(p_inst->tipo == 1){
            elegirAccionJugada(p_gato, p_inst,p_gato->jugadorB,p_inst->V_R2);
        }
    }
    else
    {
        printf("Se detecto una jugada invalida:\n");
        printf("No existe el jugador %s\n",nombreRegistros[p_inst->R1]);
    }
}

/*
 * Entradas: Puntero a un gato, puntero a una instruccion, dos enteros, uno que representa
 * al registro del jugador, y otro que indica la posicion de su jugada.
 * Salida: N/A
 * Consideracion: A raiz de la operacion de la instruccion, y de las jugadas que se encuentran
 * utilizadas, se procede a almacenar o eliminar una jugada.
 */
void elegirAccionJugada(gato *p_gato,instruccion *p_inst, int jugador,int jugada)
{
    //Verifica si existe la jugada en cualquiera de los 2 jugadores.
    //Si la jugada no existe.
    if(jugadaExiste_Total(p_gato,jugada) == 0){
        //Si la operacion no es SUBI (eliminar jugada)
        if(p_inst->op != 1){
            if(p_inst->tipo == 0){
                //Si se trata de las operaciones ADDI y SUBI, la jugada sera representada de 1 a 9.
                guardarJugada(p_gato,jugador,jugada);
            }
            else{
                //Si la operacion es SW, la jugada sera representada del 0 al 8, por ello de le agrega 1 para que quede del 1 al 9.
                guardarJugada(p_gato,jugador,jugada+1);
            }
            
        }
        //En el caso de que se desee eliminar una jugada que no existe.
        else{
            printf("Se detecto una jugada invalida:\n");
            printf("El jugador %s intento eliminar una jugada que no existe\n",nombreRegistros[jugador]);
        }
    }
    //Si la jugada existe.
    else{
        //Si la operacion es SUBI (eliminar jugada)
        if(p_inst->op == 1){
            //Proceso para verificar, y elimiar (si se puede) la jugada.
            if(p_inst->tipo == 0){
                eliminarJugada(p_gato,jugador,jugada);
            }
            else{
                eliminarJugada(p_gato,jugador,jugada+1);
            }
        }
        //En el caso de que se desee realizar una jugada ya hecha.
        else{
            printf("Se detecto una jugada invalida:\n");
            printf("El jugador %s intento realizar una jugada ya utilizada\n",nombreRegistros[jugador]);
        }
    }
}

/*
 * Entradas: Puntero a un gato y dos enteros donde uno que representa
 * al registro del jugador, y otro que indica la posicion de su jugada.
 * Salida: N/A
 * Consideracion: Se procede a almacenar la jugada dentro de la lista de jugadas
 * del jugador de entrada.
 */
void guardarJugada(gato *p_gato, int jugador,int jugada)
{
    if(jugada >= 1 && jugada <= 9){
        if(jugador == p_gato->jugadorA){
        int indice = p_gato->indiceA;
        p_gato->listaJugadasA[indice] = jugada;
        p_gato->indiceA = p_gato->indiceA +1;
    }
    else{
        int indice = p_gato->indiceB;
        p_gato->listaJugadasB[indice] = jugada;
        p_gato->indiceB = p_gato->indiceB +1;
    }
    }
    else
    {
        printf("Se detecto una jugada invalida:\n");
        printf("El jugador %s intento agregar una jugada fuera de los limites del tablero\n",nombreRegistros[jugador]);
    }

}

/*
 * Entradas: Puntero a un gato y dos enteros donde uno que representa
 * al registro del jugador, y otro que indica la posicion de su jugada.
 * Salida: N/A
 * Consideracion: Se procede a eliminar una jugada de la lista de jugadas del
 * jugador de entrada, verificando antes de que esta le pertenezca al mismo.
 */
void eliminarJugada(gato *p_gato,int jugador,int jugada)
{
    if(jugador == p_gato->jugadorA){
        //Se verifica si la jugada le pertenece al jugador de entrada
        //Jugada le pertenece al jugador A
        if(jugadaExiste_Jugador(p_gato->listaJugadasA,p_gato->indiceA,jugada) == 1){
            eliminarElementoLista(p_gato->listaJugadasA,p_gato->indiceA,jugada);
            p_gato->indiceA = p_gato->indiceA -1;
        }
        //Jugada no le pertenece al jugador A
        else{
            printf("Se detecto una jugada invalida:\n");
            printf("El jugador %s intento eliminar una jugada adversaria\n",nombreRegistros[p_gato->jugadorA]);
        }
    }
    else{
        //Se verifica si la jugada le pertenece al jugador de entrada
        //Jugada le pertenece al jugador B
        if(jugadaExiste_Jugador(p_gato->listaJugadasB,p_gato->indiceB,jugada) == 1){
            eliminarElementoLista(p_gato->listaJugadasB,p_gato->indiceB,jugada);
            p_gato->indiceB = p_gato->indiceB -1;
        }
        //Jugada no le pertenece al jugador B
        else{
            printf("Se detecto una jugada invalida:\n");
            printf("El jugador %s intento eliminar una jugada adversaria\n",nombreRegistros[p_gato->jugadorB]);
        }
    }
}

/*
 * Entradas: Puntero a una lista de enteros, dos enteros, uno que indica el largo
 * de esta, y el otro, el elemento que se desea eliminar.
 * Salida: N/A
 */
void eliminarElementoLista(int *lista,int largoLista,int elemento)
{
    for(int i = 0 ; i < largoLista; i++){
        if(lista[i] == elemento){
            lista[i] = lista[largoLista-1];
            i = largoLista;
        } 
    }
}

/*
 * Entradas: Puntero a un gato y un entero que representa una jugada a analizar.
 * Salida: Entero que indica si existe (1) o no existe(0) una jugada en ambos usuarios.
 */
int jugadaExiste_Total(gato *p_gato, int jugada)
{
    int verificador = 0;
    verificador = jugadaExiste_Jugador(p_gato->listaJugadasA,p_gato->indiceA,jugada);
    //Si no se encontro la jugada en el jugador A.
    if(verificador == 0){
        verificador = jugadaExiste_Jugador(p_gato->listaJugadasB,p_gato->indiceB,jugada);
    }
    return verificador;
}

/*
 * Entradas: Puntero a una lista de enteros representativas a las jugadas de un usuario,
 *  y dos enteros, uno que indica el largo de la lista, y otro que indica la jugada.
 * Salida: Entero que indica si existe (1) o no existe(0) la jugada dentro de la lista
 * de entrada.
 */
int jugadaExiste_Jugador(int *listaJugadas,int largoLista,int jugada)
{
    int verificador = 0;
    for(int i = 0 ; i < largoLista; i++){
        if(listaJugadas[i] == jugada){
            verificador = 1;
        }
    }
    return verificador;
}

/*
 * Entradas: Puntero a un gato.
 * Salida: Entero que indica el estado final de la partida, en donde, 
 * 0 = Incompleto; 1 = Gana Jugador A; 2 = Gana Jugador B; 3 = Empate.
 * Consideracion: Para analizar el estado, se hace uso de la busqueda de una
 * combinacion ganadora dentro de las jugadas de cada usuario.
 */
int estadoPartidaGato(gato *p_gato)
{
    int elementosA = p_gato->indiceA;
    int elementosB = p_gato->indiceB;
    int totalElementos = elementosA + elementosB;
    //Si no hay suficientes jugadas, el juego esta incompleto.
    if(totalElementos < 9){
        return 0;
    }
    else{
        //Si el jugador A posee una combinacion ganadora.
        if(buscarCombinacionGanadora(p_gato->listaJugadasA,p_gato->indiceA) == 1){
            return 1;
        }
        else{
            //Si el jugador B posee una combinacion ganadora.
            if(buscarCombinacionGanadora(p_gato->listaJugadasB,p_gato->indiceB) == 1){
                return 2;
            }
            //Si ninguno de los jugadores posee una combinacion ganadora.
            else{
                return 3;
            }
        }
    }
}

/*
 * Entradas: Puntero a una lista de enteros representativas a las jugadas de un usuario,
 *  y un entero que indica el largo de la lista.
 * Salida: Entero que indica si la lista posee una combinacion ganadora, en donde
 * 1 = Posee una combinacion ganadora, y 0 no la posee.
 */
int buscarCombinacionGanadora(int *listaJugadas,int largoLista)
{
    if(poseeCombinacionGanadora(listaJugadas,largoLista,0,1,2) == 1){
        return 1;
    }
    else if(poseeCombinacionGanadora(listaJugadas,largoLista,3,4,5) == 1){
        return 1;
    }
    else if(poseeCombinacionGanadora(listaJugadas,largoLista,6,7,8) == 1){
        return 1;
    }
    else if(poseeCombinacionGanadora(listaJugadas,largoLista,0,3,6) == 1){
        return 1;
    }
    else if(poseeCombinacionGanadora(listaJugadas,largoLista,1,4,7) == 1){
        return 1;
    }
    else if(poseeCombinacionGanadora(listaJugadas,largoLista,2,5,8) == 1){
        return 1;
    }
    else if(poseeCombinacionGanadora(listaJugadas,largoLista,0,4,8) == 1){
        return 1;
    }
    else if(poseeCombinacionGanadora(listaJugadas,largoLista,6,4,2) == 1){
        return 1;
    }
    else{
        return 0;
    }
}
/*
 * Entradas: Puntero a una lista de enteros representativas a las jugadas de un usuario,
 * un entero que indica el largo de la lista y 3 enteros Mn, que representan una posible
 * combinacion ganadora.
 * Salida: Entero que indica si la lista posee una combinacion ganadora, en donde
 * 1 = Posee una combinacion ganadora, y 0 no la posee.
 * Consideracion: La funcion utiliza 3 verificadores, los cuales indican si se encuentran
 * M1,M2, y M3 dentro de la lista.
 */
int poseeCombinacionGanadora(int *listaJugadas,int largoLista,int M1,int M2,int M3)
{
    int v_1,v_2,v_3;
    v_1 = 0;
    v_2 = 0;
    v_3 = 0;
    for(int i = 0 ; i < largoLista; i++){
        if(listaJugadas[i] == M1){
            v_1 = 1;
        }
        else if(listaJugadas[i] == M2){
            v_2 = 1;
        }
        else if(listaJugadas[i] == M3){
            v_3 = 1;
        }
    }
    //La lista posee los 3 elementos de entrada.
    if( v_1 == 1 && v_2 == 1 && v_3 == 1){
        return 1;
    }
    //La lista no posee alguno de los 3 elementos de entrada.
    else{
        return 0;
    }
}

/*
 * Entradas: Puntero a un gato
 * Salida: N/A
 * Consideracion: La funcion conforma el archivo Resultado.txt, a razon
 * de los elementos almacenados dentro del gato de entrada. Ingresanto tanto
 * los elementos del tablero, como el estado final de la partida.
 */
void conformarArchivoSalida_Gato(gato *p_gato)
{
    FILE *archivo;
    char *nombreArchivo;
    nombreArchivo = solicitarNombreArchivo(1);
    archivo = fopen(nombreArchivo,"w");
    for(int i = 0 ; i < MAX_TABLERO;i++){
        if(jugadaExiste_Jugador(p_gato->listaJugadasA,p_gato->indiceA,i) == 1){
            agregarFigura(archivo,"X",i);
        }
        else if(jugadaExiste_Jugador(p_gato->listaJugadasB,p_gato->indiceB,i) == 1){
            agregarFigura(archivo,"O",i);
        }
        else {
            agregarFigura(archivo," ",i);
        }
    }
    int estadoFinal = estadoPartidaGato(p_gato);
    if(estadoFinal == 0){
        fprintf(archivo,"El juego se encuentra incompleto");
    }
    else if(estadoFinal == 1){
        fprintf(archivo,"El jugador %s gano la partida!",nombreRegistros[p_gato->jugadorA]);
    }
    else if (estadoFinal == 2){
        fprintf(archivo,"El jugador %s gano la partida!",nombreRegistros[p_gato->jugadorB]);
    }
    else{
        fprintf(archivo,"Empate!");
    }
    fclose(archivo);
    printf("Se genero el archivo de resultados con exito!\n");
}

/*
 * Entradas: Puntero a un archivo, un string representativo de una figura y un
 * entero que indica cual la posicion de este.
 * Salida: N/A
 * Consideracion: El indice se modifica para conformar una figura de 3x3.
 */
void agregarFigura(FILE *archivo, char* figura, int indice)
{
    indice = indice%3;    
    if(indice == 0){
        fprintf(archivo,"%s|",figura);
    }
    else if(indice == 1){
        fprintf(archivo,"%s",figura);
    }
    else if(indice == 2){
        fprintf(archivo,"|%s\r\n",figura);
    }
    
}

/*
 * Entradas: Enteros representativos de los contadores de cada segmento del camino de datos.
 * Salida: N/A
 * Consideracion: Se procede a crear una archivo Etapas.txt para almacenar los elementos de
 * entrada.
 */
void conformarArchivoSalida_Etapas(int contIF,int contID,int contEX,int contMEM,int contWB)
{
    FILE *archivo;
    char *nombreArchivo;
    nombreArchivo = solicitarNombreArchivo(2);
    archivo = fopen(nombreArchivo,"w");
    fprintf(archivo,"Etapa  Cantidad\r\n");
    fprintf(archivo,"IF     %d\r\n",contIF);
    fprintf(archivo,"ID     %d\r\n",contID);
    fprintf(archivo,"EX     %d\r\n",contEX);
    fprintf(archivo,"MEM    %d\r\n",contMEM);
    fprintf(archivo,"WB     %d\r\n",contWB);
    fclose(archivo);
    free(nombreArchivo);
    printf("Se genero el archivo contador de etapas con exito!");
}

/*
 * Entradas: Puntero a una instruccion
 * Salida: N/A
 * Consideracion: La funcion se encarga de liberar la memoria dinamica de los elementos
 * de la estructura instruccion
 */
void liberarMemoria_instruccion(instruccion *p_inst)
{
    if(p_inst != NULL){
        free(p_inst->instruccion);
        free(p_inst);
    }
}

/*
 * Entradas: Puntero a una memoria
 * Salida: N/A
 * Consideracion: La funcion se encarga de liberar la memoria dinamica de los elementos
 * de la estructura memoria
 */
void liberarMemoria_memoria(memoria *p_mem)
{
    if(p_mem != NULL){
        for(int i = 0 ; i < CANTIDAD_REGISTROS; i++){
            free(p_mem->datos[i]);
        }
        free(p_mem->datos);
        free(p_mem);
    }
}

/*
 * Entradas: Puntero a una estructura de registros
 * Salida: N/A
 * Consideracion: La funcion se encarga de liberar la memoria dinamica de los elementos
 * de la estructura registros
 */
void liberarMemoria_registros(registros *p_reg)
{
    if(p_reg != NULL){
        free(p_reg->datos);
        free(p_reg);
    }
}

/*
 * Entradas: Puntero a una lista de instrucciones
 * Salida: N/A
 * Consideracion: La funcion se encarga de liberar la memoria dinamica de los elementos
 * de la estructura lista de instrucciones
 */
void liberarMemoria_listaInstrucciones(listaInstrucciones *p_listInst)
{
    if(p_listInst != NULL){
        for(int i = 0 ; i < MAX_ARREGLO; i++){
            free(p_listInst->datos[i]);
        }
        free(p_listInst->datos);
        free(p_listInst);
    }
}

/*
 * Entradas: Puntero a una estructura gato
 * Salida: N/A
 * Consideracion: La funcion se encarga de liberar la memoria dinamica de los elementos
 * del gato
 */
void liberarMemoria_gato(gato *p_gato)
{
    if(p_gato != NULL){
        free(p_gato->listaJugadasA);
        free(p_gato->listaJugadasB);
        free(p_gato);
    }
}

