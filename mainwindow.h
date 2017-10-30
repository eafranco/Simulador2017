#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "controlsimulador.h"
#include "editfile.h"
#include "statusmemory.h"
#include "showmem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_7_clicked();

    void on_radioButton_clicked();

    void on_hsTime_valueChanged(int value);

    void on_hsRAM_valueChanged(int value);

    void salir();

    void ayuda();

    void acercade();

    void on_pushButton_19_clicked();

    void on_pushButton_8_clicked();

    void timerUpdate();

    void on_pushButton_9_clicked();

private:
    Ui::MainWindow *ui;
    ControlSimulador *simulador;
    bool enSimulacion;
    QMessageBox mbox;
    editFile *mFile;
    showmem* graficador;
};

#endif // MAINWINDOW_H
