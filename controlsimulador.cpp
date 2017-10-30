#include "controlsimulador.h"
#include <QTime>
#include <QEventLoop>
#include <QThread>
#include <QCoreApplication>
#include <QDebug>


void delay2( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

//Crea un procesador y agrega a lista
ControlSimulador::ControlSimulador()
{
    procesadores.push_back(new CPU());
    procesadores.push_back(new CPU());
    apagar=false;
    pausa=false;
}

 //Libera procesos y procesadores pendientes alterminar simulacion
ControlSimulador::~ControlSimulador()
{
 apagar=true;
 for(int i=0; i<procesos.size(); i++){
     delete procesos[i];
 }
 procesos.clear();

 Proceso::cuenta = 0; //inicializa contador de proceso vivos

 for(int i=0; i<procesadores.size(); i++){
     delete procesadores[i];
 }
 procesadores.clear();
}

// Inicia ciclo de simulación
void ControlSimulador::iniciarSimulacion()
{
    while( apagar==false && procesos.size()>=1){
        //round robin
        if(pausa==false) {
        for(int i=0; i<procesos.size(); i++) {

            //esta con dos procesadores alternando ejecucion
            //queda el reto de mandarlos en paralelo
            procesadores[i%2]->ejecuta( procesos[i] );

            if ( procesos[i]->estado == FINALIZADO  ) {
                delay2(4000);
                delete procesos[i]; //liberar de memoria el proceso
                procesos.remove(i); //quitar de la lista
            }

         }

        }
        delay2(100*5);
    }

}
