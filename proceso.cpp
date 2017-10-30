#include "proceso.h"
#include "cpu.h"
#include <stdio.h>

int Proceso::cuenta=0;


/**
 * @brief Proceso::Proceso
 * Constructor que crera el procesos, asignando valores a sus variables
 * carga archivo y determina memoria a usar, posicion de inicio y fin de memoria
 * y acumula en memoriaOcupada (desde loadSML() )
 * @param nfile
 */
Proceso::Proceso(QString nfile)
{
  //strcpy(nameFile,nfile);
  nameFile = nfile; //archivo con código maquina
  cuenta++;  //cantidad de procesos vivos
  id = cuenta;
  DEBUG = 1;  //1 = instruccion a instruccion se muestra
  machineOn=1; //encendido
  estado=NUEVO; //poner LISTO cuando quede el programa en memoria general
  memPointer=0; //determinar
  memProcess=0; //cantidad de instrucciones
  stackPointer=0; //determinar
  accumulator=0;
  memAsignada=0; //cantidad de memoria asignada
  wait=0;
  prioridad=1;

  consola = new ConsolaProceso();
  //consola->setWindowTitle( QString("Proceso ")+QString::number(id)+"  "+nameFile);
  consola->setWindowTitle( QString("Proceso ")+QString::number(id)+"  "+nameFile.split('/')[nameFile.split('/').length()-1]);
  consola->show();

  //al cargar el archivo se establecen limites de memoria y copian instrucciones
  loadSML(); //carga en memoria de proceso el código ejecutable

  ventanaEstado=new StatusMemory();
  ventanaEstado->recibeProceso( this ); //pasa referencia del proceso

}

//Libera memoria de objetos y arreglos dinamicos
Proceso::~Proceso()
{
  delete consola;
  delete ventanaEstado;
  delete [] codigoMem;
}


//carga archivo a memoria del proceso
void Proceso::loadSML() {
    //QString display;
    int curInst=0;
    char stemp[5];

    consola->Display("Cargando: " + nameFile);

    smlFile = fopen(nameFile.toStdString().c_str(),"r");
    if (smlFile == NULL){
        consola->Display("Error al leer el archivo.\n");
        throw -1; //error
    }

    fscanf(smlFile, "%s %d",stemp, &memProcess);  //leer cantidad de instrucciones para el proceso
    //la primer linea del archivo en código maquina contiene la cantidad aproximada de instrucciones

    //limites de memoria
    //cantidad de memoria asignada en BLOQUES de 50 palabras (enteros)
    memAsignada = (memProcess - memProcess % 50) + ( (memProcess%50>0)?50:0); //bloques de 50 localidades

    CPU::memoriaOcupada += memAsignada; //acumular memoria pedida

    codigoMem = new int[memAsignada]; //memoria total asignada al proceso


    iniMem = (id-1) * 200 ; //se asigna inicio segun ID ahorita (luego por algun metodo)

    finMem = iniMem + memAsignada - 1; //ultima posicion valida de memoria
    stackPointer = finMem;

    if (finMem > CPU::mayorMemProceso)
        CPU::mayorMemProceso=finMem;   //establece el valor de memoria mas alto usado por algun proceso

    for(int k = 0; k<memAsignada; k++)
        codigoMem[k] = 0; //inicializa memoria del proceso


    do { //copia codigo de archivo a memoria del proceso
        curInst = 0;
        fscanf(smlFile, "%d", &curInst);
        codigoMem[memPointer] = curInst;
        memPointer++;
    }while (!feof(smlFile));

    memProcess = memPointer-1;

    fclose(smlFile);
    consola->Display("Programa cargado a la memoria del proceso\n");
    memPointer = iniMem; 
}
