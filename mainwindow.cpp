#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStringListModel>
#include <QtCore>
#include "asim.h"
#include "showmem.h"
#include "helpfile.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    simulador = new ControlSimulador();
    enSimulacion=false;
    graficador = new showmem(this);
}

MainWindow::~MainWindow()
{
    if(simulador!=NULL)
        delete simulador;
    delete graficador;
    delete ui;

}

//Crea procesos y queda en espera (NUEVO)(ventanas de consola y estado)
void MainWindow::on_pushButton_clicked()
{
   if(ui->listWidget_Process->selectedItems().length()>0){
     QString filenameX = ui->listWidget_Process->currentItem()->toolTip();

    try{
     simulador->procesos.push_back(new Proceso(filenameX));
    }
    catch(...){
       QMessageBox::information(this,"Error","Archivo no localizado, Proceso no creado");
    }
   }

}

//Muestra ventanas de consolas de procesos
void MainWindow::on_pushButton_2_clicked()
{
    for(int i=0; i<simulador->procesos.size(); i++)
    {
     simulador->procesos[i]->consola->show();

    }
}

//Muestra ventanas de estados de procesos
void MainWindow::on_pushButton_5_clicked()
{
    for(int i=0; i<simulador->procesos.size(); i++)
    {
     simulador->procesos[i]->ventanaEstado->show();
    }
}

//Cambia a modo depuracion en la ejecucion de instrucciones
//1: muestra cada instruccion ejecutada
void MainWindow::on_radioButton_clicked()
{
    if( ui->radioButton->isChecked() )
       CPU::setDebug(1);
    else
       CPU::setDebug(0);
}


//Actualiza tiempo de espera en simulacion
void MainWindow::on_hsTime_valueChanged(int value)
{
    CPU::tiempo_espera = value;
    ui->time->setText( QString::number( value ) );
}

//Modifica cantidad de RAM disponible
//si esta en ejecucion, se debe reiniciar o verificar limites
void MainWindow::on_hsRAM_valueChanged(int value)
{
  // if (value> CPU::memoriaOcupada) //NO permitir RAM menor a la cantidad dememoria en uso
       //requerida ubicar el proceso con memoria mas alta (Pendiente  --> ya)
   if (value > CPU::mayorMemProceso )
   {
    CPU::RAM = value;
    ui->line_RAM->setText( QString::number(value) );
    graficador->update();

   }
}

void MainWindow::salir()
{
    if(simulador!=NULL)
    {
        delete simulador;
        simulador=NULL;
    }
    this->close();
}

void MainWindow::ayuda()
{
 helpFile *ayuda1=new helpFile();
 ayuda1->show();
 ayuda1->showFile("../Ejemplos/ayuda1.htm");

}

void MainWindow::acercade()
{
    QString msg;
    msg.append("CPU & OS simulator 3.0\n\n");
    msg.append("Es un software simulador de una computadora virtual, de su sistema operativo y de herramientas propias de la programación de sistemas.\n");
    msg.append("El objetivo original es utilizarlo en cursos de programación de sistemas, "
               "compiladores, sistemas operativos y cursos iniciales de fundamentos"
               " de programación, como recurso didáctico para analizar el funcionamiento de la computadora y de los lenguajes de programación.\n\n");
    msg.append("Contacto: Enrique Ayala Franco\neafranco@gmail.com\n\n");
    msg.append("septiembre, 2016\n");
    QMessageBox::about(this,"Acerca de",msg);
    msg.clear();
}


//Detener simulacion y reiniciar
void MainWindow::on_pushButton_3_clicked()
{
   if(enSimulacion==true){
    simulador->apagar=true;  //en simulacion actual
    ui->label_Simula->setText(" ");
   }
}


//Pausar simulacion
void MainWindow::on_pushButton_7_clicked()
{
     if(enSimulacion==true){
        if(simulador->pausa==false){
          simulador->pausa=true;
          ui->pushButton_7->setText("CONTINUAR");
          ui->label_Simula->setText("SIMULACIÓN EN PAUSA");
        }
       else{
           simulador->pausa=false;
           ui->pushButton_7->setText("PAUSAR");
           ui->label_Simula->setText("SIMULACIÓN EN PROCESO");
        }
     }
}

//Iniciar la simulacion
void MainWindow::on_pushButton_4_clicked()
{
  //ejecutar procesos existentes
    if(enSimulacion==false && simulador->procesos.size()>=1)
     {
        enSimulacion=true;
        ui->label_Simula->setText("SIMULACIÓN EN PROCESO");
        simulador->iniciarSimulacion();  //QUEDA EN CICLO INFINITO HASTA CONCLUIR PROCESOS


        ui->label_Simula->setText(" ");
        QMessageBox::information(this,"Simulador","La simulación ha concluido");

        //prepara la siguiente simulacion
        delete simulador;
        simulador=NULL;

        simulador = new ControlSimulador();
        graficador->recibeProcesos(&simulador->procesos);
        enSimulacion=false;
        simulador->apagar=false;
     }

}



//Abre abre un navegador y  carga archivo ASIM ern lista
void MainWindow::on_pushButton_20_clicked()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this,"","..", tr("archivo ASIM (*.asim)"));

    if(filename.endsWith(".asim")){
      int exist = 0;
       for(int i = 0; i < ui->listWidget_ASM->count(); i++){
                if(ui->listWidget_ASM->item(i)->text() == filename.split('/')[filename.split('/').length()-1]){
                    exist = 1;
                    break;
                }
            }
            if(exist != 1){
                QListWidgetItem *newitem = new QListWidgetItem(ui->listWidget_ASM);
                newitem->setWhatsThis(filename);
                newitem->setText(filename.split('/')[filename.split('/').length()-1]);
                newitem->setToolTip(filename);
            }else{
                mbox.setText("El archivo "+filename.split('/')[filename.split('/').length()-1]+" ya fue agregado");
                mbox.exec();
            }
        }

}


//Coloca nombre de archivo en la lista LM
void MainWindow::on_pushButton_22_clicked()
{
    QString filename;
        filename = QFileDialog::getOpenFileName(this,"","..", tr("ejecutable LM (*.lm)"));

        if(filename.endsWith(".lm")){
           int exist = 0;
            for(int i = 0; i < ui->listWidget_LM->count(); i++){
                if(ui->listWidget_LM->item(i)->text() == filename.split('/')[filename.split('/').length()-1]){
                    exist = 1;
                    break;
            }
            }
            if(exist != 1){
                QListWidgetItem *newitem = new QListWidgetItem(ui->listWidget_LM);
                newitem->setWhatsThis(filename);
                newitem->setText(filename.split('/')[filename.split('/').length()-1]);
                newitem->setToolTip(filename);
            }else{
                mbox.setText("El archivo "+filename.split('/')[filename.split('/').length()-1]+" ya fue agregado");
                mbox.exec();
            }
        }
}


//Crea nuevo archivo ASIM
void MainWindow::on_pushButton_14_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Save File"),"..",tr("ASIM (*.asim)"));
    if(filename.isEmpty())
        return;
    if(filename.split(" ").length()!= 0){

        int exist = 0;
        for(int i = 0; i < ui->listWidget_ASM->count(); i++){
            if(ui->listWidget_ASM->item(i)->text() == filename.split('/')[filename.split('/').length()-1]){
                exist = 1;
                break;
            }

        }
        if(exist != 1){
            QListWidgetItem *newitem = new QListWidgetItem(ui->listWidget_ASM);
            newitem->setWhatsThis(filename);
            newitem->setText(filename.split('/')[filename.split('/').length()-1]);
            newitem->setToolTip(filename);

            QFile file(filename);
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            file.close();

            mFile = new editFile(this);
            mFile->setFileName(filename);
            mFile->show();
        }else{
            mbox.setText("El archivo "+filename.split('/')[filename.split('/').length()-1]+" ya existe");
            mbox.exec();
        }
    }

}

//Edita archivo .ASIM
void MainWindow::on_pushButton_17_clicked()
{
    if(ui->listWidget_ASM->selectedItems().length()>0){
            QString filename = ui->listWidget_ASM->selectedItems()[0]->toolTip();
            mFile = new editFile(this);
            mFile->setFileName(filename);
            mFile->show();
        }
}

//Elimina de la lista el archivo ASIM
void MainWindow::on_pushButton_18_clicked()
{
  ui->listWidget_ASM->takeItem(ui->listWidget_ASM->currentIndex().row());
}

//Ver archivo LM sin modificar
void MainWindow::on_pushButton_21_clicked()
{
    if(ui->listWidget_LM->selectedItems().length()>0){
        QString filename = ui->listWidget_LM->selectedItems()[0]->toolTip();
        mFile = new editFile(this);
        mFile->setFileName(filename);
        mFile->show();
    }
}

//Elimina de lista archivo LM
void MainWindow::on_pushButton_6_clicked()
{
   ui->listWidget_LM->takeItem(ui->listWidget_LM->currentIndex().row());
}


//Carga archivo en lista de espera de procesos para la ejecución
void MainWindow::on_pushButton_15_clicked()
{
    if(ui->listWidget_LM->selectedItems().length()>0){
        int exist = 0;
        for(int i = 0; i < ui->listWidget_Process->count(); i++){
          if(ui->listWidget_Process->item(i)->text()==
                   ui->listWidget_LM->currentItem()->text().split(".lm")[0]){
           exist = 1;
           break;
         }
        }
        if(exist != 1){
         QString filename = ui->listWidget_LM->currentItem()->toolTip();
         QListWidgetItem *newitem = new QListWidgetItem(ui->listWidget_Process);
         newitem->setWhatsThis(filename);
         QString nombre = (filename.split('/')[filename.split('/').length()-1]).split(".lm")[0];
         newitem->setText(nombre);

         newitem->setToolTip(filename);
         ui->statusBar->showMessage("Archivo ejecutable "+nombre+" fue agregado a ventana de simulación",16000);
        }
        else{
         mbox.setText("El archivo "+ui->listWidget_LM->currentItem()->text()+" ya fue agregado");
         mbox.exec();
        }
      }
 }


// Ejecuta el Programa ensamblador para generar código ejecutable
void MainWindow::on_pushButton_19_clicked()
{
    if(ui->listWidget_ASM->selectedItems().length()>0){
        QString filename = ui->listWidget_ASM->selectedItems()[0]->toolTip();

        asim archivo(filename); //ejecuta y deja en directorio el resultado
        if(archivo.hayerrores){
          ui->statusBar->showMessage("Ejecutable NO GENERADO hay errores en el código fuente!!",15000);
          return;
        }

        filename = filename.split(".asim")[0]+".lm";
        int exist = 0;
        for(int i = 0; i < ui->listWidget_LM->count(); i++){
            if(ui->listWidget_LM->item(i)->text() == filename.split('/')[filename.split('/').length()-1]){
                exist = 1;
                break;
            }

        }
        if(exist != 1){
            QListWidgetItem *newitem = new QListWidgetItem(ui->listWidget_LM);
            newitem->setWhatsThis(filename);
            newitem->setText(filename.split('/')[filename.split('/').length()-1]);
            newitem->setToolTip(filename);
            ui->statusBar->showMessage("Archivo ejecutable generado con éxito!!",16000);
        }else{
            QMessageBox mbox;
            mbox.setText("El archivo "+filename.split('/')[filename.split('/').length()-1]+" ya fue agregado");
            mbox.exec();
        }

    }

}

//carga ventana para visualizar memoria
void MainWindow::on_pushButton_8_clicked()
{
   // graficador = new showmem(this);
    if(simulador!=NULL) {
     graficador->recibeProcesos(&simulador->procesos);
     graficador->show();
  //  graficador->update();
     QTimer::singleShot(1000, this, SLOT(timerUpdate()));
    }
}

//Para actualizar la pantalla del monitor de memoria
void MainWindow::timerUpdate()
{
   if(simulador!=NULL) {
    graficador->update();
    QTimer::singleShot(1000, this, SLOT(timerUpdate()));
    //volver a llamar cada segundo
   }
}

//Quita imagen de lista de procesos para simulación
//si esta en ejecucion no termina el proceso
void MainWindow::on_pushButton_9_clicked()
{
     ui->listWidget_Process->takeItem(ui->listWidget_Process->currentIndex().row());

}
